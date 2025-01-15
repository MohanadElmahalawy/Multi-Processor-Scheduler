#pragma once
#include "Processor.h"
#include "Processes.h"
#include "LinkedQueue.h"
#include "LinkedList.h"
#include "KILLSIG.h"
using namespace std;

class Scheduler;

class FCFS : public Processor
{
private:

    LinkedList<Processes*> rdyfcfs;
    int MaxW; // process migration 
    int forkprob;
    LinkedList<Processes*> killedprocesses;
    int currentOH=0;
    int fcfsAT = 0;


public:
    FCFS();
    int countready();
    void processoroverheat(Scheduler* S); //function that handles processor overheating

    void ScheduleAlgo(Scheduler* S, int timestep);

    int getcountkilled();

    bool Killsig(Scheduler* S,int timestep,int j);
    //Killsig function called in scheduler. Returns process to kill(send to trm) after removing it from ready/run.
    // If not found, returns null and ignores killsignal
    void printRdy();

    void setMaxW(int maxw);
    void setforkprob(int fork);
    int getforkprob();
    
    int getnprocess();
    void addready(Processes* p1,int ts);
    void addready(Processes* p1);

    void removeready(Processes*& p1);

    int getQFT(int timestep);// gets the expected finishing time of the queue

    bool forkprobabilty();//returns wether or not to fork based on the probability

    void killchildren(Processes* p, Scheduler* S);//kill all children of the given process

    void killbill(Processes* p, Scheduler* S);//used in killchildren function to avoid code repetition 

    bool tostealornottosteal();//checks wether the first process is forked or not

    ~FCFS();



};


