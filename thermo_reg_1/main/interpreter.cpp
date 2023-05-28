/*
 * interpreter.c
 *
 *  Created on: 26.02.2023 г.
 *      Author: dida
 */




#include <string.h>
#include "interpreter.h"

struct cmd_struct
{
	signed char id;
	char const *name;
} cmd_tbl[] =
{
{ GETKP, "kp?\n" },
{ SETKP, "setKP" },
{ GETMEASTEMP, "mTemp?" },
{ GETZADANIE, "zadanie?" },
{ SETZADANIE, "setZadanie" },
{ TRANSFERSTRUCT, "transferStruct=" },
{ NUMBPROG, "countProgs?" },
{ PROGNAMES, "nameProgs=" },
{ PROGSTEPSNUMB, "nameStepProgs=" },
{ PRINTVECT, "vect?" },
{ GETPROGRAMS, "programs=" },
{ GETOPPARAM, "opparam" },
{ -1, NULL } };

/**
 *
 * @param name
 * @return
 */
char get_cmd_id(char *name)
{
	unsigned char i = 0;

	if (name[0] == 0)
		return -2;

	for (i = 0; cmd_tbl[i].id != -1; i++)
		if (!strncmp(name, cmd_tbl[i].name,strlen(cmd_tbl[i].name)))
			return cmd_tbl[i].id;

	return -1;
}

/**
 *
 * @param id
 * @return command name or NULL if not found
 */
char const * get_cmd_name(char id)
{
	unsigned char i;

	for (i = 0; cmd_tbl[i].id != -1; i++)
		if (id == cmd_tbl[i].id)
			return cmd_tbl[i].name;

	return NULL;
}
