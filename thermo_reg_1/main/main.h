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
void delProg(std::string programme_name_toberm);
void delAllProgs(void);
void addProg(ProgramStruct tobeadd);
std::vector<ProgramStruct> getProgram (void);
void debugProg(void);
void startProgram(std::string nameDesiredPrg);




#endif /* MAIN_MAIN_H_ */
