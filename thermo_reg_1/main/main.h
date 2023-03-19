/*
 * main.h
 *
 *  Created on: 19.03.2023 г.
 *      Author: dida
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_
#include <vector>
#include <string>

struct ProgramStruct
{
	std::string progName;
	std::vector<OperationParam>program;
};

OperationParam getCurrentParam (void);
void setCurrentParam (OperationParam a);
float getMeasTemp (void);
float measTemp (void);





#endif /* MAIN_MAIN_H_ */
