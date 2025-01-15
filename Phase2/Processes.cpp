#include <iostream>
using namespace std;
#include "Processes.h"



	

	Processes::Processes()
	{
		leftchild = NULL;
		rightchild = NULL;
	};

	Processes::Processes(int at, int pid, int ct, int nio, LinkedQueue<IO*>* ioq,int dedline)
	{
		AT = at;
		CT = ct;
		PID = pid;
		IOnum = nio;
		iop = ioq;
		totaliod = 0;
		RT = -1;
		TRT = 0;
		TT = 0;
		Is_orph = false;
		parent = NULL;
		leftchild = NULL;
		rightchild = NULL;
		remtime = ct;
		deadline = dedline;
	}
	
	void Processes::setdeadline(int d) {
		deadline = d;
	}
	int Processes::getdeadline() {
		return deadline;
	}
	void Processes::dec_remtime()
	{
		remtime--;
	}

	void Processes:: setPID(int id)
	{
		PID=id;
	}
	int Processes:: getPID()
	{
		return PID;
	}

	
	void Processes:: set_AT(int arrtime)
	{
		AT=arrtime;
	}
	int Processes:: get_AT()
	{
		return AT;
	}
	
	void Processes:: set_CT(int CPUtime)
	{
		CT=CPUtime;
	}
	int Processes:: get_CT()
	{
		return CT;
	}
	void Processes:: set_TT(int Termination_Time)
	{
		TT=Termination_Time;
	}
	int Processes:: get_TT()
	{
		return TT;
	}
	void Processes:: set_TRT()
	{
		TRT=TT-AT;
	}
	int Processes:: get_TRT()
	{
		return TRT;
	}
	void Processes:: set_WT()
	{
	
		WT=TRT-CT;
	}
	int Processes:: get_WT()
	{
		return WT;
	}
	bool Processes::getIsorph()
	{
		return Is_orph;
	}
	void Processes::setIsorph(bool is_orph)
	{
		Is_orph = is_orph;
	}
	void Processes::incwaittime() {
		WT++;
	}
	void Processes::set_TRT(int ts)
	{
		TRT = ts;
	}
	void Processes::set_WT(int ts) {
		WT = ts;
	}
	void Processes::setRT(int remtime)
	{
		RT = remtime;
	}
	void Processes::setnio(int nio)
	{
		IOnum = nio;
	}
	void Processes::setremtime(int t)
	{
		remtime = t;
	}
	int Processes::getRT()
	{
		return RT;
	}

	Processes* Processes::getparent()
	{
		return parent;
	}

	void Processes::setparent(Processes* Parent)
	{
		parent = Parent;
	}

	Processes* Processes::getleftchild()
	{
		return leftchild;
	}

	Processes* Processes::getrightchild()
	{
		return rightchild;
	}

	void Processes::setleftchild(Processes* child)
	{
		leftchild = child;
	}

	void Processes::setrightchild(Processes* child)
	{
		rightchild = child;
	}

	bool Processes::operator==(Processes* p)
	{
		if (PID == p->getPID()) {
			return true;
		}
		else
			return false;
	}

	void Processes::setcurrentiod(int ciod)
	{
		currentiod = ciod;
	}

	int Processes::getcurrentiod()
	{
		return currentiod;
	}

	void Processes::deccurrentiod() {
		currentiod--;
	}


	LinkedQueue<IO*>* Processes::getIO()
	{
		return iop;
	}
		

	int Processes::getremtime()
	{
		return remtime;
	}


	int Processes::gettotaliod() {
		return totaliod;
	}
	void Processes::settotaliod(int iod) {
		totaliod = iod;
	}

	ostream& operator<<(ostream& os, Processes* p)
	{
		os << p->getPID();
		return os;
	}

	bool Processes::operator ==(int k) {
		if (PID == k)
			return true;
		else
			return false;
	} 

	Processes::~Processes() {
		delete leftchild;
		delete rightchild;
	}