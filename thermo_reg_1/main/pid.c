/*
 * pid.c
 *
 *  Created on: 15.09.2022 г.
 *      Author: dida
 */

#include <driver/gpio.h>
#include "port_desc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_intr_alloc.h"
#include "pid.h"

static int test = 0;

static long tZad = 0;
static long tIzm = 0;
static long kP = 1;
static long sum = 0;
static long kI = 1;
static long kD = 1;
static long dErr = 0;
static long pidResul = 0;
static long liminIntegrate = 100000;



static long debug;

void initPid (void)
{
  gptimer_handle_t gptimer = NULL;
  gptimer_config_t timer_config;
  timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
  timer_config.direction = GPTIMER_COUNT_UP;
  timer_config.flags.intr_shared = true;
  timer_config.resolution_hz = 1000000; // 1Hz, 1 tick = 1us

  gptimer_alarm_config_t alarm_config;
  alarm_config.reload_count = 0;
  alarm_config.flags.auto_reload_on_alarm = true;
  alarm_config.alarm_count = 1000000;

  gptimer_event_callbacks_t timerCB;
  timerCB.on_alarm = pidEvent;

  gptimer_new_timer (&timer_config, &gptimer);

  gptimer_set_alarm_action (gptimer, &alarm_config);
  gptimer_register_event_callbacks (gptimer, &timerCB, (void*) (NULL));
  gptimer_enable (gptimer);
  gptimer_start (gptimer);

  printf ("Timer init\n");
}

static bool pidEvent (gptimer_handle_t timer,
		      const gptimer_alarm_event_data_t *edata, void *user_ctx)
{

  pid ();
  return true;
}

int getTimerTest (void)
{
  return test;
}

void pid (void)
{
  long dif;
  long integr;
  long propor;
  long errT;


  errT = tZad - tIzm;


  propor = errT * kP;


 sum = sum + errT;
  if (sum > liminIntegrate)
    {
      sum = liminIntegrate;
    }
  if (sum < (liminIntegrate * (-1)))
    {
      sum = liminIntegrate * (-1);
    }

 integr = sum * kI;


  dif = kD * (errT - dErr);

  dErr = errT;

  pidResul = propor + integr + dif;
}

float getKp (void)
{
  float k = (float) (kP);
  return k/100.0;
}

void setKp (float a)
{
  float k =a*100.0;
  kP =(long) (k);
}

float getKd (void)
{
  float k = (float) (kD);
  return kD/100.0;
}

void setKd (float a)
{
  float k = a*100.0;
  kD = (long) (k);
}

float getKid (void)
{
  float k = (float) (kI);
  return kI/100.0;
}

void setKid (float a)
{
  float k = a*100.0;
  kI = (long) (k);
}

float getZad (void)
{
  float result;
  result=(float)(tZad);
  result=result/100.0;
  return result;
}

void setZad (float a)
{
  float zadanie = a*100.0;
  tZad = (long) (zadanie);
}

float getIzmerena (void)
{
  float t=(float)(tIzm);
  return t/100.0;
}

void setIzmerena (float a)
{
  float i = a*100.0;
  tIzm = (long) (i);
}

float getLimit (void)
{
  float k = (float)(liminIntegrate);
  return k/100.0;
}

void setLimit (float a)
{
  float k= a*100.0;
  liminIntegrate = (long)(k);
}
float getPid (void)
{
  float k = (float)(pidResul);
  return k/10000.0;
}

void resetPid (void)
{
  tIzm = 0;
  sum = 0;
  dErr = 0;
  pidResul = 0;
}















float getErr (void)
{
  float deb = (float) (debug);
  return deb/10000.0;
}
