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
#include "port_desc.h"
#include <time.h>
#include <vector>
#include <string>
#include "main.h"
//#include <map>

#include "wifi_sta.h"
#include "tcp_ip_server.h"

#include <sntp.h>


OperationParam currentParam;

std::vector<OperationParam> currentProg;

std::vector<ProgramStruct> programs;

float temp;
bool programStarted;
time_t timeStart;


extern "C"

void app_main (void)
{
  int countTime=0;

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

  programStarted=false;

  gpio_set_level (LIGHT_PWM, 1);
  gpio_set_level (SWITCH_2, 0);
  gpio_set_level (SWITCH_3, 0);
  gpio_set_level (SWITCH_1, 0);

  displayInit ();
  initPid ();
  currentParam.zadanie=41.5;
  currentParam.kp=200;
  currentParam.kid=0.5;
  currentParam.kd=25;
  currentParam.limit=getLimit();
  currentParam.rele1On=false;
  currentParam.rele3On=false;
  currentParam.time=0;
  currentParam.timeStart=0;
  currentParam.timeStop=0;

  setZad(currentParam.zadanie);
  setKp(currentParam.kp);
  setKid(currentParam.kid);
  setKd(currentParam.kd);

  debugProg();

  SSD1306_Init ();

  startSta();
  xTaskCreate(tcp_server_task, "tcp_server", 4096, (void*)AF_INET, 5, NULL);
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
      time(&now);

      if(countTime>10)
	{
	  countTime=0;
	  localtime_r(&now, &timeinfo);
	  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	  printf( "The current date/time in Bulgaria is: %s \n", strftime_buf);
	}
      //Measure time between start and current moment in seconds
      uint64_t deltaStartInt=now-timeStart;
      //printf("now=%lld timeStart=%lld\n", now, timeStart);
      float deltaStartFloat = static_cast<float>(deltaStartInt);

      if(programStarted)
      {

    	  printf("starrting the program\n");
    	  programStarted = false;
		  for(uint z=0; z<currentProg.size(); z++)
		  {
			  printf("currentProg[z].timeStart=%f deltaStartFloat=%f deltaStartInt=%lld currentProg[z].timeStop=%f\n", currentProg[z].timeStart, deltaStartFloat, deltaStartInt, currentProg[z].timeStop);
			  if((currentProg[z].timeStart<=deltaStartFloat) && (currentProg[z].timeStop>deltaStartFloat))
			  {
				  printf("wliam\n");
				  programStarted = true;
				  currentParam = currentProg[z];
				  printf("currentParam zadanie = %f\ currentProg zadanie = %f \n", currentParam.zadanie, currentProg[z].zadanie);
			  }
		  }
      }

      setZad(currentParam.zadanie);
      setKp(currentParam.kp);
      setKid(currentParam.kid);
      setKd(currentParam.kd);
      setLimit(currentParam.limit);

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
		  //sprintf (displayBuf, "T=%.2f    ", getZad());
			sprintf (displayBuf, "T=%.2f    ", currentParam.zadanie);
		  break;
		case PrK:
		  //sprintf (displayBuf, "Kp=%.2f    ", getKp());
			sprintf (displayBuf, "Kp=%.2f    ", currentParam.kp);
		  break;
		case IK:
		  //sprintf (displayBuf, "Ki=%.2f    ", getKid());
			sprintf (displayBuf, "Ki=%.2f    ", currentParam.kid);
		  break;
		case DK:
		  //sprintf (displayBuf, "Kd=%.2f    ", getKd());
			sprintf (displayBuf, "Kd=%.2f    ", currentParam.kd);
		  break;
		case LIMIT:
		  //sprintf (displayBuf, "Lim=%f    ", getLimit());
			sprintf (displayBuf, "Lim=%f    ", currentParam.limit);
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
			//currentParam.zadanie=getZad()+(float)(getEnkoder())/10.0;
			currentParam.zadanie=currentParam.zadanie+(float)(getEnkoder())/10.0;
		  setZad(currentParam.zadanie);
		  break;
		case PrK:
			//currentParam.kp=getKp()+(float)(getEnkoder())/10.0;
			currentParam.kp=currentParam.kp+(float)(getEnkoder())/10.0;
		  setKp(currentParam.kp);
		  break;
		case IK:
			//currentParam.kid=getKid()+(float)(getEnkoder())/10.0;
			currentParam.kid=currentParam.kid+(float)(getEnkoder())/10.0;
		  setKid(currentParam.kid);
		  break;
		case DK:
			//currentParam.kd=getKd()+(float)(getEnkoder())/10.0;
			currentParam.kd=currentParam.kd+(float)(getEnkoder())/10.0;
		  setKd(currentParam.kd);
		  break;
		case LIMIT:
			//currentParam.limit=getLimit()+(float)(getEnkoder());
			currentParam.limit=currentParam.limit+(float)(getEnkoder());
		  setLimit(currentParam.limit);
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


float getMeasTemp (void)
{
  return temp;
}



OperationParam getCurrentParam (void)
{
  return currentParam;
}

void setCurrentParam (OperationParam a)
{
  currentParam =a;
}

void addProg(ProgramStruct tobeadd)
{
	programs.push_back(tobeadd);
}


void delProg(std::string programme_name_toberm)
{
	for(int i=0; i<programs.size(); i++)
	{
		if(programs[i].progName.compare(programme_name_toberm))
		{
			programs.erase(programs.begin()+i);
		}
	}

}


std::vector<ProgramStruct> getProgram (void)
{
  return programs;
}


void debugProg(void)
{
	OperationParam debugcurrentparams;
	debugcurrentparams.zadanie=45;
	debugcurrentparams.kp=200;
	debugcurrentparams.kid=0.5;
	debugcurrentparams.kd=25;
	debugcurrentparams.limit=getLimit();
	debugcurrentparams.rele1On=true;
	debugcurrentparams.rele3On=false;
	debugcurrentparams.time=3600;
	debugcurrentparams.timeStart=0;
	debugcurrentparams.timeStop=0;

	currentProg.push_back(debugcurrentparams);

	debugcurrentparams.zadanie=32;
	debugcurrentparams.kp=200;
	debugcurrentparams.kid=0.5;
	debugcurrentparams.kd=25;
	debugcurrentparams.limit=getLimit();
	debugcurrentparams.rele1On=false;
	debugcurrentparams.rele3On=false;
	debugcurrentparams.time=5*3600;
	debugcurrentparams.timeStart=0;
	debugcurrentparams.timeStop=0;

	currentProg.push_back(debugcurrentparams);

	ProgramStruct debugprogrammestruct;
	debugprogrammestruct.progName="prog1";
	debugprogrammestruct.program=currentProg;

	programs.push_back(debugprogrammestruct);


	//2ns prog
	currentProg.clear();
	debugcurrentparams.zadanie=78;
	debugcurrentparams.kp=200;
	debugcurrentparams.kid=0.5;
	debugcurrentparams.kd=25;
	debugcurrentparams.limit=getLimit();
	debugcurrentparams.rele1On=false;
	debugcurrentparams.rele3On=false;
	debugcurrentparams.time=2*3600;
	debugcurrentparams.timeStart=0;
	debugcurrentparams.timeStop=0;

	currentProg.push_back(debugcurrentparams);

	debugcurrentparams.zadanie=60;
	debugcurrentparams.kp=200;
	debugcurrentparams.kid=0.5;
	debugcurrentparams.kd=25;
	debugcurrentparams.limit=getLimit();
	debugcurrentparams.rele1On=false;
	debugcurrentparams.rele3On=false;
	debugcurrentparams.time=9*3600;
	debugcurrentparams.timeStart=0;
	debugcurrentparams.timeStop=0;

	currentProg.push_back(debugcurrentparams);


	debugprogrammestruct.progName="prog2";
	debugprogrammestruct.program=currentProg;

	programs.push_back(debugprogrammestruct);

}


void delAllProgs(void)
{
	programs.clear();
}

void startProgram(std::string nameDesiredPrg)
{
	//std::vector<OperationParam> currentProg;
	printf("desired prog: %s\n", nameDesiredPrg.c_str());
	for(uint i=0; i<programs.size(); i++)
	{
		printf("progs[i]=%s desired=%s strcmp=%d\n", programs[i].progName.c_str(), nameDesiredPrg.c_str(), strcmp(programs[i].progName.c_str(), nameDesiredPrg.c_str()));

		if(!strcmp(programs[i].progName.c_str(), nameDesiredPrg.c_str()))
		{
			printf("hm1\n");
			currentProg=programs[i].program;
			time(&timeStart);
			float currentTimeShift;
			currentTimeShift = 0;
			for(uint j=0; j < currentProg.size(); j++)
			{
				currentProg[j].timeStart=currentTimeShift;
				currentTimeShift=currentTimeShift+currentProg[j].time;
				currentProg[j].timeStop=currentTimeShift;
			}
			programStarted=true;
		}
	}
}

