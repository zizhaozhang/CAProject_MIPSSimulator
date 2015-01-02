#ifndef REGISTER_STATUS_H
#define REGISTER_STATUS_H
#include "commonHeader.h"
#define useless -1
class Register_Status_Entry{

public:
	Register_Status_Entry();

public:
	ROB_Index Reorder;
	bool Busy;
	int Value;
	///////////////////////
	//Xiaofan Zhou
public:
	void clearReg_Status_Entry();
};
class Register_Status{

public:
//	Register_Status_Entry RegisterStat; 
	Register_Status_Entry RegS[registerTotalNum];

	int REGISTER_CAMPACITY;
public:
	Register_Status();

};


#endif