#include "ReorderBuffer.h"


ReorderBuffer::ReorderBuffer()
	:ROB_CAPACITY(6),ROB_SIZE(0)
{

}



//////////////////////////////////////////////////////////////////////////
// ReorderBuffer_Entry defination
ReorderBuffer_Entry::ReorderBuffer_Entry()
	:Value(useless),Dest(useless),state(State(NULL)),Busy(no), entry_ID(useless)
{
	Ready = yes;
	instruction = "";
	flag = 0;
	order = 0;
	address = 0;

	cycle = 1;
}
void ReorderBuffer_Entry::clearROB_Entry()
{
	this->Ready = yes;
	this->Busy = no;
	this->instruction = "";
	this->state = State(NULL);
	this->Dest = useless;
	this->Value = useless;
	this->flag = false;
	this->order = useless;
	this->address = useless;
	this->cycle =1 ;
	


}
int ReorderBuffer::findEmptyEntry()
{
	int i;
	for (i = Entry_Start; i <= ROB_CAPACITY; i++)
	{
		if (ROB[i].flag ==0){
			ROB[i].flag = 1;
			return i;
		}
	}
	return -1;
}