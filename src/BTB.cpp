#include "BTB.h"
#define useless -1
// define BTB_Entry
BTB_Entry::BTB_Entry()
	:instr_addr(0), branch_addr(0),
	predictor(0)
{
	flag = 0;
	mis = false;
	notset = 0;
}



//////////////////////////////////////////////////////////////////////////
// define BTB
BTB::BTB()
	:BTB_CAMPACITY(16),needUpdateTable(BTBstate(0))
{
	Cur_Size = 0;
}
//
void BTB::addnew_notset(string name, int currInstr, int imm)
{
	Instruction temp;
	if(!temp.isBranch(name)) return;
	int i;
	int index = -1;
	for(i=0; i<BTB_CAMPACITY; i++)
	{
		if(BTB_table[i].flag == false && index == -1) index = i;
		if (BTB_table[i].instr_addr == currInstr) break;
	}
	if(i >= BTB_CAMPACITY)
	{
		
		BTB_table[index].instr_addr = currInstr;
		BTB_table[index].flag = true;
		BTB_table[index].predictor = false;
		BTB_table[index].notset = true;//not set
		if(name == "J")
		{
			BTB_table[index].branch_addr = imm;
			
		}
		else if(name == "BEQ")
		{
			BTB_table[index].branch_addr = currInstr + 4 + imm;
		}
		Cur_Size ++;
	}

}
// return the Predicted PC return predictor result if addr find in BTB, -1 if not, result_PC give the result PC
int BTB::makePrediction(const int cur_PC, int &result_PC)
{
	int i = 0;
	result_PC = cur_PC + 4; // default: the next PC will be PC + 4;
	for (i=0; i < BTB_CAMPACITY; i++)
	{
		if (BTB_table[i].instr_addr == cur_PC){
			if (BTB_table[i].predictor == 1)// if find
			{
				result_PC = BTB_table[i].branch_addr;
			}
			if (BTB_table[i].notset == false)
				return BTB_table[i].predictor;	
			else return -1;
		}
	}
	return -1; // if not find in BTB, return -1
}
void BTB::updateBTB(int addr, BTBstate state, int branch_address, bool& predictor)
{
	//if(state == update_BTB)
	//{
		int i;
		for(i = 0; i < BTB_CAMPACITY; i++)
		{
			if(BTB_table[i].instr_addr == addr) break;
		}
		assert(i < BTB_CAMPACITY);
		//BTB_table[i].predictor = !(BTB_table[i].predictor);
		BTB_table[i].predictor = predictor;
		BTB_table[i].notset = false;
		if (state == update_BTB ) BTB_table[i].mis = true;
	
	//else if(state == addnew_BTB)
	//{
	//	if(Cur_Size < BTB_CAMPACITY) // yes have position
	//	{	
	//		int i;
	//		for(i=0; i<BTB_CAMPACITY; i++)
	//		{
	//			if (BTB_table[i].flag == 0) break;
	//		}
	//		BTB_table[i].branch_addr = branch_address;
	//		BTB_table[i].predictor = predictor;
	//		BTB_table[i].instr_addr = addr;
	//		BTB_table[i].flag = true;
	//		Cur_Size ++;
	//	}
	//}
}
