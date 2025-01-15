#include "EDF.h"
#include "Scheduler.h"

EDF::EDF(){

}

void EDF::processoroverheat(Scheduler* S)
{
	Processor* sp;
	Processes* pr;
	if (currentOH == getnoverheat()) {//return to normal settings after overheating timesteps are over
		setoverheat(false);
		settype(4);
	}
	else
		if (currentOH < getnoverheat()) {
			settype(8);
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
			while (rdyedf.listCount())//empty ready list
			{
				rdyedf.dequeue(pr);
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

int EDF::countready()
{
	return rdyedf.listCount();
}

void EDF::ScheduleAlgo(Scheduler* S, int timestep)
{

	Processes* p1 = nullptr;
	Processes* p2 = nullptr;
	if (getbusy())
	{
		p1 = getrun();
	
		//before decrementing, it should check if a new process inside it has a shorter deadline
		int currdeadline = p1->getdeadline();
		int n = rdyedf.listCount();
		for (int i = 0; i < n; i++) { // compares rdy to run, if rdy is shorter substitutes it
			rdyedf.dequeue(p2);
			if (p2->getdeadline() < currdeadline) {
				p1 = removerun();
				if (p2->getRT() == -1)
					p2->setRT(timestep - p2->get_AT());
				addrun(p2);
				rdyedf.enqueue(p1, p1->getdeadline());
				currdeadline = p2->getdeadline();
			}
			else
				rdyedf.enqueue(p2, p2->getdeadline());
		}

		p1->dec_remtime();
		if (p1->getremtime() <= 0)
		{
			p1 = removerun();
			if (timestep < p1->getdeadline())
				S->incbeforedeadline();
			S->addtotrm(p1);
			this->ScheduleAlgo(S, timestep);
			return;
		}
		LinkedQueue<IO*>* iopq = p1->getIO();

		if (iopq->listCount())
		{
			IO* iop;
			iopq->peek(iop);
			if (iop->IO_R == (p1->get_CT() - p1->getRT()) && p1->getremtime() != 0)
			{
				iopq->dequeue(iop);
				p1 = removerun();
				S->handlingIO(p1, iop->IO_D);
				if (rdyedf.listCount() && !getbusy())
				{
					rdyedf.dequeue(p1);
					if (p1->getRT() == -1)
						p1->setRT(timestep - p1->get_AT());
					addrun(p1);
				}
				else
					return;

				//call function from run to blk by scheduler

			}
			if (!(p1->get_CT() - p1->getremtime()))

			{
				iopq->dequeue(iop);

			}
			// ^^ IO Handling
		}
	}
	if (rdyedf.listCount() && !getbusy())
	{
		rdyedf.dequeue(p1);
		if (p1->getRT() == -1)
			p1->setRT(timestep - p1->get_AT());
		addrun(p1);
	}
	

}

void EDF::printRdy()
{
	rdyedf.printList();
}

int EDF::getnprocess()
{
	return rdyedf.listCount();
}

void EDF::addready(Processes* p1)
{

	if (p1)
		rdyedf.enqueue(p1, p1->getdeadline());

}
void EDF::addready(Processes* p1,int ts)
{

	if (p1) {
		rdyedf.enqueue(p1, p1->getdeadline());
		edfAT = ts;
	}
}

void EDF::removeready(Processes*& p1)
{
	rdyedf.dequeue(p1);
}

int EDF::getQFT(int timestep)
{
	Processes* p1;
	LinkedQueue<Processes*> temp;
	int t = 0;
	int n = rdyedf.listCount();
	for (int i = 0; i < n; i++)
	{
		rdyedf.dequeue(p1);
		t += p1->getremtime();
		temp.enqueue(p1);
	}

	for (int i = 0; i < n; i++)
	{
		temp.dequeue(p1);
		rdyedf.enqueue(p1, p1->getdeadline());
	}

	return t + timestep;
}



EDF::~EDF(){

}