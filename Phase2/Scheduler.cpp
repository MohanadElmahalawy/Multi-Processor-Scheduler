#pragma once
#include "Scheduler.h"
#include <fstream>

using namespace std;

Scheduler::Scheduler()
{
	srand(time(0)+randindex);
	randindex+=3;
}

void Scheduler::SaveFile() {
	float sum=0;
	float sum1 = 0;
	float sum2 = 0;
	int sum3 = 0;
	int totaltrt=0;
	float temp;
	ofstream outFile;
	cout << "Enter output file name:" << endl;
	cin >> fileName;
	outFile.open(fileName + ".txt", ios::out);
	outFile << "TT		PID		AT		CT		IO_D		WT		RT		TRT" << endl;
	for (int i = 0; i < nprocesses; i++) {
		Trm.dequeue(px); 
		outFile << px->get_TT() << "		" << px->getPID() << "		" << px->get_AT() << "		" << px->get_CT() << "		";
		outFile <<px->gettotaliod()<< "		" << px->get_WT() << "		" << px->getRT() << "		" << px->get_TRT() << endl;
		totaltrt += px->get_TRT();
		sum1 += px->get_WT();
		sum2 += px->getRT();
		sum3 += px->get_CT();
		if (px->getIsorph()) {
			sum++;
		}
	}
	outFile << "Processes:" << nprocesses << endl;
	outFile << "Avg WT = " << sum1 / nprocesses << ",     Avg RT = " << sum2/nprocesses << ",      Avg TRT = " << totaltrt/nprocesses << endl;
	outFile << "Migration %:      RTF = " << (countrtfmig * 100) / nprocesses << "%,      MaxW = " << (countmaxwmig*100)/nprocesses << "% \n";
	outFile << "Work Steal % :" << (countsteal*100)/nprocesses << "%\n";
	outFile << "Forked Processes: " << (nforked*100 / nprocesses) << "% \n";
	outFile << "Killed Processes: ";
	for (int i = 0; i < nprocessors; i++) {
		if (P[i]->gettype() == 1) {
			sum+= P[i]->getcountkilled();
		}
	}
	outFile << (sum / nprocesses) * 100 << "% \n";
	outFile << "EDF: Finished Before Deadline: " << (countbeforedeadline * 100) / nprocesses << "% \n";
	outFile << "Processors: " << nprocessors << "[" << nFCFS << " FCFS, " << nSJF << " SJF, " << nRR << " RR, " << nEDF<< " EDF] " << endl;
	outFile << "Processors Load" << endl;
	for (int i = 0; i < nprocessors; i++) {
		//sum total turnaround - total waiting 
		ptrP = P[i];
		temp = ptrP->gettotalbusy()*100/(totaltrt-sum1);
		outFile << "p" << i + 1 << "=" << (temp )<<"%";
		if (i!=nprocessors-1)
			outFile << ",   ";
	}
	outFile <<endl << "Processors Utilization" << endl;
	sum = 0;//using sum again
	for (int i = 0; i < nprocessors; i++) {
		ptrP = P[i];
		temp = ptrP->gettotalbusy()/(ptrP->gettotalbusy() + ptrP->gettotalidle());

		outFile << "p" << i + 1 << "=" << temp*100 << "%";
		if (i != nprocessors - 1)
			outFile << ",   ";
		sum += temp*100;
	}
	
	outFile <<endl<< "Avg utilization= " << (sum / nprocessors) << "%\n";
	}
	
bool Scheduler::LoadFile() {
	ifstream inFile;
	char eater;
	int countp=1;
	////////////////////////////////////////
	cout << "Enter sample number (1-6)" << endl;
	cin >> fileName;
	inFile.open("sample" + fileName + ".txt", ios::in);
	//////////////////////////////////////////
	if (inFile.is_open()) {
		 
		inFile >> nFCFS >> nSJF >> nRR >> nEDF; // inputs number of processors of each type
		nprocessors = nFCFS + nSJF + nRR + nEDF;
		P = new Processor * [nprocessors];
		inFile >> tRR >> AT >> RTF; // inputs time slice for RR
		inFile >> MaxW >> STL >> ForkProb; // AT is n overheat (used for overheat)
		for (int i = 0; i < nFCFS; i++) {
			addFCFS(P,countp,MaxW,ForkProb,AT);
			countp++;
		}
		for (int i = 0; i < nSJF; i++){
			addSJF(P,countp,AT);
			countp++;
		}
		
	
		for (int i = 0; i < nRR; i++) {
			addRR(P,tRR,RTF, countp,AT);
			countp++;
		}
	    
		for (int i = 0; i < nEDF; i++) {
			addEDF(P, countp, AT);
			countp++;
		}
		inFile >> nprocesses;
		arrid = new int[nprocesses];
		
		for (int i = 0; i < nprocesses; i++) {
			totaliod = 0;
			inFile >> AT >> PID >> CT >> dedline>> nIO;
			arrid[i] = PID;
			ioq = new LinkedQueue<IO*>;
			if (nIO > 0) {
				
				for (int k = 0; k < nIO; k++) {
					io = new IO();
					inFile >> eater;
					inFile >> ior;
					io->IO_R = ior;
					inFile >> eater;
					inFile  >> iod;
					totaliod += iod;
					io->IO_D = iod;
					ioq->enqueue(io);
					inFile >> eater;
					if (k != nIO - 1)
						inFile >> eater;
					
				}
			
			}
			addNew(AT, PID, CT, nIO, ioq, dedline);
		}
		for (int i = 0; i < 12; i++)
			inFile >> eater;
		while (inFile.eof()==0)
		{
			inFile >> tkillp >> idkillp;
			killsig = new KILLSIG();
			killsig->kill_T = tkillp;
			killsig->kill_ID = idkillp;
			killQ.enqueue(killsig);
			nsigkill++;
		}
		cout << "File successfully loaded!" << endl;
		system("cls");
	}
	else {
		cout << "File not found";
		return false;
	}
	inFile.close();
	return true;
}
void Scheduler::addFCFS(Processor **P,int count, int maxw, int forkprob,int n)// construct a FCFS processor
{
	FCFS* P2 = new FCFS();
	int index = count - 1;
	P2->setnoverheat(n);
	P2->setforkprob(forkprob);
	P2->settype(1);
	P2->setMaxW(maxw);
	P2->setbusy(false);
	P2->setprocid(count);
	P[index] = P2;
	
}

LinkedQueue<KILLSIG*>* Scheduler::getkillq(){
	return &killQ;
}

bool Scheduler::searchBlk(Processes* P) {//looks for a certain process in the blk list
	Processes* P2;
	bool x=false;
	Blk.peek(P2);
	int i = 0;
	if (Blk.listCount())
	while (i != Blk.listCount()) {
		if (P2->getPID() == P->getPID())
			x= true;
		Blk.dequeue(P2);
		Blk.enqueue(P2);
		i++;
	}
	return x;

}

bool Scheduler::searchTrm(Processes* P) {//looks for a certain process in the trm list
	Processes* P2;
	bool x = false;
	Trm.peek(P2);
	int i = 0;
	if (Trm.listCount())
		for (int i=0;i<Trm.listCount();i++) {
			if (P2->getPID() == P->getPID())
				x = true;
			Trm.dequeue(P2);
			Trm.enqueue(P2);
			Trm.peek(P2);
		}
	return x;
}
void Scheduler::addSJF(Processor** P, int count,int n)// construct a SJF processor
{
	SJF* P2 = new SJF();
	int index = count - 1;
	P2->setnoverheat(n);
	P2->settype(2);
	P2->setbusy(false);
	P2->setprocid(count);
	P[index] = P2;

}

void Scheduler::addEDF(Processor** P, int count, int n)// construct a EDF processor
{
	EDF* P2 = new EDF();
	int index = count - 1;
	P2->setnoverheat(n);
	P2->settype(4);
	P2->setbusy(false);
	P2->setprocid(count);
	P[index] = P2;

}

void Scheduler::addRR(Processor** P, int tsl,int RTF, int count,int n)// construct a RR processor
{
	RR *P2=new RR();
	int index = count - 1;
	P2->setnoverheat(n);
	P2->setrtf(RTF);
	P2->settsl(tsl);
	P2->settype(3);
	P2->setbusy(false);
	P2->setprocid(count);
	P[index] = P2;

}

void Scheduler::addNew(int at,int pid,int ct,int nio,LinkedQueue<IO*>* ioq, int dedline)// function to add a new process
{
	
	Processes *P2 = new Processes(at, pid, ct, nio, ioq, dedline);

	New.enqueue(P2);

	
}

int Scheduler::getnprocessors()
{
	return nprocessors;
}

void Scheduler::StealLimit() // function to calculate the steal limit and set SQF and LQF
{
	SQFint = P[0]->getQFT(timestep);
	LQFint = P[0]->getQFT(timestep);
	LQFp = P[0];
	SQFp = P[0];//setting all variables equal to the initial processor
	int t;
	for (int i = 1; i < nprocessors; i++)
	{
		t = P[i]->getQFT(timestep);
		if (t>LQFint)
		{
			LQFint = t;
			LQFp = P[i];
		}
		else if (t < SQFint)
		{
			SQFint = t;
			SQFp = P[i];
		}
		
		
	}

	steallimit = ((LQFint - SQFint) * 100 / LQFint);//setting the steal limit data member

}

void Scheduler::handlingIO(Processes* p1, int ioduration)
{
	if (p1 && !p1->getparent())
	{
		p1->setcurrentiod(ioduration);
		p1->settotaliod(p1->gettotaliod() + ioduration);
			Blk.enqueue(p1);
	}
}

int Scheduler::getnFCFS()
{
	return nFCFS;
}

void Scheduler::blktordy()
{
	
	Processor* pr1 = NULL;
	Processes* p1 = NULL;

		Blk.peek(p1);
		if (p1)
		{
			p1->deccurrentiod();
			if (p1->getcurrentiod() == 0)
			{
				pr1 = getshortesttype(10,1);//getting shortest of all types
				Blk.dequeue(p1);
				pr1->addready(p1, timestep);
			}
		}
	}


void Scheduler::Steal()// function to perform stealing action
{
	Processes* p1=NULL;
	StealLimit();
	while (steallimit > 40)
	{
		if (((LQFp->gettype()!=1) || (LQFp->gettype() == 1 && LQFp->tostealornottosteal())) && !SQFp->getoverheat())
		{
			LQFp->removeready(p1);
			SQFp->addready(p1,timestep);
			countsteal++;
			SQFint = SQFp->getQFT(timestep);
			LQFint = LQFp->getQFT(timestep);
			steallimit = ((LQFint - SQFint) * 100 / LQFint);

		}
		else
			break;
		

	}
}

bool Scheduler::mig1(Processes* x)//migration function from RR to SJF
{
	Processor* s;
	s=getshortesttype(2,0);
	if (s)
	{
		s->addready(x,timestep);
		countrtfmig++;
		return true;
	}
	if (!s)
	{
		return false;
	}
	
}

bool Scheduler::mig2(Processes* x)//migration function from FCFS to RR
{
	Processor* s;
	s = getshortesttype(3,0);
	if (!s || x->getparent())
	{
		return false;
	}
	else if (s)//add only if a destination processor exists
	{
		s->addready(x,timestep);
		countmaxwmig++;
		return true;
	}
	
}
int Scheduler::gettimestep() {
	return timestep;
}
void Scheduler::incbeforedeadline()
{
	countbeforedeadline++;
}

bool Scheduler::newtordy(Processes* p)// function to move from new to ready
{
	Processor* shrt = NULL;
	shrt=getshortesttype(10,1);
	if (shrt)
	{
		shrt->addready(p,timestep);
		return true;
	}
	else
	{
		return false;
	}
}

bool Scheduler::fork(Processes* parent)// function to create a new foked process
{
	Processor* shrt = NULL;
	shrt = getshortesttype(1, 1);
	if (shrt)
	{
		Processes* child = new Processes(timestep, nprocesses + 1, parent->getremtime(), 0, NULL,  0);
		nprocesses++;
		if (child->getRT() == -1)
			child->setRT(timestep - child->get_AT());
		shrt->addready(child,timestep);
		if (!parent->getleftchild())//forktree
		{
			parent->setleftchild(child);
		}
		else
		if (!parent->getrightchild())
		{
			parent->setrightchild(child);
		}
		
		child->setparent(parent);
		nforked++;
		return true;
	}
	if (!shrt)
	{
		return false;
	}
}
void Scheduler::killorph(Processes * p)// function to kill forked processes
{
	for (int i = 0; i < nprocessors; i++)
	{
		if (P[i]->gettype() == 1)
		{
			P[i]->killchildren(p, this);
		}
	}
}

void Scheduler::addtotrm(Processes* pr)// function to terminate processes
{
	IO* io;
	if (pr->getremtime())
		pr->set_CT(pr->get_CT() - pr->getremtime());
	if (pr->getRT() == -1) {//premature termination check
		pr->setRT(0);
		pr->set_CT(0);
		pr->setremtime(0);
	}
	pr->set_TT(timestep);
	pr->set_TRT();
	pr-> set_WT();
	if (pr->getIO())
	{
		while (pr->getIO()->listCount())
		{
			pr->getIO()->dequeue(io);
			
		}
	}
	Trm.enqueue(pr);
}

// 1->FCFS
// 2->SJF
// 3->RR
//4->EDF
//10->Minimum of ALL
Processor* Scheduler::getshortesttype(int t, bool run)// function to get the shortest ready queue of a certain type
{
		int mint = 99999, min = 99999, minrunt = 99999, minrung = 99999;
		Processor* shrt = NULL;
		Processor* shrtt = NULL;
		Processor* shrtrunt = NULL;
		Processor* shrtrung = NULL;
		for (int i = 0; i < nprocessors; i++)
		{
			if (P[i]->getremtime() <= mint && P[i]->gettype() == t && P[i]->gettype() <5)//get shortest rdy of a certain type
			{
				if (P[i]->getremtime() < mint)
				{
					shrtt = P[i];
					mint = P[i]->getremtime();
					if (P[i]->getbusy() == 0)// get shortest rdy of a certain type that has no running process
					{
						shrtrunt = P[i];
						minrunt = P[i]->getremtime();

					}
				}
				if (P[i]->getremtime() == mint && P[i]->getbusy() == 0)
				{
					shrtt = P[i];
					mint = P[i]->getremtime();
					shrtrunt = P[i];
					minrunt = P[i]->getremtime();


				}
			}
			if (P[i]->getremtime() <= min && t == 10 && P[i]->gettype() < 5)// get shortest rdy of all types
			{
				if (P[i]->getremtime() < min)
				{
					shrt = P[i];
					min = P[i]->getremtime();
					if (P[i]->getbusy() == 0)// get shortest rdy of all types that has no running process
					{
						shrtrung = P[i];
						minrung = P[i]->getremtime();

					}
				}
				if (P[i]->getremtime() == min && P[i]->getbusy() == 0)
				{
					shrt = P[i];
					min = P[i]->getremtime();
					shrtrung = P[i];
					minrung = P[i]->getremtime();


				}
			}

		}
		if (t == 10)
		{
			if (run)// return shortest ready of all types that is not busy
			{
				if (shrtrung)
				{
					return shrtrung;
				}
				return shrt;
			}
			else // return shortest ready of all types regardless of if its busy or not
			{
				return shrt;
			}

		}
		else
		{
			if (run)// return shortest ready of a certaian type that is not busy
			{
				if (shrtrunt)
				{
					return shrtrunt;
				}
				return shrtt;
			}
			else // return shortest ready of a certaian type regardless of if its busy or not
			{
				return shrtt;
			}
		}

	}






void Scheduler::simulator()
{
	bool overheat;
	int modee= ui.SelectMode();
	while (modee < 1 || modee>3) {
		cout << "Error. Enter another mode" << endl;
		modee = ui.SelectMode();
	}
	bool Loaded;
	
	Loaded = LoadFile();
	while (Loaded < 1 || Loaded>6)
	{
		cout <<endl<< "Error. Try again." << endl;
		Loaded = LoadFile();
	}
	Processor* proc = NULL;
	Processes* x = NULL;
	Processes* x2 = NULL;
	Processor* p = NULL;
	bool killstatus = false;
	while (Trm.listCount() != nprocesses) 
	{

		for (int k = 0; k < nprocessors; k++) { // calling sigkill of each processor (done seperately on purpose)
			if (P[k]->gettype() == 1) {
				killstatus = P[k]->Killsig(this, timestep, k);
				if (killstatus)
					k = -1;
			}
		}

				for (int i = 0; i < nprocessors; i++)
				{
						if (!P[i]->getoverheat() && timestep!=0) {// checking for overheating probability
							overheat = (rand() % 100 +1) < 2;
							P[i]->setoverheat(overheat);
						}
						if (P[i]->getoverheat())
						{
							P[i]->processoroverheat(this);
						}

					P[i]->ScheduleAlgo(this, timestep);
				
				}



	
		blktordy();

		//////////////////////////////////////////////////work stealing 
		if (timestep%STL==0 && timestep!= 0)
		{
			Steal();
		}
		New.peek(x);
		if (x)
			if (x->get_AT() == timestep)//moving processes from new to ready
				do
				{
					New.dequeue(x);
					newtordy(x);

					New.peek(x);
					if (New.isEmpty())
						break;
				} while (x->get_AT() == timestep);

				for (int i = 0; i < nprocessors; i++) {
					if (P[i]->getbusy() || P[i]->countready())
						P[i]->inctotalbusy();
					else
						P[i]->inctotalidle();
				}
		ui.Display(P,nprocessors, &Blk, &Trm, timestep,modee);

		timestep++;
	
		
	}
	if (Loaded)
	{
		SaveFile();
		cout << endl << "Done!";
	}
};

Scheduler::~Scheduler() {
	delete[] P;
	delete[] arrid;
	delete ioq;
	//delete LQF;
	//delete SQF;
}

