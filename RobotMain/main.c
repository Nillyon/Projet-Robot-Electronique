#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entr�es sorties
    /****************************************************************************************************/
    InitIO();
    InitPWM();

    //  PWMSetSpeed(0,0);
    //  PWMSetSpeed(1,0);
    LED_BLANCHE = 0;
    LED_BLEUE = 0;
    LED_ORANGE = 0;

    InitTimer1();
//    InitTimer23();
    InitTimer4();
    InitADC1();

    



    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        if(ADCIsConversionFinished()){
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result[0])*3.3/4096 *3.2;
            robotState.distanceTelemetreDroit=34/volts-5;
            volts = ((float) result[1])*3.3/4096 *3.2;
            robotState.distanceTelemetreCentre_Droit=34/volts-5;
            volts = ((float) result[2])*3.3/4096 *3.2;
            
            robotState.distanceTelemetreCentre=34/volts-5;
            volts = ((float) result[3])*3.3/4096 *3.2;
            robotState.distanceTelemetreGauche=34/volts-5;
            volts = ((float) result[4])*3.3/4096 *3.2;
            robotState.distanceTelemetreCentre_Gauche=34/volts-5;
            
        }
        
        
    }
}
unsigned char stateRobot;

    void OperatingSystemLoop(void) {
        switch (stateRobot) {
            case STATE_ATTENTE:
                timestamp = 0;
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_ATTENTE_EN_COURS;

            case STATE_ATTENTE_EN_COURS:
                if (timestamp > 1000)
                    stateRobot = STATE_AVANCE;
                break;

            case STATE_AVANCE:
                PWMSetSpeedConsigne(25, MOTEUR_DROIT);
                PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
                stateRobot = STATE_AVANCE_EN_COURS;
                break;
            case STATE_AVANCE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_GAUCHE:
                PWMSetSpeedConsigne(10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_DROITE:
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
            
                
            case STATE_TOURNE_CENTRE_GAUCHE:
                PWMSetSpeedConsigne(20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_CENTRE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_CENTRE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_CENTRE_DROITE:
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_CENTRE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_CENTRE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;    
                

            case STATE_TOURNE_SUR_PLACE_GAUCHE:
                PWMSetSpeedConsigne(15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_DROITE:
                PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            default:
                stateRobot = STATE_ATTENTE;
                break;
        }
    }

    unsigned char nextStateRobot = 0;

    void SetNextRobotStateInAutomaticMode() {
        unsigned char positionObstacle = PAS_D_OBSTACLE;

        //D�termination de la position des obstacles en fonction des t�l�m�tres
        if ((robotState.distanceTelemetreCentre_Droit < 30 || robotState.distanceTelemetreDroit <40)&&
                robotState.distanceTelemetreCentre > 20 &&
                robotState.distanceTelemetreCentre_Gauche > 30 &&
                robotState.distanceTelemetreGauche >30) //Obstacle centre-droite
            positionObstacle = OBSTACLE_CENTRE_DROITE;
        else if (robotState.distanceTelemetreCentre_Droit > 30 &&
                robotState.distanceTelemetreDroit>30 &&
                robotState.distanceTelemetreCentre > 20 &&
                (robotState.distanceTelemetreCentre_Gauche < 30 || robotState.distanceTelemetreGauche <40)) //Obstacle centre-gauche
            positionObstacle = OBSTACLE_CENTRE_GAUCHE;
        else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
            positionObstacle = OBSTACLE_EN_FACE;
        else if (robotState.distanceTelemetreCentre_Droit > 30 &&
                robotState.distanceTelemetreCentre > 20 &&
                robotState.distanceTelemetreCentre_Gauche > 30 &&
                robotState.distanceTelemetreDroit > 30 &&
                robotState.distanceTelemetreGauche > 30) //pas d?obstacle           
            positionObstacle = PAS_D_OBSTACLE;
        else if (robotState.distanceTelemetreDroit<30 &&
                robotState.distanceTelemetreCentre_Droit>30 &&
                robotState.distanceTelemetreCentre>20 &&
                robotState.distanceTelemetreCentre_Gauche>30 &&
                robotState.distanceTelemetreGauche>30) //obstacle � gauche
            positionObstacle=OBSTACLE_A_DROITE;
        else if(robotState.distanceTelemetreDroit>30 &&
                robotState.distanceTelemetreCentre_Droit>30 &&
                robotState.distanceTelemetreCentre>20 &&
                robotState.distanceTelemetreCentre_Gauche>30 &&
                robotState.distanceTelemetreGauche<30) //obstacle � droite
            positionObstacle=OBSTACLE_A_GAUCHE;
        //D�termination de l?�tat � venir du robot
        if (positionObstacle == PAS_D_OBSTACLE)
            nextStateRobot = STATE_AVANCE;
        else if (positionObstacle == OBSTACLE_CENTRE_DROITE)
            nextStateRobot = STATE_TOURNE_CENTRE_GAUCHE;
        else if (positionObstacle == OBSTACLE_CENTRE_GAUCHE)
            nextStateRobot = STATE_TOURNE_CENTRE_DROITE;
        else if (positionObstacle == OBSTACLE_A_DROITE)
            nextStateRobot = STATE_TOURNE_GAUCHE;
        else if (positionObstacle == OBSTACLE_A_GAUCHE)
            nextStateRobot = STATE_TOURNE_DROITE;        
        else if (positionObstacle == OBSTACLE_EN_FACE)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

        //Si l?on n?est pas dans la transition de l?�tape en cours
        if (nextStateRobot != stateRobot - 1)
            stateRobot = nextStateRobot;
    }
// fin main
