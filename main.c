#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    InitPWM(); 
 //  PWMSetSpeed(0,0);
 //  PWMSetSpeed(1,0);
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;
    
    InitTimer1();
    InitTimer23();
    

    
    

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        if(ADCIsConversionFinished()){
            unsigned int * result = ADCGetResult() ;
            ADCClearConversionFinishedFlag();
            char ADCValue0=result[0];
            char ADCValue1=result[1];
            char ADCValue2=result[2];
        }
        }
    } // fin main
