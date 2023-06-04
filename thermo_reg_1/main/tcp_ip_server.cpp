/*
 * tcp_server.c
 *
 *  Created on: 18.02.2023 г.
 *      Author: dida
 */

/* BSD Socket API Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "tcp_ip_server.h"
#include "port_desc.h"
#include "main.h"
#include "utils.h"
#include "rom/ets_sys.h"


static const char *TAG = "tcpipexample";



void comunication_commands(const int sock)
{
	int8_t cmd;
	char command_mssg[128];

	char number_progs[10];
	char nameProg[128];
	char numStepsProg[128];
	char get_kp_value[128];
	char meas_temp[128];
	char set_zadanie_value[128];
	char get_zadanie_value[128];
	char * pch;
	std::vector<ProgramStruct> tmpProg;
	int len;
	int len_program_bytes;
	uint64_t ch;
	int numb_progs_struct=0;
	char numb_progs_struct_str[128];


	char program_bytes[512];
	char *p_begin_prog;
	int numb_steps;
	ProgramStruct tmpProgReadStruct;
	OperationParam tmpParamReadStruct;
	OperationParam tmpParam;
	do
	{
	    len=recv(sock,command_mssg, sizeof(command_mssg)-1, 0);
	    command_mssg[len]=0;
	    ESP_LOGI(TAG, "command msg: %s", command_mssg);
	    cmd = get_cmd_id( command_mssg );

	    tmpParam=getCurrentParam();


	    switch ( cmd )
	    {
    		case GETMEASTEMP:
    			sprintf(meas_temp, "%f\n", getMeasTemp());
    			send(sock, meas_temp, strlen(meas_temp) , 0);
    			ESP_LOGI(TAG, "meas temp: %s\n", meas_temp);
    			break;

    		case GETZADANIE:
    			sprintf(get_zadanie_value, "%f\n", tmpParam.zadanie);
    			send(sock, get_zadanie_value, strlen(get_zadanie_value) , 0);
    			ESP_LOGI(TAG, "read zadanie: %s\n", get_zadanie_value);
    		    break;

    		case SETZADANIE:
    			ESP_LOGI(TAG, "write msg: %s\n", command_mssg);
    			strncpy(set_zadanie_value, (char*)memmove(command_mssg, command_mssg+11, strlen(command_mssg)), strlen(command_mssg));
    			tmpParam.zadanie=atof(set_zadanie_value);
    			printf("set zadanie  %f\n", tmpParam.zadanie);
    			setCurrentParam(tmpParam);
    			//setKp(atof(set_kp_value)); // da prpwerq dali naistina e float dali ima tochka i chisla
    			printf("setted zadanie  %s\n", set_zadanie_value);
    		    break;

	    	case GETKP:
	    		sprintf(get_kp_value, "%f\n", tmpParam.kp);
	    		send(sock, get_kp_value, strlen(get_kp_value) , 0);
	    		ESP_LOGI(TAG, "read kp: %s\n", get_kp_value);
	    		break;
	    	case NUMBPROG:
	    		tmpProg=getProgram();
	    		sprintf(number_progs,"%d\n",tmpProg.size());
	    		send(sock, number_progs, strlen(number_progs), 0);
	    		ESP_LOGI(TAG, "numb prog: %d\n", tmpProg.size());
	    		break;
	    	case PROGNAMES:
	    		pch=strstr(command_mssg, "=");
	    		if(pch!=NULL)
	    		{
	    			pch=strstr(pch, "=");
	    			ch=ConvertASCItouint64(pch+1);
	    			tmpProg=getProgram();
	    			printf("ch= %lld\n", ch);

	    			//for(int i=0; i<tmpProg.size(); i++)
	    			//{
	    				   sprintf(nameProg, "%s\n", tmpProg[ch].progName.c_str());
	    					//sprintf(nameProg, "abcdefg\n");
	    				   //printf("progName=%s\n", nameProg);
	    				   //ESP_LOGI(TAG, "Prog Name: %s \n", tmpProg[i].progName.c_str());
	    				   send(sock, nameProg, strlen(nameProg), 0);
	    			//}
	    		}

	    		break;
	    	case PROGSTEPSNUMB:
	    		pch=strstr(command_mssg, "=");
	    		if(pch!=NULL)
	    		{
	    			pch=strstr(pch, "=");
	    			ch=ConvertASCItouint64(pch+1);
	    			tmpProg=getProgram();
	    		//for(int i=0; i<tmpProg.size(); i++)
	    		//{
	    		    sprintf(numStepsProg, "%d\n", tmpProg[ch].program.size());
	    			send(sock, numStepsProg, strlen(numStepsProg), 0);
	    		//}
	    		}

	    		break;
	    	case GETPROGRAMS:
	    		pch=strstr(command_mssg, "=");
	    		if(pch!=NULL)
	    		{
	    			pch=strstr(pch, "=");
	    			ch=ConvertASCItouint64(pch+1);
	    			tmpProg=getProgram();
	    			for(int j=0; j<tmpProg[ch].program.size(); j++)
	    			{
	    				send(sock, &tmpProg[ch].program[j], sizeof(OperationParam)*tmpProg[ch].program.size(), 0);
	    				printf("size=%d\n", sizeof(OperationParam)*tmpProg[ch].program.size());
	    			}
	    		}
	    		break;

	    	case PRINTVECT:
	    		tmpProg=getProgram();
	    		for(int i=0; i<tmpProg.size(); i++)
	    		{
	    			printf("progname %d=%s\n", i, tmpProg[i].progName.c_str() );
	    			for(int j=0; j<tmpProg[i].program.size(); j++)
	    			{
	    				printf("zadanie i=%d=%f\n", i, tmpProg[i].program[j].zadanie);
	    				printf("kp i=%d=%f\n", i, tmpProg[i].program[j].kp);
	    				printf("kid i=%d=%f\n", i, tmpProg[i].program[j].kid);
	    				printf("limit i=%d=%f\n", i, tmpProg[i].program[j].limit);
	    				printf("kd i=%d=%f\n", i, tmpProg[i].program[j].kd);
	    				printf("timeStart i=%d=%f\n", i, tmpProg[i].program[j].timeStart);
	    				printf("timeStop i=%d=%f\n", i, tmpProg[i].program[j].timeStop);
	    				printf("time i=%d=%f\n", i, tmpProg[i].program[j].time);

	    			}
	    		}

	    		break;

	    	case PICK:
	    		pch=strstr(command_mssg, "=");
	    		if(pch!=NULL)
	    		{
	    			pch=strstr(pch, "=");
	    			printf("pch=%s\n", pch);
	    			//pch+1="prog1"
	    			std::string desiredProg=pch+1;
	    			startProgram( desiredProg.substr(0, desiredProg.find("\n", 0)));

	    		}

	    		break;
	    	case TRANSFERSTRUCT:
	    		delAllProgs();
	    		//ESP_LOGI(TAG, "write msg: %s\n", command_mssg);
	    		//numb_progs_struct
	    		strncpy(numb_progs_struct_str, (char*)memmove(command_mssg, command_mssg+15, strlen(command_mssg)), strlen(command_mssg));
	    		//printf("numb_progs_struct_stre  %s\n", numb_progs_struct_str);
	    		numb_progs_struct=atoi(numb_progs_struct_str);
	    		printf("numb_progs_struct  %d\n", numb_progs_struct);


	    		len_program_bytes=recv(sock,program_bytes, sizeof(program_bytes)-1, 0);  //poluchavam imeto na programata
	    		ESP_LOGI(TAG, "number of symbols accepted%d\n", len_program_bytes );
	    		program_bytes[len_program_bytes]=0;
	    		ESP_LOGI(TAG, "string %s", program_bytes);
	    		ESP_LOGI(TAG, "strlen %d", strlen(program_bytes));
	    		ESP_LOGI(TAG, "strlen+1 %d", program_bytes[strlen(program_bytes)+1]); //number of steps in program
	    		numb_steps=program_bytes[strlen(program_bytes)+1]-'0';  //convetts char to int corresponding to numb of steps
	    		ESP_LOGI(TAG, "strlen+1 %d", numb_steps); //number of steps in program
	    		ESP_LOGI(TAG, "strlen+2 %d", program_bytes[strlen(program_bytes)+2]);
	    		ESP_LOGI(TAG, "strlen+3 %d", program_bytes[strlen(program_bytes)+3]);
	    		ESP_LOGI(TAG, "strlen+4 %d", program_bytes[strlen(program_bytes)+4]);
	    		ESP_LOGI(TAG, "strlen+5 %d", program_bytes[strlen(program_bytes)+5]);
	    		ESP_LOGI(TAG, "strlen+6 %d", program_bytes[strlen(program_bytes)+6]);
	    		ESP_LOGI(TAG, "strlen+7 %d", program_bytes[strlen(program_bytes)+7]);
	    		ESP_LOGI(TAG, "strlen+8 %d", program_bytes[strlen(program_bytes)+8]);

	    		//p_begin_prog=&program_bytes[strlen(program_bytes)+3];//from here the parameters for the first program for the first step start
	    		//ESP_LOGI(TAG, "pointer 0 %d", *(p_begin_prog+1));

	    		p_begin_prog=&program_bytes[0];

	    		for(int i=0; i<numb_progs_struct; i++)
	    		{
	    			//strlen(*(p_begin_prog+(numb_steps*34)));
	    			//tmpProgReadStruct.progName=p_begin_prog(strlen(*(p_begin_prog+(numb_steps*34))));
	    			//step_counter=0;

	    			tmpProgReadStruct.program.clear();

	    			tmpProgReadStruct.progName=p_begin_prog;
	    			printf("progname=%s\n", tmpProgReadStruct.progName.c_str());
	    			p_begin_prog=p_begin_prog+strlen(p_begin_prog)+1; ///tuka sme na nomer na stypki
	    			numb_steps=*(p_begin_prog)-'0'; ///moje da se izlychi kato chislo ne kato ascii
	    			printf("steps in prog %d\n" , numb_steps);
	    			p_begin_prog=p_begin_prog+2;
	    			//tmpParamReadStruct.
	    			for(int j=0; j<numb_steps; j++)
	    			{

	    				tmpParamReadStruct.kd=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.kd=%f pointer=%p\n", j, tmpParamReadStruct.kd, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpParamReadStruct.kid=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.kid=%f pointer=%p\n", j, tmpParamReadStruct.kid, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpParamReadStruct.kp=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.kp=%f pointer=%p\n", j, tmpParamReadStruct.kp, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpParamReadStruct.limit=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.limit=%f pointer=%p\n", j, tmpParamReadStruct.limit, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				//tmpParamReadStruct.rele1On=recv_struct[16];
	    				p_begin_prog=p_begin_prog+1;
	    				//tmpParamReadStruct.rele3On=recv_struct[17];
	    				p_begin_prog=p_begin_prog+1;
	    				tmpParamReadStruct.rele1On=0;
	    				tmpParamReadStruct.rele3On=0;
	    				tmpParamReadStruct.time=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.timeStart=%f pointer=%p\n", j, tmpParamReadStruct.time, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpParamReadStruct.timeStart=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.time=%f pointer=%p\n", j, tmpParamReadStruct.timeStart, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpParamReadStruct.timeStop=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.timestop=%f pointer=%p\n", j, tmpParamReadStruct.timeStop, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpParamReadStruct.zadanie=convertExpMantissToFloat(*(p_begin_prog), *(p_begin_prog+1), *(p_begin_prog+2), *(p_begin_prog+3));
	    				printf("%d tmpParamReadStruct.zadanie=%f pointer=%p\n", j, tmpParamReadStruct.zadanie, p_begin_prog);
	    				p_begin_prog=p_begin_prog+4;
	    				tmpProgReadStruct.program.push_back(tmpParamReadStruct);
	    			}

	    			//p_begin_prog=p_begin_prog+numb_steps*34; //dali trbqwa da e +1 ili +2
	    			addProg(tmpProgReadStruct);

	    		}

	    		break;
	    	default:
	    		break;

	    }
	} while (len > 0);

}





// length of the string
int len(std::string str)
{
    int length = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        length++;

    }
    return length;
}



void tcp_server_task(void *pvParameters)
{

    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    struct sockaddr_storage dest_addr;

    if (addr_family == AF_INET) {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    }
#ifdef CONFIG_EXAMPLE_IPV6
    else if (addr_family == AF_INET6) {
        struct sockaddr_in6 *dest_addr_ip6 = (struct sockaddr_in6 *)&dest_addr;
        bzero(&dest_addr_ip6->sin6_addr.un, sizeof(dest_addr_ip6->sin6_addr.un));
        dest_addr_ip6->sin6_family = AF_INET6;
        dest_addr_ip6->sin6_port = htons(PORT);
        ip_protocol = IPPROTO_IPV6;
    }
#endif

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
    // Note that by default IPV6 binds to both protocols, it is must be disabled
    // if both protocols used at the same time (used in CI)
    setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
#endif

    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(TAG, "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }

    while (1) {

        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
        // Convert ip address to string
        if (source_addr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
#ifdef CONFIG_EXAMPLE_IPV6
        else if (source_addr.ss_family == PF_INET6) {
            inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
#endif
        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

        comunication_commands(sock);

        //ESP_LOGI(TAG, "Received %d bytes: %s", sizeof(read_mssg), read_mssg);
        shutdown(sock, 0);
        close(sock);
    }

CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}





