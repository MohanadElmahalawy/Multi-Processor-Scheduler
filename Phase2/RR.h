#pragma once
#include "Processor.h"
#include "LinkedQueue.h"

using namespace std;

class Scheduler;

class RR :public Processor
{
private:
	LinkedQueue<Processes*> rdyrr;

	int timeslice;
	int RTF; // process migration
	int currentslice=0;
	int currentOH=0;
	int rrAT = 0;
	

public:
	RR();

	void settsl(int tsl);

	int countready();
	void setrtf(int rtf);

	int gettsl();

	void printRdy();

	int countRdy();

	Processes* migration(Processes* x);

	void processoroverheat(Scheduler* S);//function that handles processor overheating

	void ScheduleAlgo(Scheduler* S, int timestep);

	int getnprocess();

	void addready(Processes* p1);
	void addready(Processes* p1, int ts);
	void removeready(Processes* &p1);

	int getQFT(int timestep);// gets the expected finishing time of the queue

	

	~RR();



};

