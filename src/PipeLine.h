#ifndef PIPELINE_H
#define PIPELINE_H
#include "commonHeader.h"
// #include "InstructionQueue.h"
#include "RegisterStatus.h"
#include "ReorderBuffer.h"
#include "ReservationStations.h"
#include "BTB.h"
#include "computeInstruction.h"
#include "OPMap.h"
#include <fstream>
using namespace std;

extern int StartAddr;
class PileLine{

public:
	PileLine(vector<Instruction> InstrMemory, int StartAddr);
	void InstructionFetch(); // Fetch instruction from InstrMemory
	void DecodeandIssue();
	void Execution();
	void Write_Result();
	void Commit_(int &);
public:
	// After execution, if any insruction get the result, save it here see Figure 3.14
	int result; 
	int PC; // Always pointer to next PC after BTB
    
    
	
private:
    
	ReorderBuffer ROB_handle;
	ReservationStation RS_handle;
	Register_Status RegS_handle;
	BTB BTB_handle;
    queue<Instruction> IQ; // Instruction Queue handle

public:
	// where the IF stage fetch instruction
	vector<Instruction> InstrMemory;
	int MainMemory[100000];
	int order;
	int cycle;
//////////////////////////////////////////////////////////////////////////
//Zizhao
public:
	OprandMap map;
	int CurrentAddress; // Curent Address now fetched
	int lastAddress; //using CurrentAddress and lastAddress to control one stage in one cycle
	Instruction lastInstr; // used for wrong prediction and put instruction back to InstrMemeory;
	int PraserInstruction(Instruction currentInstr, int &rs, int &rt, int &rd, int &);
	int computeResult(const int);
	std::queue<int> AccAddressMap;
	void PrintOutResult(ofstream &);
	char * printState(State);
	void DoFlush(int );
	//void AccessMemory(string, int &);
    //zhouxiaofan
    int DataSegement[10];
    int num;//used for index of DataSegement
   // queue<Instruction> getIQ();

};
#endif