#ifndef PWM_H
#define	PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1
void InitPWM(void);
//void PWMSetSpeed(int,float);
void PWMUpdateSpeed(void);
void PWMSetSpeedConsigne(float,int);
#endif /* PWM_H */

