/*
CDA5155 Fall 2014
Computer Architecture Course Project II: MIPsSimulator

-------------------------------------------------
Student Information

Name: Zizhao Zhang
UFID: 11338226
E-mail: zizhaozhang@ufl.edu

Name: Xiaofan Zhou
UFID: 54659863
E-mail:zhouxiaofan@ufl.edu


------------------------------------------------
How to use

- Go to the /src folder
- >> make
- Go to the /bin folder. The executable file is here
- >> ./MIPSsim inputfilename outputfilename -sim [-Tm:n]
- if error format, it will return the format requirement
- [-Tm:n] is optional. It will output all the cycles in default.



-------------------------------------------------
Authenticity

"On my honor, I have neither given nor received unauthorized aid on this assignment"
*/

#include <iostream>
#include "MIPsSimulator.h"
#include <string>
using namespace std;

void MainCycle(PileLine pipeline, vector<Instruction> InstMemory, char *output, int * range)
{
	int cycle = 1;
	int ending = 0;
	std::ofstream outfile(output,ios::out);

    //zhouxiaofan
	while(1)
	{

		pipeline.InstructionFetch();
		if (cycle >= 2) 
			pipeline.DecodeandIssue();
        //cout<<cycle<<"1$$$$$"<<pipeline.getIQ().size()<<endl;
        if (cycle >= range[0] && cycle <= range[1])
			pipeline.PrintOutResult(outfile);
		if (cycle >= 3)
			pipeline.Execution();
        //cout<<"2$$$$$"<<pipeline.getIQ().size()<<endl;
		if(cycle >= 4)
			pipeline.Write_Result();
//        cout<<"3$$$$$"<<pipeline.getIQ().size()<<endl;
		if(cycle >= 5)
            pipeline.Commit_(ending);
//        cout<<"4$$$$$"<<pipeline.getIQ().size()<<endl;
		//cout<<pipeline.PC;
		if (ending == 1)
		{
	        if ((cycle >= range[0] && cycle <= range[1])||(range[0]==0&&range[1]==0))
    	        {
    	        	outfile<<"Final ";
    	        	pipeline.PrintOutResult(outfile);
    	        }
			cout<<"End the program!!"<<endl;
			break;
		}
        //cout<<"--------------"<<endl;
	  	cycle += 1;
		pipeline.cycle += 1;

	}
	// print out final 45
	// add noset to BTB
	 outfile.close();
	 cout<<"!!! result has been writen in: "<<output<<endl;
}
int main(int argc, char **argv)
{

	if(argc < 4) // it is ok to miss [-Tm:n]
	{
		cout<<"input format error!"<<endl;
		cout<<"Format: ./MIPsSimulator [inpuptfilename] [outputfilename] -operation [-Tm:n]"<<endl;
		cout<<"Inputfilename - The file name of the binary input file."<<endl;
		cout<<"Outputfilename - The file name to which to print the output."<<endl;

		cout<<"Operation - Either dis or sim to specify disassembly or simulation."<<endl;
			
		cout<<"-Tm:n (optional) - Optional argument to specify the start (m) and end (n) cycles of simulation tracing output. Tracing should be done in a single-step fashion with the contents of registers and memory shown after every processor cycle. -T0:0 indicates that no tracing is to be performed; eliminating the argument specifies that every cycle is to be traced."<<endl;
		return 0;
	}



	MIPsDis mipsDis;
	// char inputname[] = "fibonacci_bin.bin";
	// char outputname[] = "output.txt";
	char *inputname = argv[1];
	char *outputname = argv[2];
	char *operation = argv[3];
	const char *truck; 
	int range[2];
	range[0] = 0;
	range[1] = 45;
	if (argc == 5)
	{
		truck = argv[4];
		int i = 2;
		string start;
		string end;
		bool flag = false;
		while(truck[i]!='\0')
		{
			if(truck[i]!=':'&& flag == false)
			{
				start+=truck[i];
				i++;
				continue;
			}
			if(truck[i]==':')
			{
				flag = true;
				i++;
				continue;
			}
			else
			{
				end+=truck[i];
				i++;
			}
			
		}

		// range[0] = stoi(start);
		// range[1] = stoi(end);
		range[0] =  atoi(start.c_str());
		range[1] = atoi(end.c_str());
		cout<<"rang0:"<<range[0]<<endl;
		cout<<"rang1:"<<range[1]<<endl;
	}


	vector<Instruction> InstMemory; // return the instMemeory
	if(!strcmp(operation, "dis"))
		mipsDis.CallMIPsDisassembler(InstMemory, inputname, outputname);
	else
		mipsDis.CallMIPsDisassembler(InstMemory, inputname, "result_dis.txt");

	if (!strcmp(operation, "sim"))
	{
		StartAddr = 600;
		PileLine pipeline(InstMemory, StartAddr);
		MainCycle(pipeline, InstMemory, outputname, range);
	}

	
	return 0;
}

