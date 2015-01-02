#ifndef  COMMONHEADER_H
#define  COMMONHEADER_H

#include <iostream>
#include <string>
#include<math.h>
#include <vector>
#include <queue>
#include <map>
#include <assert.h>
using namespace std;
// The type of the Instruction
typedef unsigned int Instr;
// The type for the value in ROB and RS
typedef int Operand;
typedef int RegisterID; // if no useless set it to -1 so need signed int
typedef int ROB_Index;
#define registerTotalNum 32
#define no 0
#define yes 1
#define Entry_Start 1



#endif 