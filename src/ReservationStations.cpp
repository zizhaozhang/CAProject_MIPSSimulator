#include "ReservationStations.h"
////int ReservationStation::useless = -1;
//#define useless -1;
ReservationStation::ReservationStation()
	:RS_CAMPACITY(10),RS_SIZE(0)
{

}
//////////////////////////////////////////////////////////////////////////
ReservationStation_Entry::ReservationStation_Entry()
	:Busy(0), Op(""),Vj(useless), Vk(useless), Qj(useless), Qk(useless), Dest(useless),
	A(useless)
{
	result = useless;
	flag = 0;
	address = useless;
	order = useless;
}

//////////////////////////////////////////////////////////////////////////
//Zizhao Zhang
int ReservationStation::findEmptyEntry()
{
	int i;
	for (i = Entry_Start; i <= RS_CAMPACITY; i++)
	{
		if (RS[i].flag == 0){
			RS[i].flag = 1;
			return i;
		}
	}
	return -1;
}
bool ReservationStation::clearEntry(int ind)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//Xiaofan Zhou
void ReservationStation_Entry::clearRS_Entry()
{
	this->Busy = no;
	this->Op = "";
	this->Vj = useless;
	this->Vk = useless;
	this->Qj = useless;
	this->Qk = useless;
	this->Dest = useless;
	this->result = useless;
	this->flag = false;
	this->address = useless;
	this->cycle = 1;
	this->A = useless;
}