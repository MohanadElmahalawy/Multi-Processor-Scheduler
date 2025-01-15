#include <iostream>
#include "LinkedQueue.h"
#include "IO.h"
#pragma once
using namespace std;
class Processes
{
	private:
		int PID;
		int AT;
		int RT;
		int CT;
		int TT;
		int TRT;
		int WT;
		int* IOR;
		int* IOD;
		int totaliod;
		int IOnum;
		int remtime; // CT - timesteps currently ran 
		Processes* leftchild;
		Processes* rightchild;
		Processes* parent;
		LinkedQueue<IO*>* iop;
		bool Is_orph;
		int currentiod;
		int deadline;
		
public:

	Processes();
	Processes(int at, int pid, int ct, int nio, LinkedQueue<IO*>* ioq,int dedline);
	~Processes();
	void setdeadline(int d);
	int getdeadline();
	void dec_remtime(); // function that decrements remtime as timestep passes
	void setPID(int PID);
	int getPID();
	void incwaittime();
	int gettotaliod();
	void settotaliod(int iod);
	void set_AT(int AT);
	int get_AT();
	void set_CT(int CT);
	int get_CT();
	void set_TT(int TT);
	int get_TT();
	void set_TRT(); 
	void set_TRT(int ts);
	void set_WT(int ts);
	int get_TRT();
	void set_WT();
	int get_WT();
	bool getIsorph();
	void setIsorph(bool Is_orph);
	void setRT(int CPUtime);
	int getRT();
	void setnio(int nio);
	void setremtime(int t);
	
	void setIO(LinkedQueue<IO*> io);
	Processes* getparent(); // getter for parent 
	void setparent(Processes*parent); // setter for parent
	Processes* getleftchild(); // getter for child 
	Processes* getrightchild(); // getter for child 
	void setleftchild(Processes* child); // setter for parent
	void setrightchild(Processes* child); // setter for parent
	void setcurrentiod(int ciod);
	int getcurrentiod();
	void deccurrentiod();


	int getremtime();
	LinkedQueue<IO*>* getIO();
    friend ostream& operator<<(ostream& output, Processes* p1);
	bool operator ==(int k);
	bool operator ==(Processes* p);

};