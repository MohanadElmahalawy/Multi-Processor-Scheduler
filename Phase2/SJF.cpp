#include "SJF.h"
#include "Scheduler.h"

SJF::SJF()
{

}
void SJF::processoroverheat(Scheduler* S) 
{
Processor* sp;
Processes* pr;
if (currentOH == getnoverheat()) {//return to normal settins after overheating timesteps are over
	setoverheat(false);
	settype(2);
}
else
	if (currentOH < getnoverheat()) {
		settype(6);
		pr = getrun();

		if (pr)//remove running process
		{
			removerun();
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
		while (rdysjf.listCount())//empty ready queue
		{
			rdysjf.dequeue(pr);
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
		currentOH++;
	}
}
int SJF::countready()
{
	return rdysjf.listCount();
}
void SJF::ScheduleAlgo(Scheduler* S,int timestep)
{

	Processes* p1=nullptr;
	if (getbusy())
	{
		p1 = getrun();
		p1->dec_remtime();
		if (p1->getremtime() <= 0)//terminating process after it finishes its CT
		{
			p1 = removerun();
			S->addtotrm(p1);
			this->ScheduleAlgo(S, timestep);
			return;
		}
		LinkedQueue<IO*>* iopq = p1->getIO();
		if (iopq->listCount())
		{
			IO* iop;
			iopq->peek(iop);
			if (iop->IO_R == (p1->get_CT() - p1->getremtime()) && p1->getremtime() != 0)
			{
				iopq->dequeue(iop);
				p1 = removerun();
				S->handlingIO(p1, iop->IO_D);
				if (rdysjf.listCount() && !getbusy())
				{
					rdysjf.dequeue(p1);
					if (p1->getRT() == -1)
						p1->setRT(timestep - p1->get_AT());
					addrun(p1);
				}
				else
					return;

				//call function from run to blk by scheduler

			}
			if (!(p1->getIO()-p1->getremtime()))
			{
				iopq->dequeue(iop);

			}



		}
	}
	if (rdysjf.listCount() && !getbusy())//adding process to run from ready queue
	{
		rdysjf.dequeue(p1);
		if (p1->getRT() == -1)
			p1->setRT(timestep - p1->get_AT());
		addrun(p1);
	}
	
   
}
void SJF::printRdy() 
{
	rdysjf.printList();
}

int SJF::getnprocess()
{
	return rdysjf.listCount();

}

void SJF::addready(Processes* p1)
{

	if (p1)
	rdysjf.enqueue(p1,p1->getremtime());


}
void SJF::addready(Processes* p1,int ts)
{

	if (p1) {
		rdysjf.enqueue(p1, p1->getremtime());
		sjfAT = ts;
	}

}

void SJF::removeready(Processes*& p1)
{

	rdysjf.dequeue(p1);

}

int SJF::getQFT(int timestep)
{
	Processes *p1;
	LinkedQueue<Processes*> temp;
	int t=0;
	int n = rdysjf.listCount();
	for (int i = 0; i < n; i++)
	{
		rdysjf.dequeue(p1);
		t+=p1->getremtime();
		temp.enqueue(p1);
	}
	
	for (int i = 0; i < n; i++)
	{
		temp.dequeue(p1);
		rdysjf.enqueue(p1,p1->getremtime());
	}

	return t + timestep;
}




SJF::~SJF()
{

}
