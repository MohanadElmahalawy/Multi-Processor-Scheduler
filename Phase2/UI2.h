#pragma once
#include "Processor.h"
#include "LinkedList.h"
#include "LinkedQueue.h"
//esta3mel class gahez kda 3shan mate3melsh loop include w f nafs el wa2t ykon 3aref en fi haga esmaha
//processor (forward declaration)
class UI2
{
private:
	int timestep;
	int mode;
	Processes* Run;
	Processor* Pr;
public:
	UI2();
	void Display(Processor **P,int sizeP, LinkedQueue<Processes*> *Blk, LinkedQueue<Processes*> *Trm, int timestep,int &mode);
	int SelectMode();
	~UI2();
};

