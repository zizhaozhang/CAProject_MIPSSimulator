#ifndef RESERVATION_STATIONS_H
#define RESERVATION_STATIONS_H
#include "commonHeader.h"


#define useless -1
class ReservationStation_Entry
{
	//enum OP{ // = 0 when no Opcode in the Entry
	//	LOAD = 1,
	//    STORE,
	//	SUB,
	//	ADD,
	//	MUL,
	//	DIV,
	//	JUMP,
	//	NOP
	//}; // enum type for Op in Reservation stations
	//// According to different Op, we achieve different function to compute
public:
	ReservationStation_Entry();

public:
	// compute 


public:
	bool Busy;
	string Op; // the instruction name
	// Realy value in two operand rs and rt
	Operand Vj;
	Operand Vk;

	// Qj,Qk: The ROB index will produce the corresponding operand
	// Zero indicates the source operand is available or unnecessary
	ROB_Index Qj;
	ROB_Index Qk;
	ROB_Index Dest;
	// A: Hold memory address for load or store. Immediate is stored here.
	Operand A;
	int result;
	bool flag; // As the requirements said, we need a flag to see if this entry is in use;
	int cycle;
	int address;
	int order;
//////////////////////////////////////////////////////////////////////////
// Zizhao Zhang


//////////////////////////////////////////////////////////////////////////
//Xiaofan Zhou
	void clearRS_Entry();
	
};

class ReservationStation
{

public:
	ReservationStation_Entry RS[11]; // 10 RS I think How large it is?
public:
	ReservationStation();

	// Find emtry Entry, return -1 if full
	// return index of the slots if not full
	int findEmptyEntry();
	bool clearEntry(int ind);
	// According to different instructions, if Vj and Vk or A is ready, compute the result.
	int computeResult(int ind);

public:
	int RS_CAMPACITY; // 10 RS
	int RS_SIZE;


};



#endif