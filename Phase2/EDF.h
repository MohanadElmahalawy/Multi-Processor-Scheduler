#pragma once
#include "Processor.h"
#include "linkedpqueue.h"
class EDF : public Processor
{
private:
    linkedpqueue<Processes*> rdyedf;
    int currentOH = 0;
	int edfAT = 0;
public:
	EDF();

	void processoroverheat(Scheduler* S);

	int countready();

	void ScheduleAlgo(Scheduler* S, int timestep);

	void printRdy();

	int getnprocess();

	void addready(Processes* p1);

	void addready(Processes* p1, int ts);

	void removeready(Processes*& p1);

	int getQFT(int timestep);// gets the expected finishing time of the queue

	~EDF();
};

