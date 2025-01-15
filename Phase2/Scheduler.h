#pragma once
#include "LinkedQueue.h"
#include <fstream>
#include "FCFS.h"
#include "SJF.h"
#include "RR.h"
#include "EDF.h"
#include "UI2.h"
#include "IO.h"
#include "KILLSIG.h"

class Scheduler
{
private:
	int nFCFS, nSJF, nRR,nEDF, tRR, nprocesses, nprocessors, AT, CT, tkillp, idkillp, RTF, MaxW, ForkProb, STL, nIO, ior, iod, migrationperc,dedline;
	int TT, PID, WT, TRT, avgWT, avgRT, avgTRT, RTFperc, wstealperc, MaxWperc,forkperc,killperc, steallimit, SQFint, LQFint;
	int timestep, nsigkill, nRDY, nTRM, nBLK, ctrm, crun,nforked,nkilled = 0, randindex=0;
	float countbeforedeadline = 0;
	int countsteal = 0;
	float countrtfmig = 0;
	float countmaxwmig = 0;
	int totaliod=0;
	int* arrid;
	string fileName;

	Processes* px;
	Processor** P;
	Processor* ptrP;
	LinkedQueue <Processes*> New;
	LinkedQueue <Processes*> Blk;
	LinkedQueue <Processes*> Trm;
	LinkedQueue<IO*>* ioq;
	Processor* LQFp;
	Processor* SQFp;

	UI2 ui;

	
	LinkedQueue<KILLSIG*> killQ;

	IO* io;
	KILLSIG* killsig;

public:
	Scheduler(); //Default constructor
	bool LoadFile();
	void SaveFile();
	int gettimestep();
	void addFCFS(Processor **P,int count, int maxw,int forkprob,int n);
	void addSJF(Processor** P, int count,int n);
	void addEDF(Processor** P, int count, int n);
	void addRR(Processor** P, int tsl,int RTF, int count,int n);
	void addNew(int AT, int PID, int CT, int nIO, LinkedQueue<IO*>* ioq, int dedline);
	bool searchBlk(Processes* P); // used in killsig, searches for process in BLK
	bool searchTrm(Processes* P); // used in killsig, searches for process in TRM
	void incbeforedeadline();
	int getnFCFS();
	int getnprocessors();
	void blktordy(); // function to check blk every timestep and move processes to shortest rdy list
	void handlingIO(Processes* p1,int ioduration); // function that handles the I/O of the processes moving it from run to blk
	void StealLimit(); // function to calculate steallimit
	void Steal();//function that performs the actual theft 
	bool mig1(Processes* x); // RR to SJF
	bool mig2(Processes* x); //FCFS to RR
	void addtotrm(Processes* pr);
	Processor* getshortesttype(int x, bool run); // returns the shortest processor queue (including run or excluding it)
	bool newtordy(Processes* p); //moves from new to the shortest rdy queue 
	bool fork(Processes* parent); // function to create a new forked process
	void killorph(Processes* p);
	void simulator(); //function that simulates the scheduler
	LinkedQueue<KILLSIG*>* getkillq();
	~Scheduler();
};