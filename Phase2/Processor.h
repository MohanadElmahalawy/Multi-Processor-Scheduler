//#ifndef SCHEDULER_H
#include "LinkedQueue.h"
#include "Processes.h"
#include <string>
#pragma once
using namespace std;
class RR;
class SJF;
class FCFS;
class Scheduler;

class Processor
{
private:


	bool isbusy = false;
	int load;
	int util; //processor utilization
	int ptype;
	Processes* Run;
	int RunID;
	int remtime;//remaining time till the processor is done running
	int procid;
	int busytotal; // total processor busy time (for load and util)
	int idletotal; // for util
	bool overheat;
	int noverheat;
	int currentnOH=0;




public:

	Processor();
	Processor(int t);
	void setnoverheat(int n);
	virtual int countready();
	int getnoverheat();
	void setoverheat(bool x);
	bool getoverheat();
	void inctotalbusy();
	void inctotalidle();
	int gettotalidle();
	void settotalidle(int x);
	bool getbusy();
	void setrun(Processes *p);
	void setbusy(bool x);
	virtual void addready(Processes* p1);
	virtual void removeready(Processes*& p1);
	virtual void addready(Processes* p1, int ts);
	virtual void processoroverheat(Scheduler* S)=0;
	virtual void ScheduleAlgo(Scheduler* Sx,int timestep) =0;
	virtual bool Killsig(Scheduler* S, int timestep,int k);
	~Processor();
	void settype(int t);
	int gettype();
	string getpstype();
	
	int getremtime();
	virtual void printRdy();
	virtual int getnprocess();
	void setprocid(int count);
	int getprocid();
	bool addrun(Processes* p1);
	float gettotalbusy();
	virtual int getcountkilled(); //returns how many processors were killed 
	virtual int getQFT(int timestep)=0;// gets the expected finishing time of the queue
	
	void settotalbusy(int x);

	Processes* removerun();
	
	 void getrun(Processes* p1);
	 Processes* getrun();
	 virtual void killchildren(Processes* p, Scheduler* S);//kill all children of the given process
	 virtual bool tostealornottosteal();//checks wether the first process is forked or not
	friend ostream& operator<<(ostream& output, Processor* p1);

};