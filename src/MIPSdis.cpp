//-----------------------------------------------------------------------------------------------------------------------
// Titile: CDA5155 Computer Architecture project I
// Author: Zizhao Zhang
// UFID: 11338226
// Email: zizhaozhang@ufl.edu

// All rights reserved
//-----------------------------------------------------------------------------------------------------------------------


#include"MIPSdis.h"
#include <queue>
using namespace std;
//define
#define Bit6 0x3f
#define Bit5 0x1f
#define Bit3 0x7
#define Bit16 0xffff

MIPsDis::MIPsDis()
{

}
string MIPsDis::Int2String(Instr target)
{
	string m("");
	Instr remainder = 0;
	Instr temp = target;
	do 
	{
		remainder = temp % 10;
		m = (char)((int)'0' + remainder) + m;
		temp = temp / 10;
	}while (temp != 0);
	return m;
}
int MIPsDis::unsigned2sign(Instr target)
{
	int s;
	if((target >> 15) & 0x1 ==  1) // is nagative
	{
		return target + 0xffff0000;
	}
	else 
	{
		s = (int) target; 
		return s;
	}
}
string MIPsDis::Int2String_signed(Instr target)
{   
	
	string m("");
	Instr temp = target;
	if((temp >> 15) & 0x1 ==  1) //is negative
	{
		temp =	(~(temp - 1)) & 0xffff; //inverse low 16bit here we assume the target has 16 bits but it is not good 
		m = '-' + Int2String(temp);
		//cout<<MIPsDis::unsigned2sign(target);
	}
	else
	{
		m = Int2String(temp);
	}

	return m;
}
string MIPsDis::instructionParser(Instruction & instruction, Instr in)
{

    //in =  Big_endian2Little_endian(in);
    //Parser handle;
    string Result;
    Parser::exParser(instruction, Result, in);
    // if(Result == "NOP")
    // {
    //     handle.ifNOP = true;
    // }
    return Result;
}
//Convert machine language to string for print out
string MIPsDis::ConvertNbit(Instr target, int n)
{
    string output("");
    for(int i=0; i < n; i++)
    {
        output = Int2String((target >> i) & 0x1) + output;
    }
    return output;
}
string MIPsDis::Instr2Char(Instr target)
{
    string output("");
    //6 5 5 5 5 6

    if(Parser::isBreak <= 1){ // have not meet break
        output = ConvertNbit((target >> 26) & Bit6, 6) + ' ' +
                 ConvertNbit((target >> 21) & Bit5, 5) + ' ' +
                 ConvertNbit((target >> 16) & Bit5, 5) + ' ' +
                 ConvertNbit((target >> 11) & Bit5, 5) + ' ' +
                 ConvertNbit((target >> 6) & Bit5, 5) + ' ' +
                 ConvertNbit((target >> 0) & Bit6, 6);
    }
    else
    {
        output = ConvertNbit((target >> 26) & Bit6, 6) +
                ConvertNbit((target >> 21) & Bit5, 5) +
                ConvertNbit((target >> 16) & Bit5, 5) +
                ConvertNbit((target >> 11) & Bit5, 5) +
                ConvertNbit((target >> 6) & Bit5, 5) +
                ConvertNbit((target >> 0) & Bit6, 6);
    }
    return output;

}
void MIPsDis::PrintOut(FILE * out, Instr machLan, string Assembly)
{
    string outString = Instr2Char(machLan) + " " + Int2String((unsigned int) StartAddr) + " " + Assembly;
    StartAddr += 4;
    cout<<outString<<endl;
    fprintf(out, outString.c_str());
    fprintf(out, "\n");
}
int MIPsDis::Disassembler(vector<Instruction> &Respos, FILE *input, FILE *output)
{
    Instr OneOP;
    //
    int ex = 1;
    while (!ferror(input))
    {

        ex = fread(&OneOP, sizeof(Instr), 1, input);
        if(ex == 0) break;
        OneOP = Big_endian2Little_endian(OneOP);
		Instruction currentInstr;
		string result = instructionParser(currentInstr, OneOP);
		Respos.push_back(currentInstr);
        PrintOut(output, OneOP, result);
		if (StartAddr > 716)
			break;
		
    }
	return 1;

}
int MIPsDis::CallMIPsDisassembler(vector<Instruction> &Respos, char *filenameInput, char* filenameOutput)
{
	/*if(argc != 4)
	{
	cout<<"input format error!"<<endl;
	cout<<"Format: ./MIPSsim [inpuptfilename] [outputfilename] -dis"<<endl;
	return 0;
	}*/
  //  char filename[] = "C:\\Users\\zzz\\Dropbox\\Computer Architecture\\Project2\\Proj\\fibonacci.bin";
    //char outputname[] = "C:\\Users\\zzz\\Dropbox\\Computer Architecture\\Project2\\Proj\\output.txt";
    //Open binary file
    FILE* input = fopen(filenameInput,"rb");

    FILE* output = fopen(filenameOutput,"w");
    if (input == NULL || output == NULL ){
        printf("Failed open file\n");
		fclose(input);
		fclose(output);
		return 0;
    }
    else
    {
        cout<<"File load successfully!"<<endl;

    }
	//if(!strcmp(argv[3], "-dis"))
	//{
    Disassembler(Respos, input, output);
    cout<<"File write successfully!"<<endl;
	//}
	/*else{
		cout<<"Do not support other functions now!"<<endl;
	}
    *///read binary file one instruction by one instruction
    fclose(input);
    fclose(output);

    return 1;
}
