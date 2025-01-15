#include "FCFS.h"
#include "Scheduler.h"
#include <random>
FCFS::FCFS()
{

}

int FCFS::countready()
{
	return rdyfcfs.listCount();
}

void FCFS::processoroverheat(Scheduler* S) {
	Processor* sp;
	Processes* pr;
	if (currentOH == getnoverheat()) {//return to normal settins after overheating timesteps are over
		setoverheat(false);
		settype(1);
	}
	else
	if (currentOH < getnoverheat()) {
		settype(5);
		pr = getrun();
		
		if (pr)//remove running process
		{
			removerun();
			if (pr->getparent())//check that its not forked
			{
				sp = S->getshortesttype(1, 1);
				if (sp)
				{
					sp->addready(pr,S->gettimestep());
				}
				else
					if (!sp)
					{
						S->killorph(pr);
						S->addtotrm(pr);
					}
			}
			else
			{
				sp = S->getshortesttype(10, 1);

				if (sp)
				{
					sp->addready(pr);
				}
				else
					if (!sp)
					{
						S->addtotrm(pr);
					}
			}
		}
		while (rdyfcfs.listCount())//empty ready list
		{
			rdyfcfs.getFirst(pr);
			rdyfcfs.DeleteFirst();
			
			
			if (pr->getparent())//check that its not forked
			{
				sp = S->getshortesttype(1, 1);
				if (sp)
				{
					sp->addready(pr);
				}
				else
				if (!sp)
				{
					S->killorph(pr);
					S->addtotrm(pr);
				}
			}
			else
			{
				sp = S->getshortesttype(10, 1);
				if (sp)
				{
					sp->addready(pr);
				}
				else
					if (!sp)
					{
						S->addtotrm(pr);
					}
			}
			
		}
		currentOH++;
	}

}
void FCFS::ScheduleAlgo(Scheduler* S, int timestep)
{
	Processes* P1 = NULL;
	bool mig = false;
	if (getbusy())
	{
		P1 = getrun();
		
		P1->dec_remtime();

		if (P1->getremtime() <= 0)
		{
			if (P1->getleftchild() || P1->getrightchild())/////kill children
			{
				S->killorph(P1);
			}
			// 7aram
			P1 = removerun();
			if (P1)
			{
				S->addtotrm(P1);
				this->ScheduleAlgo(S, timestep);
				return;
			}
		}
		if (forkprobabilty() && (P1->getleftchild() == NULL || P1->getrightchild() == NULL)) //////////////forking
		{
			S->fork(P1);
		}
		LinkedQueue<IO*>* iopq = P1->getIO();
		if (P1->getIO())
		{
			if (iopq->listCount())
			{
				IO* iox;
				iopq->peek(iox);
				if (iox->IO_R == (P1->get_CT() - P1->getremtime()) && P1->getremtime() != 0)
				{
					iopq->dequeue(iox);
					P1 = removerun();
					S->handlingIO(P1, iox->IO_D);
					if (rdyfcfs.listCount() && !getbusy()) {
						rdyfcfs.getFirst(P1);
						while (timestep - fcfsAT > MaxW)// migration 
						{
							mig = S->mig2(P1);
							if (mig && rdyfcfs.listCount())
							{

								removeready(P1);
								if (rdyfcfs.listCount())
									rdyfcfs.getFirst(P1);
								else break;

							}
							else break;
						}
						if (rdyfcfs.listCount() || !(mig))//adding process to run from ready queue
						{
							removeready(P1);
							if (P1->getRT() == -1)
								P1->setRT(timestep - P1->get_AT());
							addrun(P1);
						}

					}
				}
				if (P1->get_CT() - P1->getremtime() == 0)

				{
					iopq->dequeue(iox);
				}

			}
		}
	}
	if (rdyfcfs.listCount() && !getbusy()) {
		rdyfcfs.getFirst(P1);
		while (timestep - fcfsAT > MaxW)// migration 
		{
 			mig = S->mig2(P1);
			if (mig && rdyfcfs.listCount())
			{
				removeready(P1);
				if (rdyfcfs.listCount())
					rdyfcfs.getFirst(P1);
				else break;

			}
			else break;
		}
		if (rdyfcfs.listCount() || !(mig))//adding process to run from ready queue
		{
			removeready(P1);
			if (P1->getRT() == -1)
				P1->setRT(timestep - P1->get_AT());
			addrun(P1);
		}
	}
}

int FCFS::getcountkilled() {
	return killedprocesses.listCount();
}
bool FCFS::Killsig(Scheduler*S, int timestep,int j)
 {
	LinkedQueue<KILLSIG*>* killq = S->getkillq();
	Processes *p1=NULL;
	Processes* px=NULL;
	KILLSIG* k;
	int tokill;
	Processes pdummy;
	Processes* pdel = NULL;


	if (!killq->isEmpty()) {
		killq->peek(k);
		while (k->kill_T < timestep) {
			killq->dequeue(k);
			killq->peek(k);
			if (killq->isEmpty()) {
				break;
			}
		}
		pdummy.setPID(k->kill_ID);
	
		if (k->kill_T == timestep) { //if current timestep, enter
			killq->peek(k); 
			tokill = k->kill_ID;
			
			if (getrun())
			if (tokill == getrun()->getPID()) { //if found in run, removerun and return p1 (return to sched so that it can go to TRM)
				px = removerun();
				killq->dequeue(k);
				S->addtotrm(px);//trm here
				this->ScheduleAlgo(S, timestep);
				killedprocesses.InsertEnd(px);
				return true;
			}
		
			pdel = rdyfcfs.DeleteWReturn(&pdummy); // if not found in run, search and delete from ready
			if (pdel) {
				killq->dequeue(k);
				
				S->addtotrm(pdel);
				this->ScheduleAlgo(S, timestep);
				killedprocesses.InsertEnd(pdel);
				return true;
			}
	



			if (S->searchTrm(&pdummy) || S->searchBlk(&pdummy)) { // if not found in ready or run, search in TRM and BLK

				killq->dequeue(k);
				return false;
			}
			if (j+1 >= S->getnFCFS()) { // if not found in any, and last processor in list, then it is not in system yet, dequeue
				killq->dequeue(k);
				return false;
			}
			
		}
	
	}
	
	return false;
}

void FCFS::printRdy() {
	rdyfcfs.printList();
}

void FCFS::setMaxW(int maxw) {
	MaxW = maxw;
}

void FCFS::setforkprob(int fork)
{
	forkprob = fork;
}

int FCFS::getforkprob()
{
	return forkprob;
}



int FCFS::getnprocess()
{
	return rdyfcfs.listCount();

}

void FCFS::addready(Processes* p1, int ts) {
	
	if (p1) {
		rdyfcfs.InsertEnd(p1);
		fcfsAT = ts;
	}
}

void FCFS::addready(Processes* p1)
{

	if (p1)
	rdyfcfs.InsertEnd(p1);

}


void FCFS::removeready(Processes*& p1)
{
	rdyfcfs.getFirst(p1);
	rdyfcfs.DeleteFirst();	
}

int FCFS::getQFT(int timestep)
{
	Processes* p1;
	int t = 0;
	int n = rdyfcfs.listCount();
	for (int i = 0; i < n; i++)
	{
		rdyfcfs.getFirst(p1);
		t +=p1->getremtime();
		rdyfcfs.InsertEnd(p1);
		rdyfcfs.DeleteFirst();
	}
	return t + timestep;
}

bool FCFS::forkprobabilty()
{
	int r;
	 r = (rand() % 100)+1;
		if (r <= forkprob)
 			return true;
		else
			return false;
	
}

void FCFS::killchildren(Processes* p, Scheduler* S)
{
	if (p->getleftchild())
	{
		killbill(p->getleftchild(), S);
	}
	if (p->getrightchild())
	{
		killbill(p->getrightchild(), S);
	}
}

void FCFS::killbill(Processes* p, Scheduler* S)
{
	Processes* tokill;
	Processes* isrun;
	Processes* isrdy;
	Processes* tosave = p;
	Processes* p2 = p; 

	tokill = p2;
	isrun = getrun();
	if (tokill == isrun)
	{
		removerun();
		tokill->setIsorph(true);
		
		S->addtotrm(tokill);
	}
	else
	{
		for (int i = 0; i < rdyfcfs.listCount(); i++)
		{
			rdyfcfs.getFirst(isrdy);
			if (isrdy == tokill)
			{
				tokill->setIsorph(true);
				S->addtotrm(tokill);
			}
			if (isrdy != tokill)
			{
				rdyfcfs.InsertEnd(isrdy);
			}

			rdyfcfs.DeleteFirst();
		}

	}
	if (tokill->getleftchild())
	{
		killbill(p->getleftchild(), S);
	}
	if (tokill->getrightchild())
	{
		killbill(p->getrightchild(), S);
	}
}




bool FCFS::tostealornottosteal()
{
	Processes* x;
	rdyfcfs.getFirst(x);
	if (x->getparent())
	{
		return false;
	}
	else return true;

}

FCFS::~FCFS()
{

}