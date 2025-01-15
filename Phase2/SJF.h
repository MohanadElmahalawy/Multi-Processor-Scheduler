#pragma once
#include "Processor.h"
#include "Processes.h"
#include "linkedpqueue.h"
#include "LinkedQueue.h"
using namespace std;

class Scheduler;

class SJF : public Processor
{
private:

	linkedpqueue<Processes*> rdysjf;
	int currentOH=0;
	int sjfAT = 0;
public:
	SJF();

	void processoroverheat(Scheduler* S);//function that handles processor overheating

	int countready();
	void ScheduleAlgo(Scheduler* S,int timestep);

	void printRdy();

	int getnprocess();

	void addready(Processes* p1);

	void addready(Processes* p1, int ts);

	void removeready(Processes*& p1);

	int getQFT(int timestep);// gets the expected finishing time of the queue

	

	~SJF();




};

