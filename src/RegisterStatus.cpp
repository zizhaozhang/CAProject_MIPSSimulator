#include "RegisterStatus.h"
#include "commonHeader.h"
Register_Status::Register_Status()
	: REGISTER_CAMPACITY(registerTotalNum)
{


}


//////////////////////////////////////////////////////////////////////////
Register_Status_Entry::Register_Status_Entry()
	:Reorder(useless), Busy(no), Value(0)
{

}
void Register_Status_Entry:: clearReg_Status_Entry()
{
	this->Reorder = useless;
	this->Busy = no;
	//this->Value = 0; //can not free up the value
}