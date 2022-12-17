/*
 * enkoder_isr.c
 *
 *  Created on: 8.09.2022 г.
 *      Author: dida
 */

#include "enkoder_isr.h"
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

#define ESP_INTR_FLAG_DEFAULT 0

int count = 0;
int buton = 0;
static void IRAM_ATTR enkoder_isr(void* ccwin)
{
	int cw;
	int ccw;
	int no_enk;
	int aa;
	aa= ccwin;

	cw=gpio_get_level(CW);
	ccw=gpio_get_level(CCW);
	no_enk=gpio_get_level(NO_ENK);


	if(aa==NO_ENK)
	{
		if(no_enk == 0)
		{
			buton = 1;
		}
	}
	if((aa == CW)/* || (aa == CCW)*/)
	{
		if(cw == 1)
		{
			if(ccw == 1)
			{
				count = count - 1;
			}
			else
			{
				count = count  + 1;
			}
		}
		else
		{
			if(ccw == 1)
			{
				count = count + 1;
			}
			else
			{
				count = count  - 1;
			}
		}
	}
}

int getButon(void)
{
	int  result = buton;
	buton = 0;
	return result;
}



int getEnkoder(void)
{
	return count;
}

void clearEnkoder(void)
{
  count=0;
}

void initenkoder(void)
{
	gpio_config_t initSW;
	initSW.intr_type = GPIO_INTR_ANYEDGE;
	initSW.mode = GPIO_MODE_INPUT;
	initSW.pin_bit_mask = IN_MASK;
	initSW.pull_down_en = GPIO_PULLDOWN_DISABLE;
	initSW.pull_up_en = GPIO_PULLUP_ENABLE;
	gpio_config(&initSW);

	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	gpio_isr_handler_add(CCW, enkoder_isr, (void*) CCW);
	gpio_isr_handler_add(CW, enkoder_isr, (void*) CW);
	gpio_isr_handler_add(NO_ENK, enkoder_isr, (void*) NO_ENK);
}


