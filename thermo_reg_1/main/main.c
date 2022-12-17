#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "init_ESP.h"
#include "mcp3425a1.h"
#include "ssd1306.h"
#include "enkoder_isr.h"
#include "pid.h"
#include "math.h"
#include <time.h>

#include "wifi_sta.h"

#include <sntp.h>

float measTemp (void);

void app_main (void)
{
  int countTime=0;
  float temp;
  char displayBuf[50];
  bool programMode=false;
  bool flagMenu=true;
  int currentMenu=TEMP;
  int buttonTime=0;
  initSW ();
  initenkoder ();

  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;

  setenv("TZ", "EST-2", 1);
  tzset();

  gpio_set_level (LIGHT_PWM, 1);
  gpio_set_level (SWITCH_2, 0);
  gpio_set_level (SWITCH_3, 0);
  gpio_set_level (SWITCH_1, 0);

  displayInit ();
  initPid ();
  setZad(41.5);

  setKp(200);
  setKid(0.5);
  setKd(25);

  SSD1306_Init ();

  startSta();

  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();

  while (true)
    {
      vTaskDelay (10 / portTICK_PERIOD_MS);
      //cw=getButon();
      //ccw=getEnkoder();
      //sleep(1);

      //rtc
      countTime++;
      if(countTime>10)
	{
	  countTime=0;
	  time(&now);
	  localtime_r(&now, &timeinfo);
	  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	  printf( "The current date/time in Bulgaria is: %s \n", strftime_buf);
	}


      //Measure current temp
      temp=measTemp();
      //Set current temp to PID for process
      setIzmerena(temp);

      //Print temp on display
      sprintf (displayBuf, "%.3f    ", temp);
      SSD1306_GotoXY (8, 5);
      SSD1306_Puts (displayBuf, &Font_11x18, SSD1306_COLOR_WHITE);

      if(!programMode)
	{
      //print pid on display
//      sprintf (displayBuf, "%f    ", getPid());
	  sprintf (displayBuf, "%d    ", getEnkoder());
      SSD1306_GotoXY (8, 30);
      SSD1306_Puts (displayBuf, &Font_11x18, SSD1306_COLOR_WHITE);
	}
      else
	{
	  switch (currentMenu)
	  {
		case TEMP:
		  sprintf (displayBuf, "T=%.2f    ", getZad());
		  break;
		case PrK:
		  sprintf (displayBuf, "Kp=%.2f    ", getKp());
		  break;
		case IK:
		  sprintf (displayBuf, "Ki=%.2f    ", getKid());
		  break;
		case DK:
		  sprintf (displayBuf, "Kd=%.2f    ", getKd());
		  break;
		case LIMIT:
		  sprintf (displayBuf, "Lim=%f    ", getLimit());
		 break;
		default:
		  break;
			  }
		SSD1306_GotoXY (8, 30);
	        SSD1306_Puts (displayBuf, &Font_11x18, SSD1306_COLOR_WHITE);
	}
      SSD1306_UpdateScreen ();

      //Pid On/Off
      if(getPid()>0)
	{
	  gpio_set_level (SWITCH_2, 1);
	}
      else
	{
	  gpio_set_level (SWITCH_2, 0);
	}

      //Check enter/exit programing mode (button 5s)
      if(gpio_get_level(NO_ENK)!= 1)
	{
	  buttonTime++;
	  if(buttonTime==15)
	    {
	      programMode=!programMode;
	      getButon();
	      clearEnkoder();
	      buttonTime=0;
	      flagMenu=0;
	      currentMenu=TEMP;
	    }

	}
      else
	{
	  buttonTime=0;
	  //getButon();
	}

      //Parse program mode
      if(programMode)
	{
	  if(getButon()==1)
	    {
	      flagMenu=!flagMenu;
	    }
	  if(flagMenu)
	    {
	      currentMenu=currentMenu+getEnkoder();

	      clearEnkoder();
	      if(currentMenu<0)
		{
		  currentMenu=TEMP;
		}
	      if(currentMenu>=MAXMENU)
		{
		  currentMenu=LIMIT;
		}
	    }
	  else
	    {
	      switch (currentMenu)
	      {
		case TEMP:
		  setZad(getZad()+(float)(getEnkoder())/10.0);
		  break;
		case PrK:
		  setKp(getKp()+(float)(getEnkoder())/10.0);
		  break;
		case IK:
		  setKid(getKid()+(float)(getEnkoder())/10.0);
		  break;
		case DK:
		  setKd(getKd()+(float)(getEnkoder())/10.0);
		  break;
		case LIMIT:
		  setLimit(getLimit()+(float)(getEnkoder()));
		  break;
		default:
		  break;
	      }
	      clearEnkoder();
	    }
	}

    }


}

float measTemp (void)
{
  float temp;
  float res;
  float volt;

  volt = meassureMCP (SPS_15, PGA_1);
  res = 1000000 * volt / 253.4;
  temp = (1 / ((log (res / 10000) / 3988) + 1 / 298.15)) - 273.15;
  return temp;
}
