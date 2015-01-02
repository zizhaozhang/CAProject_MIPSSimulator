#ifndef REORDERBUFFER_H
#define REORDERBUFFER_H
#include "commonHeader.h"
#include <vector>
#include <queue>
#include "Instructions.h"
/* Please refer to Figure 3.12
   ReorderBuffer_Entry includes all temp variables of in Recorder buffer

*/
#define useless - 1
enum State{ // = 0 when empty
	//Five stages
	IF = 1,
	Issue,
	Execute,
	WriteResult,
	Commit 
};
class ReorderBuffer_Entry {

public:

public:

	Operand Value; 
	RegisterID Dest; // Desitination Register accordint to RegisterStatus
	State state; // state of ROB
	string instruction; // not sure using just name is fine?
	bool Busy;
	unsigned int entry_ID; //ID for ROB
	bool Ready;
public:
	bool flag; // As the requirements said, we need a flag to see if this entry is in use;
	int order;
	int cycle;
public:
	ReorderBuffer_Entry();

//////////////////////////////////////////////////////////////////////////
//Zizhao Zhang
	int address;

//////////////////////////////////////////////////////////////////////////
//Xiaofan Zhou
void clearROB_Entry();



};
	 
class ReorderBuffer{
public:
	unsigned int ROB_SIZE; // Current size of ROB ROB_SIZE <= ROB_CAPACITY if ROB == ROB_CAPACITY means FULL
public:
	const unsigned int ROB_CAPACITY; // 8 according to the requirements

public:
	ReorderBuffer();

	// Find emtry Entry, return -1 if full
	// return index of the slots if not full
	int findEmptyEntry();
	// clear NO.ind entry if committed
	bool clearEntry(int ind);
	// 
	//Search 
public:
	bool isFull(); // if ROB_SIZE == ROB_CAPACITY return true; 

public:
	ReorderBuffer_Entry ROB[7]; // ROB has ROB_SIZE=6 entries. 
									// use quene cause ROB is First issue first Commit
};



#endif 