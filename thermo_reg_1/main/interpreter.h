/*
 * interpreter.h
 *
 *  Created on: 26.02.2023 г.
 *      Author: dida
 */

#ifndef MAIN_INTERPRETER_H_
#define MAIN_INTERPRETER_H_


enum {
    GETKP,
    SETKP,
	GETMEASTEMP,
	GETZADANIE,
	SETZADANIE

};



char get_cmd_id(char *);
char const *get_cmd_name(char);


#endif /* MAIN_INTERPRETER_H_ */
