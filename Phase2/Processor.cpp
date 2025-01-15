#include "Processor.h"

#include <string>
Processor::Processor()
{
	load=0;
   util=0; //processor utilization
	ptype=0;
	Processes* Run=NULL;
	RunID=0;
	remtime=0;
	procid=0;
	busytotal=0; // total processor busy time (for load and util)
	idletotal=0; // for util
};

Processor::Processor(int t)
{
	ptype = t;
	load = 0;
	util = 0; //processor utilization
	ptype = 0;
	Processes* Run = NULL;
	RunID = 0;
	remtime = 0;
	procid = 0;
	busytotal = 0; // total processor busy time (for load and util)
	idletotal = 0; // for util
	
}

void Processor::setnoverheat(int n)
{
	noverheat = n;
}

int Processor::countready()
{
	return 0;
}

int Processor::getnoverheat()
{
	return noverheat;
}

void Processor::setoverheat(bool x)
{
	overheat = x;
}

bool Processor::getoverheat()
{
	return overheat;
}

void Processor::inctotalidle() {
	idletotal++;
}
int Processor::gettotalidle() {
	return idletotal-2;
}
void Processor::settotalidle(int x){
	idletotal = x;
}

void Processor::inctotalbusy() {
	busytotal++;
}
bool Processor::getbusy()
{
	return isbusy;
};

void Processor::setrun(Processes* p)
{
	Run = p;
	RunID = p->getPID();
}

void Processor::setbusy(bool x)
{
	isbusy = x;
	if (!x)
	Run = NULL;
}
void Processor::addready(Processes* p1)
{
}
void Processor::addready(Processes* p1,int ts)
{
}


void Processor::setprocid(int count) {
	procid = count;
}

int Processor::getprocid() {
	return procid;
}

bool Processor::addrun(Processes *p1)
{
	
	Run = p1;
	setrun(p1);

	setbusy(true);

	if (Run == p1)
		return true;
	
	return false;
}
float Processor::gettotalbusy() {
	return busytotal;
}
int Processor::getcountkilled()
{
	return -999;
}
void Processor::settotalbusy(int x) {
	busytotal = x;
}
Processes* Processor::removerun() {

	Processes* p1 = Run;
	Run = NULL;
	isbusy = false;
	return p1;

}
void Processor::getrun(Processes* p1)
{
	p1 = Run;

}
Processes* Processor::getrun()
{
	return Run;
}

void Processor::killchildren(Processes* p, Scheduler* S)
{
}

bool Processor::tostealornottosteal()
{
	return false;
}

void Processor::removeready(Processes*& p1)
{
	
}

void Processor::processoroverheat(Scheduler* S) {

};
void Processor::ScheduleAlgo(Scheduler * Sx, int timestep)
{
	
}

bool Processor::Killsig(Scheduler* S, int timestep,int j)
{
	return false;
}

void Processor::settype(int t) { 
	ptype = t;
}
int Processor::gettype() {
	return ptype;
}
string Processor::getpstype() {
	switch (gettype()) {
	case 1:
		return "FCFS";
	case 2:
		return "SJF";
	case 3:
		return "RR";
	case 4:
		return "EDF";
	case 5:
		return "Overheated FCFS";
	case 6:
		return "Overheated SJF";
	case 7:
		return "Overheated RR";
	case 8:
		return "Overheated EDF";
	default:
		return "Error. Undefined processor type";
	}

}

int Processor::getremtime()
{ 
	return getQFT(0);
}

void Processor::printRdy()
{
}


int Processor::getnprocess()
{
	return 0;
}

Processor::~Processor()
{
	delete Run;
	
};
