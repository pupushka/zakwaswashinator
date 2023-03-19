/*
 * pid.h
 *
 *  Created on: 15.09.2022 г.
 *      Author: dida
 */

#ifndef MAIN_PID_H_
#define MAIN_PID_H_
#include <driver/gptimer.h>
bool pidEvent(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

void initPid (void);
int getTimerTest(void);
void pid(void);
float getKp (void);
void setKp (float a);
float getKd (void);
void setKd (float a);
float getKid (void);
void setKid (float a);
float getZad (void);
void setZad (float a);
float getIzmerena (void);
void setIzmerena (float a);
float getLimit (void);
void setLimit (float a);
void resetPid (void);
float getPid (void);




float getErr (void);

#endif /* MAIN_PID_H_ */
