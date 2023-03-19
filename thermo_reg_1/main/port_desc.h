/*
 * port_deskripsion.h
 *
 *  Created on: 5.09.2022 г.
 *      Author: dida
 */

#ifndef MAIN_PORT_DESC_H_
#define MAIN_PORT_DESC_H_

#define CCW 		GPIO_NUM_35
#define NO_ENK 		GPIO_NUM_32
#define CW 			GPIO_NUM_33
#define SWITCH_2 	GPIO_NUM_27
#define SWITCH_3 	GPIO_NUM_4
#define SWITCH_1 	GPIO_NUM_22
#define SDA 		GPIO_NUM_25
#define SCL 		GPIO_NUM_26
#define LIGHT_PWM 	GPIO_NUM_5
#define IN_MASK 	((1ULL<<CCW) | (1ULL<<NO_ENK) | (1ULL<<CW))
#define OUT_MASK 	((1ULL<<SWITCH_2) | (1ULL<<SWITCH_3) | (1ULL<<SWITCH_1) | (1ULL<<LIGHT_PWM))

struct OperationParam
{
	float zadanie;
	float kp;
	float kid;
	float limit;
	float kd;
	float timeStart;
	float timeStop;
	float time;
	bool rele1On;
	bool rele3On;
};

#endif /* MAIN_PORT_DESC_H_ */
