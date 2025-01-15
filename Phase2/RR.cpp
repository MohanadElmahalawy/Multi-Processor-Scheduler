#include "RR.h"
#include "Scheduler.h"

RR::RR()
{

}



int RR::gettsl()
{
	return timeslice;
}

void RR::printRdy() {
	rdyrr.printList();
}

int RR::countRdy() {
	return rdyrr.listCount();
}

void RR::settsl(int tsl)
{
	timeslice = tsl;
}

int RR::countready()
{
	return rdyrr.listCount();
}

void RR::setrtf(int rtf)
{
	RTF = rtf;
}

Processes* RR::migration(Processes* x) {

	rdyrr.peek(x);
	if (x->get_CT() < RTF) {
		return x;
	}
}
void RR::processoroverheat(Scheduler* S) 
{
	Processor* sp;
	Processes* pr;
	if (currentOH == getnoverheat()) {//return to normal settins after overheating timesteps are over
		setoverheat(false);
		settype(3);
	}
	else
		if (currentOH < getnoverheat()) {
			settype(7);
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
			while (rdyrr.listCount())//empty ready queue
			{
				rdyrr.dequeue(pr);
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
void RR::ScheduleAlgo(Scheduler* S, int timestep)
{
	Processor* pr1;
	Processes* p1;
	Processes* p2;
	bool mig = false;
	if (getbusy())
	{

		IO* dtime = NULL;
		p1 = getrun();
		

		if (currentslice != timeslice) //checked, correct
		{
			p1->dec_remtime();
			currentslice++;
			if (!p1->getremtime())
			{
				p1 = removerun();
				currentslice = 0;
				S->addtotrm(p1);
				this->ScheduleAlgo(S, timestep);
				return;
			}

		}
		if (currentslice == timeslice)
		{
			p1 = removerun();
			if (p1->getremtime())
			{

				addready(p1);
				rdyrr.peek(p1);

				while (p1->getremtime() < RTF && p1->getremtime() != 0) ////////////////////////// migration 
				{
					mig = S->mig1(p1);
					if (mig && rdyrr.listCount())
					{

						removeready(p1);
						if (rdyrr.listCount())
							rdyrr.peek(p1);
						else break;
					}
					else break;


				}

				if (!rdyrr.isEmpty())//adding process to run from ready queue
				{
					removeready(p1);
					if (p1->getRT() == -1)
						p1->setRT(timestep - p1->get_AT());
					addrun(p1);
				}

			}
			if (!p1->getremtime())
			{
				S->addtotrm(p1);
				this->ScheduleAlgo(S, timestep);
				return;
			}


			currentslice = 0;



		}


		LinkedQueue<IO*>* iopq = p1->getIO();
		if (iopq->listCount())
		{
			iopq->peek(dtime);
			if (dtime->IO_R == (p1->get_CT() - p1->getremtime()) && p1->getremtime() != 0)
			{
				iopq->dequeue(dtime);
				p1 = removerun();
				S->handlingIO(p1, dtime->IO_D);
				currentslice = 0;
				if (rdyrr.listCount() && !getbusy())
				{
					rdyrr.peek(p1);
					while (p1->getremtime() < RTF && p1->getremtime() != 0) ////////////////////////// migration 
					{
						mig = S->mig1(p1);
						if (mig && rdyrr.listCount())
						{

							removeready(p1);
							if (rdyrr.listCount())
								rdyrr.peek(p1);
							else break;
						}
						else break;


					}

					if (!rdyrr.isEmpty() || !(mig))//adding process to run from ready queue
					{
						removeready(p1);
						if (p1->getRT() == -1)
							p1->setRT(timestep - p1->get_AT());
						addrun(p1);
					}

				}
			}
			if (!(p1->getIO() - p1->getremtime()))
			{
				iopq->dequeue(dtime);

			}
		}
	}
	if (rdyrr.listCount() && !getbusy())
	{
		rdyrr.peek(p1);
		while (p1->getremtime() < RTF && p1->getremtime() != 0) ////////////////////////// migration 
		{
			mig = S->mig1(p1);
			if (mig && rdyrr.listCount())
			{
				removeready(p1);
				if (rdyrr.listCount())
					rdyrr.peek(p1);
				else break;

			}
			else break;
		}
		if (!rdyrr.isEmpty() || !(mig))//adding process to run from ready queue
		{
			removeready(p1);
			if (p1->getRT() == -1)
				p1->setRT(timestep - p1->get_AT());
			addrun(p1);
		}
		if (rdyrr.listCount() && !getbusy()) {
			rdyrr.dequeue(p1);
			if (p1->getRT() == -1)
				p1->setRT(timestep - p1->get_AT());
			addrun(p1);
		}
	}
	
}

int RR::getnprocess()
{
	return rdyrr.listCount();

}

void RR::addready(Processes* p1)
{
	if (p1)
		rdyrr.enqueue(p1);

}
void RR::addready(Processes* p1,int ts)
{

	if (p1) {
		rdyrr.enqueue(p1);
		rrAT = ts;
	}
}

void RR::removeready(Processes*& p1)
{

	rdyrr.dequeue(p1);

}

int RR::getQFT(int timestep)
{
	Processes* p1;
	LinkedQueue<Processes*> temp;
	int t = 0;
	int n = rdyrr.listCount();
	for (int i = 0; i < n; i++)
	{
		rdyrr.dequeue(p1);
		t += p1->getremtime();
		temp.enqueue(p1);
	}

	for (int i = 0; i < n; i++)
	{
		temp.dequeue(p1);
		rdyrr.enqueue(p1);
	}

	return t + timestep;
}




RR::~RR()
{

}
