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

	char send_mssg[128];
	char command_mssg[128];

	char number_progs[10];
	char nameProg[128];
	char numStepsProg[128];
	char set_kp_value[128];
	char get_kp_value[128];
	char meas_temp[128];
	char set_zadanie_value[128];
	char get_zadanie_value[128];
	char get_zadanie_struct[128];
	char * pch;
	std::vector<ProgramStruct> tmpProg;
	int len;
	int len_prog_name;
	int len_recv_struct;
	int len_numb_steps_in_prog;
	uint64_t ch;
	int numb_progs_struct=0;
	char numb_progs_struct_str[128];

	int numb_steps_in_prog_struct=0;
	char numb_steps_in_prog_struct_str[128];

	char *ptr;

	char prog_name[128];
	char prog_name_steps[128];
	char prog_name_steps_split[4];
	char recv_struct[128];
	ProgramStruct tmpProgReadStruct;
	OperationParam tmpParamReadStruct;
	float tmp_struct_transf;
	float tmp_struct_transf2;
	float tmp_struct_transf3;
	float tmp_struct_transf4;
	do
	{
	    len=recv(sock,command_mssg, sizeof(command_mssg)-1, 0);
	    command_mssg[len]=0;
	    ESP_LOGI(TAG, "command msg: %s", command_mssg);
	    cmd = get_cmd_id( command_mssg );
	    OperationParam tmpParam;
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
	    	case TRANSFERSTRUCT:
	    		delAllProgs();
	    		//ESP_LOGI(TAG, "write msg: %s\n", command_mssg);
	    		//numb_progs_struct
	    		strncpy(numb_progs_struct_str, (char*)memmove(command_mssg, command_mssg+15, strlen(command_mssg)), strlen(command_mssg));
	    		//printf("numb_progs_struct_stre  %s\n", numb_progs_struct_str);
	    		numb_progs_struct=atoi(numb_progs_struct_str);
	    		printf("numb_progs_struct  %d\n", numb_progs_struct);
	    		for(int i=0; i<numb_progs_struct; i++)
	    		{
//	    			len_prog_name=recv(sock,prog_name, sizeof(prog_name)-1, 0);  //poluchavam imeto na programata
//	    			prog_name[len_prog_name]=0;
//	    			ESP_LOGI(TAG, "prog name: %s", prog_name);
//	    			tmpProgReadStruct.progName=prog_name;
//
//	    			//ets_delay_us(3000);
//	    			len_numb_steps_in_prog=recv(sock, numb_steps_in_prog_struct_str, sizeof(numb_steps_in_prog_struct_str)-1, 0);  //poluchawam stypkite w programata
//	    			//numb_steps_in_prog_struct_str[len_numb_steps_in_prog]=0;
//	    			numb_steps_in_prog_struct=atoi(numb_steps_in_prog_struct_str);
//	    			ESP_LOGI(TAG, "steps in prog: %d", numb_steps_in_prog_struct);


	    			len_prog_name=recv(sock,prog_name_steps, sizeof(prog_name_steps)-1, 0);  //poluchavam imeto na programata i stypkite w programata
	    			ptr=strtok(prog_name_steps, " ");
	    			strcpy(prog_name, ptr);
	    			ESP_LOGI(TAG, "prog_name: %s", prog_name);
	    			tmpProgReadStruct.progName=ptr;
	    			numb_steps_in_prog_struct=atoi(strtok(NULL, " "));
	    			ESP_LOGI(TAG, "steps: %d", numb_steps_in_prog_struct);
//


	    			for(int j=0; j<numb_steps_in_prog_struct; j++)  //for every step of the program
	    			{
	    				ESP_LOGI(TAG, "controll\n");
	    				len_recv_struct=recv(sock,recv_struct, sizeof(recv_struct)-1, 0);  //poluchavam structura
	    				ESP_LOGI(TAG, "len_recv_struct=%d\n", len_recv_struct);
	    				recv_struct[len_recv_struct]=0;
	    				tmpParamReadStruct.kd=convertExpMantissToFloat(recv_struct[0], recv_struct[1], recv_struct[2], recv_struct[3]);
	    				printf("%d tmpParamReadStruct.kd=%f\n", j, tmpParamReadStruct.kd);
	    				tmpParamReadStruct.kid=convertExpMantissToFloat(recv_struct[4], recv_struct[5], recv_struct[6], recv_struct[7]);
	    				printf("%d tmpParamReadStruct.kid=%f\n", j, tmpParamReadStruct.kid);
	    				tmpParamReadStruct.kp=convertExpMantissToFloat(recv_struct[8], recv_struct[9], recv_struct[10], recv_struct[11]);
	    				printf("%d tmpParamReadStruct.kp=%f\n", j, tmpParamReadStruct.kp);
	    				tmpParamReadStruct.limit=convertExpMantissToFloat(recv_struct[12], recv_struct[13], recv_struct[14], recv_struct[15]);
	    				printf("%d tmpParamReadStruct.limit=%f\n", j, tmpParamReadStruct.limit);
	    				//tmpParamReadStruct.rele1On=recv_struct[16];
	    				//tmpParamReadStruct.rele3On=recv_struct[17];
	    				tmpParamReadStruct.rele1On=0;
	    				tmpParamReadStruct.rele3On=0;
	    				tmpParamReadStruct.time=convertExpMantissToFloat(recv_struct[18], recv_struct[19], recv_struct[20], recv_struct[21]);
	    				printf("%d tmpParamReadStruct.time=%f\n", j, tmpParamReadStruct.time);
	    				tmpParamReadStruct.timeStart=convertExpMantissToFloat(recv_struct[22], recv_struct[23], recv_struct[24], recv_struct[25]);
	    				printf("%d tmpParamReadStruct.timeStart=%f\n", j, tmpParamReadStruct.timeStart);
	    				tmpParamReadStruct.timeStop=convertExpMantissToFloat(recv_struct[26], recv_struct[27], recv_struct[28], recv_struct[29]);
	    				printf("%d tmpParamReadStruct.timestop=%f\n", j, tmpParamReadStruct.timeStop);
	    				tmpParamReadStruct.zadanie=convertExpMantissToFloat(recv_struct[30], recv_struct[31], recv_struct[32], recv_struct[33]);
	    				printf("%d tmpParamReadStruct.zadanie=%f\n", j, tmpParamReadStruct.zadanie);

	    				tmpProgReadStruct.program.push_back(tmpParamReadStruct);
	    			}

	    			addProg(tmpProgReadStruct);


	    		}
//	    		setCurrentParam(tmpParam);
//	    		//setKp(atof(set_kp_value)); // da prpwerq dali naistina e float dali ima tochka i chisla
//	    		printf("setted zadanie  %s\n", set_zadanie_value);
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





