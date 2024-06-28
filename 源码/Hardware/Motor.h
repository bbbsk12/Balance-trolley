#ifndef __MOTOR_H
#define __MOTOR_H
void Motor_Init(void);	//INÒý½Å³õÊ¼»¯
void Motor_PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
void Load(int Motor1,int Motor2);
void Limit(int* MOTO1,int* MOTO2);
#endif
