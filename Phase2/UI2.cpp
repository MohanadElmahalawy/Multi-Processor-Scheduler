#pragma once
#include "UI2.h"
#include <iostream>
#include<windows.h>  
using namespace std;

UI2::UI2()
{
}

void UI2::Display(Processor** P, int sizeP, LinkedQueue<Processes*>* Blk, LinkedQueue<Processes*>* Trm, int timestep, int &mode)
{
	if (mode ==1 || mode==2) {
		cout << "Current Timestep:" << timestep << endl;
		int nrun = 0;
		
		cout << "----------------------RDY Processes------------------" << endl;
		for (int i = 0; i < sizeP; i++) {

			Pr = P[i];
			cout << "Processor " << i+1 << "[" << Pr->getpstype() << "]:" << Pr->getnprocess() << " RDY:";
			if (Pr->getbusy())
				nrun++;
			Pr->printRdy();
			cout << endl;

		}

		cout << "------------------------BLK Processes---------------------" << endl;
		cout << Blk->listCount() << " BLK: ";
		Blk->printList();
		cout << endl << "--------------------------RUN Processes---------------------" << endl;
		cout << nrun << " RUN: ";
		for (int i = 0; i < sizeP; i++) {

			Pr = P[i];
			Run = Pr->getrun();
			if (Run) {
				cout << Run->getPID() << " (P" << i+1 << ") ,";

			}
		}
		cout << "\b";
		cout << "  ";

		cout << endl << "----------------------------TRM Processes-----------------------" << endl;
		cout << Trm->listCount() << " TRM:";
		Trm->printList();
		cout << endl;
		
		if (mode == 1) {
			system("pause");
		

		}
		else if (mode == 2) {
			Sleep(1000);
		
		}

	}
	if (mode == 3) {
		cout << "Silent Mode...Simulation Starts" << endl;
		mode = -1;
		
	}
	
}

int UI2::SelectMode()
{
	int MODE;
	cout << "Select mode:" << endl << "1 - Interactive Mode" << endl << "2- Step-by-step mode" << endl << "3-Silent Mode" << endl;
	cin >> MODE;
	return MODE;
}

UI2::~UI2()
{
	delete Run;
	delete Pr;
}
