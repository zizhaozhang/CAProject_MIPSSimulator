#include "PipeLine.h"
// extern enum InsType;
PileLine::PileLine(vector<Instruction> IM, int StartAddr)
	:PC(StartAddr), lastInstr(),AccAddressMap()
{
	//this->InstrMemory = IM;
    InstrMemory = IM;
	CurrentAddress = PC;
	lastAddress = PC;	
	order = 0;
	cycle = 1;
    num = 0;
    for (int i = 0; i < 10; i++)
    {
        DataSegement[i] = 0;
    }
}


//queue<Instruction> PileLine::getIQ()
//{
//    return IQ;
//}



//////////////////////////////////////////////////////////////////////////
// Zizhao Zhang
int PileLine::PraserInstruction(Instruction instr,int &rs, int &rt, int &rd, int &immediate)
{
	instr.ParserInstruction(rs, rt, rd, immediate);
	return 1; // how to handle return
}
void PileLine::DecodeandIssue()
{    
	// I do not think I need to control the instruction do one stage in one cycle in issue stage. Becuse IQ has more		   instruction than 
	//To maintain that one instrution can only do one stage at one cycle
	/*if(InstrMemory.size() != 0 && CurrentAddress == lastAddress)
		return ; */

	// get instruction from IQ
	Instruction currentInstr;
	
	if (!IQ.empty() && IQ.size() != 1)
	{
		lastInstr = currentInstr;
		currentInstr = IQ.front();
		
		////IQ.pop();//
	}
	else
	{
		return ;
	}
	if(currentInstr.name == "0") 
	{
		IQ.pop();
		AccAddressMap.pop();
		return;
	}
	int rs = -1, rt = -1, rd = -1, immediate = -1;
	// get rs, rt, rd according to different instruction
	PraserInstruction(currentInstr, rs, rt, rd, immediate);
   
	// allocate ROB and RS
	int r = RS_handle.findEmptyEntry();// find a RS slot and set flag
	int b = ROB_handle.findEmptyEntry(); // find a ROB slot set flag
	// issue NOP and Break
	if ((currentInstr.name == "NOP" || currentInstr.name == "BREAK") && b != -1) // find a slot in ROB
	{
		ROB_handle.ROB[b].instruction = currentInstr.name;
		ROB_handle.ROB[b].Busy = yes;
		ROB_handle.ROB[b].flag = true;
		ROB_handle.ROB[b].order = ++order;
		ROB_handle.ROB[b].instruction = currentInstr.instr;
		ROB_handle.ROB[b].state = State(Issue);
		ROB_handle.ROB[b].address = AccAddressMap.front();
		ROB_handle.ROB[b].cycle = cycle;
		ROB_handle.ROB[b].Ready = yes;

		IQ.pop(); 
		AccAddressMap.pop();
		return ;
		//RS_handle.RS[r].Op = currentInstr.name;
		//RS_handle.RS[r].Busy = yes;
		//RS_handle.RS[r].Dest = b;
	}

	if (r == -1 || b == -1) // stall 
	{
		cout<<"RS or ROB is full, can not issue "<< currentInstr.name<< " now. STALL at cycle"<<cycle<<endl;
		RS_handle.RS[r].flag = false;
		ROB_handle.ROB[b].flag = false;
		return ; //???????
	}
	// yes could issue and pop the instruction in IQ;
	IQ.pop(); 
	// handle NOP and BREAK wait to be issued;
	
	//set flag
	ROB_handle.ROB[b].flag = true;
	RS_handle.RS[r].flag = true;
	ROB_handle.ROB[b].order = ++order;
	RS_handle.RS[r].order = ++order;

	// find all instructions, allocate the name to ROB and RS, state to ROB
	RS_handle.RS[r].Op = currentInstr.name; // allocate the instr name
	ROB_handle.ROB[b].instruction = currentInstr.instr;

	ROB_handle.ROB[b].state = State(Issue);
	// update this instruction address
	RS_handle.RS[r].address = AccAddressMap.front(); // PC has been +4
	ROB_handle.ROB[b].address = AccAddressMap.front();
	AccAddressMap.pop();

	// update this instruction cycle.
	RS_handle.RS[r].cycle = cycle;
	ROB_handle.ROB[b].cycle = cycle;

	
	//update rs for all instruction
	int h;
	if(rs != useless){ // this operation has rs field
		if(RegS_handle.RegS[rs].Busy)
		{
			h = RegS_handle.RegS[rs].Reorder;
			if (ROB_handle.ROB[h].Ready)
			{
				RS_handle.RS[r].Vj = ROB_handle.ROB[h].Value;
				RS_handle.RS[r].Qj = 0;
			}
			else
			{
				RS_handle.RS[r].Qj = h;
			}
		}
		else
		{
			RS_handle.RS[r].Vj = RegS_handle.RegS[rs].Value;
			RS_handle.RS[r].Qj = 0;
		}
		RS_handle.RS[r].Busy = yes;
		RS_handle.RS[r].Dest = b;
		//RS_handle.RS[r].Op = currentInstr.name; // allocate the instr name
		//ROB_handle.ROB[b].instruction = currentInstr.name;
		if(rd != -1) ROB_handle.ROB[b].Dest = rd; // handle rd soon
		ROB_handle.ROB[b].Ready = no;

	}
	
	if(rt != useless)
	{
		if(RegS_handle.RegS[rt].Busy)
		{
			h = RegS_handle.RegS[rt].Reorder;
			if (ROB_handle.ROB[h].Ready)
			{
				RS_handle.RS[r].Vk = ROB_handle.ROB[h].Value;
				RS_handle.RS[r].Qk = 0;
			}
			else
			{
				RS_handle.RS[r].Qk = h;
			}

		}
		else // Resigster rt not busy, can get the value
		{
			RS_handle.RS[r].Vk = RegS_handle.RegS[rt].Value;
			RS_handle.RS[r].Qk = 0;
		}
	}
	if (immediate != useless && currentInstr.name != "SW")
	{
		rd = rt; // if immediate is not a zero, acutally rt is destination
		// I am not sure if I need to clear Vk and Qk which not exist 
	} 
	if(rd != useless) // for FP operations rd
	{
		RegS_handle.RegS[rd].Reorder = b; // Register do not worry WAW, assign to the last operation which use b as destination
		RegS_handle.RegS[rd].Busy = yes;
		
		if(currentInstr.name != "SW") //SW does not allocat Dest. 
			ROB_handle.ROB[b].Dest = rd;
	}
	if(immediate != useless) // include store load jump and FP which have immediate field
	{
		RS_handle.RS[r].A = immediate;
	}


	if(currentInstr.isLoad())
	{
		RegS_handle.RegS[rt].Reorder = b;
		RegS_handle.RegS[rt].Busy = yes;
		ROB_handle.ROB[b].Dest = rt;
	}
	else if(currentInstr.isJump())
	{
		RS_handle.RS[r].Qj =0;
		RS_handle.RS[r].Qk = 0;
		
			RS_handle.RS[r].A = immediate;
		RS_handle.RS[r].Busy = yes;
		ROB_handle.ROB[b].Ready = no;	
		RS_handle.RS[r].Dest = b;
	}
	//// handle branch
	//if(currentInstr.isBranch())
	//{
	//	assert(currentInstr.predictor != -2);
	//	int isTaken = currentInstr.isTakenBrach(PC);
	//	if (isTaken){
	//		if(currentInstr.predictor == -1) // no find in BTB and predictor true
	//		{BTB_handle.needUpdateTable = r;} // r is the index of RS entry, means add new 
	//		else if(currentInstr.predictor == 0) // predict wrongly
	//		{
	//			BTB_handle.needUpdateTable = 100 + r; // menas to update, distinguish with addnew, I add 100 to the index
	//		}
	//		//else if(predictor == 1) // predict correctly
	//		//{

	//		//}
	//	}
	//	else // not taken
	//	{	
	//		if(currentInstr.predictor == 1) // predict wrongly
	//		{
	//			BTB_handle.needUpdateTable = 100 + r; // means update  
	//		}
	//	}
	//	// if no taken and not find in BTB, regard it as normal instruction
	//}// isBranch

}

void PileLine::InstructionFetch()
{
	Instruction tempInstr;
	//int prediction;
	if (!InstrMemory.empty())
	{
		lastAddress = CurrentAddress;
		CurrentAddress = PC;
        
		tempInstr = InstrMemory.at((CurrentAddress - StartAddr) / 4);
	
		// handle BTB
		int prediction = BTB_handle.makePrediction(CurrentAddress, PC);// return predicted PC
		tempInstr.predictor = prediction;
		BTB_handle.addnew_notset(tempInstr.name,CurrentAddress, tempInstr.immediate);
		IQ.push(tempInstr);
		AccAddressMap.push(CurrentAddress);
		
		
	}	
	
	//CurrentAddress = PC;
};
int PileLine::computeResult(int entry)
{
	computeInstruction *comH = new computeInstruction();
	int result;

	result = (comH->*comH->FunctionMapper[RS_handle.RS[entry].Op])(RS_handle.RS[entry].Vj, RS_handle.RS[entry].Vk, RS_handle.RS[entry].A);

	return result;
}
void PileLine::Execution()
{
	Instruction tempInstr_handle; // 
	// parall Execute;
	for(int i = Entry_Start; i <= RS_handle.RS_CAMPACITY; i++)
	{
		// for SW handle it separately
		if((RS_handle.RS[i].Op == "SW") && RS_handle.RS[i].Qj == 0 && RS_handle.RS[i].cycle && RS_handle.RS[i].cycle < cycle)
		{
			//if (RS_handle.RS[i].result == useless) //doing execution
			//{
				RS_handle.RS[i].result = computeResult(i);
				int h = RS_handle.RS[i].Dest;		
				ROB_handle.ROB[h].Dest = RS_handle.RS[i].result; // the Memeory address
				// update the cycle of RS
				RS_handle.RS[i].cycle = cycle;
				ROB_handle.ROB[RS_handle.RS[i].Dest].cycle = cycle;

				// update state in ROB
				int b = RS_handle.RS[i].Dest;
				ROB_handle.ROB[b].state = (State)Execute;
			//}
			   // RS_handle.RS[i].flag = false;// trick
			if(RS_handle.RS[i].Qk == 0) {
				int h = RS_handle.RS[i].Dest;	
				ROB_handle.ROB[h].Value = RS_handle.RS[i].Vk;
				ROB_handle.ROB[h].Ready = yes;
				RS_handle.RS[i].clearRS_Entry();
				
			}
			
		}
		// for other instructions
		if(RS_handle.RS[i].flag == 1 && RS_handle.RS[i].Qk == 0 && RS_handle.RS[i].Qj == 0
			&& RS_handle.RS[i].cycle < cycle && RS_handle.RS[i].result == useless)
		{ // if current RS is ready to execute and this instruction does not do other stage
		  // and this instruction has not been executed.

			// compte results
			// for load and store achieve address generation
			// for Branch and Jump, give the prediction result
			RS_handle.RS[i].result = computeResult(i);
			string currentOP = RS_handle.RS[i].Op;
			// handle Branch update Branch Table
			
			if(tempInstr_handle.isBranch(currentOP))
			{
  				//bool predictor = (bool)RS_handle.RS[i].result;
				//assert(predictor != -2);
				int tempPC = PC;
				int temp = 0;
				int PreRes = BTB_handle.makePrediction(RS_handle.RS[i].address, temp);;
				bool isTaken = Instruction::isTakenBrach(currentOP, tempPC, RS_handle.RS[i].Vj, RS_handle.RS[i].Vk, RS_handle.RS[i].A, RS_handle.RS[i].address + 4);
				if (isTaken)
				{ // if really a taken branch
					
					
					if(PreRes == -1 || PreRes == -2) // no find in BTB and predictor true
					{
						PC = tempPC;
						BTB_handle.needUpdateTable = i; // means to add new
					} // r is the index of RS entry, means add new 
					else if(PreRes == 0) // predict wrongly set PC to next instruction
					{
						//PC = RS_handle.RS[i].address + 4;
						PC = tempPC;
						BTB_handle.needUpdateTable = 100 + i; // menas to update, distinguish with addnew, I add 100 to the index
					}
				}
				else // not taken
				{	
					if(PreRes == 1) // predict wrongly it is non taken
					{
						PC = RS_handle.RS[i].address + 4;
						BTB_handle.needUpdateTable = 100 + i; // means update  
					}
					// add to BTB even this branch is not taken and not exist in BTB
					else if (PreRes == -1)
					{
						BTB_handle.needUpdateTable = i; // means update  
					}
				}
				// either case, if predict true and not find in BTB do flush
				if((isTaken == true && PreRes == false) || (isTaken == false && PreRes == true) || (isTaken == true && PreRes == -1))
				{

					DoFlush(i);
					//continue;
				}

				if(BTB_handle.needUpdateTable == i) // need add new
				{
					if (RS_handle.RS[i].Op == "BEQ")
						BTB_handle.updateBTB(RS_handle.RS[i].address, addnew_BTB,RS_handle.RS[i].address+ 4 + RS_handle.RS[i].A, isTaken);
					else
						BTB_handle.updateBTB(RS_handle.RS[i].address, addnew_BTB, RS_handle.RS[i].A, isTaken);
					BTB_handle.needUpdateTable = -100;
				}
				else if (BTB_handle.needUpdateTable == 100 + i) // meaning wrong predictor need update
				{
					BTB_handle.updateBTB(RS_handle.RS[i].address, update_BTB, RS_handle.RS[i].A, isTaken);
					BTB_handle.needUpdateTable = -100;

					
				}
				
			}

			RS_handle.RS[i].cycle = cycle;
			ROB_handle.ROB[RS_handle.RS[i].Dest].cycle = cycle;

			// update state in ROB
			int b = RS_handle.RS[i].Dest;
			ROB_handle.ROB[b].state = (State)Execute;

			// if Load or branch finish the instruction at execute stage 
			// and clear the RS entry
			if(tempInstr_handle.ClearSpecialInstrRS(currentOP))
			{
				RS_handle.RS[i].clearRS_Entry();
				ROB_handle.ROB[b].Ready = yes;
			}
					

		}
	}
}

void PileLine::PrintOutResult(ofstream &outfile)
{
    
	cout<<"Cycle <"<<cycle<<">"<<endl;
    outfile<<"Cycle <"<<cycle<<">:"<<endl;
    //print out IQ
   // cout<<"IQ:"<<endl;
    outfile<<"IQ:"<<endl;
    queue <Instruction> TempIQ;
    TempIQ = IQ;
    while (!TempIQ.empty()) {
        Instruction currentInstr;
        currentInstr = TempIQ.front();
        TempIQ.pop();
        if (currentInstr.instr!="0")
           // cout<<"["<<currentInstr.instr<<"]" <<endl;
        	if (currentInstr.instr == "NOP")
            	outfile<<"["<<currentInstr.instr<<"]" <<endl;
        	else
            	outfile<<"["<<currentInstr.instr<<"] " <<endl;
    }
    
    
	//print out Reservation stations
  cout<<"Busy"<<"\t"<<"Op"<<"\t"<<"Vj"<<"\t"<<"Vk"<<"\t"<<"Qj"<<"\t"<<"Qk"<<"\t"<<"Dest"<<"\t"<<"A"<<endl;
	for(int i=Entry_Start; i <= RS_handle.RS_CAMPACITY; i++)
	{
		//if(RS_handle.RS[i].flag!= false){
			cout<<RS_handle.RS[i].Busy<<"\t"<<RS_handle.RS[i].Op<<"\t"<<RS_handle.RS[i].Vj<<"\t"<<RS_handle.RS[i].Vk<<"\t"<<RS_handle.RS[i].Qj<<"\t"<<RS_handle.RS[i].Qk<<"\t"<<RS_handle.RS[i].Dest<<"\t"<<RS_handle.RS[i].A<<endl;
		//}
	}
	cout<<endl;
 
    
    //cout<<"RS:"<<endl;
    outfile<<"RS:"<<endl;
    ReservationStation_Entry tempRS_Entry[1000];
    for(int i=Entry_Start; i <= RS_handle.RS_CAMPACITY; i++)
    {
        if(RS_handle.RS[i].flag != false)
        {

				int j = RS_handle.RS[i].order;
				 tempRS_Entry[j]=RS_handle.RS[i];
        }
    }
    
    int i = 0;
    while (i<1000)
    {
		if(tempRS_Entry[i].Op == "SW" && ROB_handle.ROB[tempRS_Entry[i].Dest].state ==  Execute)
		{

		}
	
        else if (tempRS_Entry[i].flag && tempRS_Entry[i].Dest != -1)
        {
			
            if(ROB_handle.ROB[tempRS_Entry[i].Dest].instruction!= "")
                //cout<<"["<<ROB_handle.ROB[tempRS_Entry[i].Dest].instruction<<"]"<<endl;
                // if (ROB_handle.ROB[tempRS_Entry[i].Dest].instruction == "NOP")
                // 	outfile<<"[" +  ROB_handle.ROB[tempRS_Entry[i].Dest].instruction + "]"<<endl;
               	// else
                	outfile<<"[" +  ROB_handle.ROB[tempRS_Entry[i].Dest].instruction + "] "<<endl;
        }
        i++;
    }
    
	//print out Reorder buffer
	cout<<"Ready"<<"\t"<<"Instruction"<<"\t\t"<<"State"<<"\t"<<"Dest"<<"\t"<<"Value"<<endl;
	for (int i = Entry_Start; i <= ROB_handle.ROB_CAPACITY;i++)
	{

			cout<<ROB_handle.ROB[i].Busy<<"\t"<<ROB_handle.ROB[i].instruction.c_str()<<"\t\t"<<printState(ROB_handle.ROB[i].state)<<"\t"<<ROB_handle.ROB[i].Dest<<"\t"<<ROB_handle.ROB[i].Value<<endl;
		
	}
	cout<<endl;
 
    //cout<<"ROB:"<<endl;
    outfile<<"ROB:"<<endl;
    ReorderBuffer_Entry tempROB_Entry[1000];
    for (int i = Entry_Start; i <= ROB_handle.ROB_CAPACITY;i++)
    {
        if (ROB_handle.ROB[i].flag) {
            int j = ROB_handle.ROB[i].order;
            tempROB_Entry[j] = ROB_handle.ROB[i];
        }
    }
    i = 0;
    while (i<1000)
    {
        if (tempROB_Entry[i].flag)
        {
            if(tempROB_Entry[i].instruction!="")
               // cout<<"["<<tempROB_Entry[i].instruction<<"]"<<endl;
            	if(tempROB_Entry[i].instruction == "NOP")
               		 outfile<<"[" + tempROB_Entry[i].instruction  + "]"<<endl;
               	else
               		outfile<<"[" + tempROB_Entry[i].instruction  + "] "<<endl;
        }
        i++;
    }
    
    // print out BTB
    //cout<<"BTB:"<<endl;
    outfile<<"BTB:"<<endl;
    int j =1;
    for (i = 0; i<16; i++)
    {
       // if (BTB_handle.BTB_table[i].flag)
        if(BTB_handle.BTB_table[i].instr_addr!=0)
        {
            //cout<<"["<<"Entry "<<j<<"]:"<<"<"<<BTB_handle.BTB_table[i].instr_addr<<","<<BTB_handle.BTB_table[i].branch_addr<<","<<BTB_handle.BTB_table[i].predictor<<">"<<endl;
            if(BTB_handle.BTB_table[i].notset==yes)
            {
                outfile<<"["<<"Entry "<<j<<"]:"<<"<"<<BTB_handle.BTB_table[i].instr_addr<<","<<BTB_handle.BTB_table[i].branch_addr<<","<<"NotSet"<<">"<<endl;
            }
            else
            {
                outfile<<"["<<"Entry "<<j<<"]:"<<"<"<<BTB_handle.BTB_table[i].instr_addr<<","<<BTB_handle.BTB_table[i].branch_addr<<","<<BTB_handle.BTB_table[i].predictor<<">"<<endl;
            }
            j++;
        }
    }
    
    
	// print out register status
    //cout<<"Registers:"<<endl;
    for (int i=0; i< RegS_handle.REGISTER_CAMPACITY; i++)
    {
        if(RegS_handle.RegS[i].Reorder != useless)
            cout<< "F" <<i<<"\t";
    }
    cout<<endl;
    for (int i=0; i< RegS_handle.REGISTER_CAMPACITY; i++)
    {
        if(RegS_handle.RegS[i].Reorder != useless)
            cout<<RegS_handle.RegS[i].Reorder<<"\t";
    }
    cout<<endl;
    for (int i=0; i< RegS_handle.REGISTER_CAMPACITY; i++)
    {
        if(RegS_handle.RegS[i].Busy != no)
            cout<<RegS_handle.RegS[i].Busy<<"\t";
    }
    cout<<endl;
    
    outfile<<"Registers:"<<endl;
  //  cout<<"R00:";
    outfile<<"R00:";
    for (i = 0; i< 8; i++)
    {
        //cout<<"\t"<<RegS_handle.RegS[i].Value;
        outfile<<"\t"<<RegS_handle.RegS[i].Value;
    }
    //cout<<endl;
    outfile<<endl;
    //cout<<"R08:";
    outfile<<"R08:";
	for (; i< 16; i++)
	{
        //cout<<"\t"<<RegS_handle.RegS[i].Value;
        outfile<<"\t"<<RegS_handle.RegS[i].Value;
	}
	//cout<<endl;
    outfile<<endl;
   // cout<<"R16:";
    outfile<<"R16:";
	for (; i< 24; i++)
	{
        //cout<<"\t"<<RegS_handle.RegS[i].Value;
        outfile<<"\t"<<RegS_handle.RegS[i].Value;
	}
	//cout<<endl;
    outfile<<endl;
    //cout<<"R24:";
    outfile<<"R24:";
	for (; i< 32; i++)
	{
        //cout<<"\t"<<RegS_handle.RegS[i].Value;
        outfile<<"\t"<<RegS_handle.RegS[i].Value;
	}
	//cout<<endl;
    outfile<<endl;
    //cout<<"Data Segement:"<<endl;
    outfile<<"Data Segment:"<<endl;
    //cout<<"716:";
    outfile<<"716:";
    for (i=0; i <10; i ++)
    {
       // cout<<"\t"<<DataSegement[i];
        outfile<<"\t"<<DataSegement[i];
    }
    //cout<<endl;
    outfile<<endl;
   
   
}

char* PileLine::printState(State state)
{
	if(state == IF) return "IF";
	else if(state == Issue) return "IS";
	else if(state == Execute) return "Ex";
	else if(state == (State)WriteResult) return "WB";
	else if(state == Commit) return "Com";
	else return " ";
}

void PileLine::DoFlush(int index)
{
	int temp = 0;
	//if(BTB_handle.makePrediction(RS_handle.RS[index].address, temp) != -1) return;
	int maxOrder = -100; 
	int order = RS_handle.RS[index].order;
	for(int i=Entry_Start; i <= RS_handle.RS_CAMPACITY; i++)
	{
		if((RS_handle.RS[i].order > order) && (RS_handle.RS[i].flag != false)) // instruction issued after order
		{
			/*int dest = RS_handle.RS[i].Dest;
			if (dest == -1) continue;*/
			RS_handle.RS[i].clearRS_Entry();
			//ROB_handle.ROB[dest].clearROB_Entry();
			maxOrder = (maxOrder > RS_handle.RS[i].order)? maxOrder:RS_handle.RS[i].order;
		}
	}
	for(int i =Entry_Start; i <= ROB_handle.ROB_CAPACITY; i++)
	{
		if((ROB_handle.ROB[i].order > order) && (ROB_handle.ROB[i].flag != false)) // instruction issued after order
		{
			if(ROB_handle.ROB[i].Dest != -1) 
				RegS_handle.RegS[ROB_handle.ROB[i].Dest].clearReg_Status_Entry();
			ROB_handle.ROB[i].clearROB_Entry();
			
			maxOrder = (maxOrder > ROB_handle.ROB[i].order)? maxOrder:ROB_handle.ROB[i].order;
		}
	}
	////int numFlush = maxOrder - order; //numFlush save how many instruction need to flush in IQ, exclude the instruction has been issued just now.

	//flush IQ and address map
	

	/*for(int i=0; i < numFlush; i++)
	{*/
	while(!IQ.empty())
	{
		IQ.pop();
		AccAddressMap.pop();
	}
	//}

}

//////////////////////////////////////////////////////////////////////////
//Xiaofan Zhou
void PileLine::Write_Result()
{

	//first, check each entry of reservation station
	for(int i = Entry_Start; i <= RS_handle.RS_CAMPACITY; i++)
	{
		if (RS_handle.RS[i].cycle<cycle)
		{
			if(RS_handle.RS[i].Op == "SW")
			{
				//int h = RS_handle.RS[i].Dest;
				//ROB_handle.ROB[h].Value = RS_handle.RS[i].Vk;
				//ROB_handle.ROB[h].Dest = RS_handle.RS[i].result; // the Memeory address
				//ROB_handle.ROB[h].Ready = yes;
			}
			else if (RS_handle.RS[i].Op=="NOP"||RS_handle.RS[i].Op=="BREAK"||RS_handle.RS[i].Op=="J"||RS_handle.RS[i].Op=="BEQ"||RS_handle.RS[i].Op=="BNE"||RS_handle.RS[i].Op=="BGEZ"||RS_handle.RS[i].Op=="BGTZ"||RS_handle.RS[i].Op=="BLEZ"||RS_handle.RS[i].Op=="BLTZ")
			{
				int h = RS_handle.RS[i].Dest;
				ROB_handle.ROB[h].Ready = yes;
				ROB_handle.ROB[h].state = WriteResult;
			}
			else if(RS_handle.RS[i].Op!="LW" && RS_handle.RS[i].flag==1 && RS_handle.RS[i].Qj==0 && RS_handle.RS[i].Qk==0)
			{
				//current instruction i go through WriteResult stage, update its RS.cycle
				RS_handle.RS[i].cycle = cycle;
				int b = RS_handle.RS[i].Dest;
				ROB_handle.ROB[b].cycle= RS_handle.RS[i].cycle;//the same instr cannot go through WriteResult and Commit at the same cycle
				RS_handle.RS[i].Busy = no;
				ROB_handle.ROB[b].state = WriteResult;
				for (int j = Entry_Start; j <= RS_handle.RS_CAMPACITY; j++)
				{
					if (RS_handle.RS[j].Qj==b)
					{
						RS_handle.RS[j].Vj = RS_handle.RS[i].result;
						RS_handle.RS[j].Qj = 0;
						RS_handle.RS[j].cycle = cycle;
					}
				}
				for (int k = Entry_Start; k <= RS_handle.RS_CAMPACITY; k++)
				{
					if (RS_handle.RS[k].Qk==b)
					{
						RS_handle.RS[k].Vk = RS_handle.RS[i].result;
						RS_handle.RS[k].Qk = 0;
						RS_handle.RS[k].cycle = cycle;
					}
				}
				ROB_handle.ROB[b].Value = RS_handle.RS[i].result;
				ROB_handle.ROB[b].Ready = yes;
				RS_handle.RS[i].clearRS_Entry();
				//break;
			}
			else if(RS_handle.RS[i].Op=="LW" && RS_handle.RS[i].flag==1 && RS_handle.RS[i].Qj==0)//memory address for LW is computed out in execution stage,i.e. , RS.Vj is got
			{
				//current instruction i go through WriteResult stage, update its RS.cycle
				//RS_handle.RS[i].cycle = cycle;
				int b = RS_handle.RS[i].Dest;
				ROB_handle.ROB[b].cycle= RS_handle.RS[i].cycle;//the same instr cannot go through WriteResult and Commit at the same cycle
				//How to show the two stages of load
				//load stage 1, get the mem address
				if (cycle - RS_handle.RS[i].cycle == 1)
				{
					RS_handle.RS[i].A = RS_handle.RS[i].result;
					ROB_handle.ROB[b].state = WriteResult;
				}
				else //load stage 2. get the value from mem
				{
					int MemAddr = RS_handle.RS[i].A;
					int b = RS_handle.RS[i].Dest;
					ROB_handle.ROB[b].Value = MainMemory[MemAddr];
					ROB_handle.ROB[b].Ready = yes;
					RS_handle.RS[i].result = MainMemory[MemAddr];
					// update current cycle
					RS_handle.RS[i].cycle = cycle;
					ROB_handle.ROB[b].cycle = cycle;
					ROB_handle.ROB[b].state = WriteResult;
					for (int j = Entry_Start; j <= RS_handle.RS_CAMPACITY; j++)
					{
						if (RS_handle.RS[j].Qj==b)
						{
							RS_handle.RS[j].Vj = RS_handle.RS[i].result;
							RS_handle.RS[j].Qj = 0;
							RS_handle.RS[j].cycle = cycle;
						}
					}
					for (int k = Entry_Start; k <= RS_handle.RS_CAMPACITY; k++)
					{
						if (RS_handle.RS[k].Qk==b)
						{
							RS_handle.RS[k].Vk = RS_handle.RS[i].result;
							RS_handle.RS[k].Qk = 0;
							RS_handle.RS[k].cycle = cycle;
						}
					}
					RS_handle.RS[i].clearRS_Entry();

				}
				//break;
			}

		}
	}

}


void PileLine::Commit_(int & ending)
{
	//get the h
	// int min = std::numeric_limits<int>::max();
	int min  = 100000;
	for (int i = Entry_Start; i <= ROB_handle.ROB_CAPACITY ; i ++)
	{
		if (ROB_handle.ROB[i].order > 0 && ROB_handle.ROB[i].order< min) {
			min = ROB_handle.ROB[i].order;
		}
	}
	int h;
	for ( h = Entry_Start; h <= ROB_handle.ROB_CAPACITY ; h ++)
	{
		if (ROB_handle.ROB[h].order==min)
			break;
	}

	if (ROB_handle.ROB[h].cycle<cycle)
	{

		//then start commit
		if (ROB_handle.ROB[h].Ready==yes)
		{


			int d = ROB_handle.ROB[h].Dest;
			string s = ROB_handle.ROB[h].instruction;
			if (s.find("BEQ")!=string::npos||s.find("BNE")!=string::npos||s.find("BGEZ")!=string::npos||s.find("BGTZ")!=string::npos||s.find("BLEZ")!=string::npos||s.find("BLTZ")!=string::npos||s.find("J")!=string::npos)
			{


			}
			else if(ROB_handle.ROB[h].instruction.find("SW")!= string::npos)// store put the result in memory, after it commits, any dependent load(with the same address as the store) waiting in the RS is waken up and move to the memory access in the next cycle
			{
				
				int MemAddr = ROB_handle.ROB[h].Dest;
				MainMemory[MemAddr] = ROB_handle.ROB[h].Value;
                cout<<"memory"<<"["<<MemAddr<<"]:"<<MainMemory[MemAddr]<<endl;
                if (num <10 )
                {
                    DataSegement[num]= ROB_handle.ROB[h].Value;
                    cout<<"-----num----------"<<num<<"---------"<<endl;
                    cout<<"------DataSegement------"<<DataSegement[num]<<"-------"<<endl;
                    num++;
                }
			}
			else//put the result in register destination
			{
				RegS_handle.RegS[d].Value = ROB_handle.ROB[h].Value;

			}

			if(ROB_handle.ROB[h].instruction.find("BREAK") != string::npos)
			{
				ending = 1;
			}
			//free up ROB entry
			//ROB_handle.ROB[h].Busy = no, is it enough?
			ROB_handle.ROB[h].clearROB_Entry();

			//free up dest register if no one else writing it
			if (RegS_handle.RegS[d].Reorder==h)
			{
				RegS_handle.RegS[d].clearReg_Status_Entry();
			}

			

	}

}
}
