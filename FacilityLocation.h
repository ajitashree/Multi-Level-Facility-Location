
#ifndef FACILITYLOCATION_H
#define FACILITYLOCATION_H

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;

class FacilityLocation
{
	long M, N; // M : No of Facilities, N: No of Clients
	double cost, epsilon;

	double *facilityCost;
	double **connectionCost; // N * M Matrix
public:

	bool *isFacilityOpen;
	long *facilityAssign;
	

	double computeCost();
	void printDetails();
	void initFeasibleSol();
	void initAssignment(bool *, long *);
	long findNearestOpenFacility(long);

	bool addMove(long);
	bool deleteMove(long);
	bool swapMove(long, long);
	void localSearch();

	void setfacilityCost(double*);
	void setconnectionCost(double**);

	FacilityLocation(long M, long N);
	~FacilityLocation();

};

void FacilityLocation::initAssignment(bool *_isFacilityOpen, long *_facilityAssign)
{
	for (long m = 0; m < M; m++)
	{
		isFacilityOpen[m] = _isFacilityOpen[m];
	}
	for(long n = 0; n < N; n++)
	{
		facilityAssign[n] = _facilityAssign[n];
	}
}

void FacilityLocation::printDetails()
{
	cout << "The facilities cost are as follows :\n" << endl;
	for (long m = 0; m < M; m++)
	{
		cout << facilityCost[m] << " ";
	}
	cout << endl;

	cout << "Open Facilities :\n" << endl;
	for (long m = 0; m < M; m++)
	{
		cout << isFacilityOpen[m] << " ";
	}
	cout << endl;

	cout << "Connection cost matrix :\n" << endl;
	for (long n = 0; n < N; n++)
	{
		for (long m = 0; m < M; m++)
		{
			cout << connectionCost[n][m] << " ";
		}
		cout << endl;
	}

	cout << "Client Assignment :\n" << endl;
	for (long m = 0; m < N; m++)
	{
		cout << facilityAssign[m] << " ";
	}
	cout << endl;

}

void FacilityLocation::setfacilityCost(double* _facilityCost)
{
	for (long m = 0; m < M; m++)
	{
		facilityCost[m] = _facilityCost[m];
	}
}

void FacilityLocation::setconnectionCost(double** _connectionCost)
{
	for(long n = 0; n < N; n++)
	{
		for (long m = 0; m < M; m++)
		{
			connectionCost[n][m] = _connectionCost[n][m];
		}
	}
}

double FacilityLocation::computeCost()
{
	cost = 0.0;

	for (long n = 0; n < N; n++)
	{
		cost += connectionCost[n][facilityAssign[n]];
	}
	for (long m = 0; m < M; m++)
	{
		if (isFacilityOpen[m]) 
			cost += facilityCost[m];
	}
	return cost;
}
void FacilityLocation::initFeasibleSol()
{
	long r;


	for (long n = 0; n < N; n++)
	{
		/*
		if (n % 2 == 0)
			facilityAssign[n] = 0;
		else
			facilityAssign[n] = 1;*/

		facilityAssign[n] = 0;
		/*
		r = rand() % M;

		facilityAssign[n] = r;
		isFacilityOpen[r] = true;*/
	}
	isFacilityOpen[0]=true;
	//isFacilityOpen[1]=true;
}

FacilityLocation::FacilityLocation(long M, long N)
{
	cost = 0.0;
	epsilon = 0.9999;
	this->M = M ;
	this->N = N ;
	facilityCost = new double[M];
	facilityAssign = new long[N];
	isFacilityOpen = new bool[M];
	connectionCost = new double*[N];

	for (long n = 0; n < N; n++)
	{
		connectionCost[n] = new double[M];
	}
	for(long m = 0; m < M; m++)
	{
		isFacilityOpen[m] = false;
	}

}

bool FacilityLocation::addMove(long facId)
{
	long oldAssign = 0;
	long *tempAssign = new long[N];

	double cost = computeCost();
	double oldCost = 0, newCost = 0;
	double savings = -facilityCost[facId];

	for (long i = 0; i < N; i++)
	{
		oldAssign = facilityAssign[i];
		oldCost = connectionCost[i][oldAssign];
		newCost = connectionCost[i][facId];

		if( oldCost > newCost)
		{
			tempAssign[i] = facId;
			savings += (oldCost - newCost); 
		}
		else
		{
			tempAssign[i] = oldAssign;
		}
	}
	bool flag = false;
	if(cost - savings <= epsilon*cost) // improving (savings > 0)
	{
		for (long i = 0; i < N; i++)
			facilityAssign[i] = tempAssign[i];

		isFacilityOpen[facId] = true;
		flag = true;
	}
	delete tempAssign;
	return flag;
}

long FacilityLocation::findNearestOpenFacility(long cId)
{	
	long i, fId;
	bool openFacFound =false ;
	for (i = 0; i < M; i++)
	{
		if(isFacilityOpen[i])
		{
			fId = i;
			openFacFound = true ;
			break ;
		}

	}
	if(!openFacFound)
		return -1 ;
	for (; i < M; i++)
	{
		if(isFacilityOpen[i]  && (connectionCost[cId][i] < connectionCost[cId][fId]))
			fId = i;
	}
	return fId;
}

bool FacilityLocation::deleteMove(long facId)
{
	long newAssign = 0;
	long *tempAssign = new long[N];

	double cost = computeCost();
	double oldCost = 0, newCost = 0;
	double savings = facilityCost[facId];

	isFacilityOpen[facId] = false;

	for (long n = 0; n < N; n++)
	{
		if(facilityAssign[n] == facId)
		{
			newAssign = findNearestOpenFacility(n);
			if(newAssign == -1) 
			{
				isFacilityOpen[facId] = true;
				return false;
			}
			oldCost = connectionCost[n][facId];
			newCost = connectionCost[n][newAssign];
			savings += oldCost - newCost;
			tempAssign[n] = newAssign;
		}
		else
		{
			tempAssign[n] = facilityAssign[n];
		}
	}
	bool flag = false;
	if(cost - savings <= epsilon*cost)  //(savings > 0)
	{
		for (long i = 0; i < N; i++)
			facilityAssign[i] = tempAssign[i];
		flag =  true;
	}
	else
	{
		isFacilityOpen[facId] = true;
	}
	delete tempAssign;
	return flag;
}

// 1 is getting closed and 2 is getting open
bool FacilityLocation::swapMove(long fId1, long fId2)
{
	isFacilityOpen[fId1] = false;
	isFacilityOpen[fId2] = true;

	double cost = computeCost();
	long newAssign = 0;
	long *tempAssign = new long[N];

	double oldCost = 0, newCost = 0;
	double savings = facilityCost[fId1] - facilityCost[fId2];

	for (long n = 0; n < N; n++)
	{

		newAssign = findNearestOpenFacility(n);
		oldCost = connectionCost[n][facilityAssign[n]];
		newCost = connectionCost[n][newAssign];

		savings += oldCost - newCost;
		tempAssign[n] = newAssign;
	}
	bool flag = false;
	if(cost - savings <= epsilon*cost)
	{
		for (long i = 0; i < N; i++)
			facilityAssign[i] = tempAssign[i];
		flag = true;
	}
	else
	{
		isFacilityOpen[fId1] = true;
		isFacilityOpen[fId2] = false;
	}
	delete tempAssign;
	return flag;
}

void FacilityLocation::localSearch()
{
	while(true)
	{
			bool add_move_done = false;
			bool delete_move_done = false;
			bool swap_move_done = false;


			for (long m = 0; m < M; m++)
			{
				if(!isFacilityOpen[m])
				{
					add_move_done = addMove(m)||add_move_done;
				}
			}
			for(int i = 0; i < M; i++)
			{
				if(isFacilityOpen[i])
					delete_move_done = deleteMove(i)||delete_move_done;
			}
		
			for(int i = 0; i < M; i++)
			{
				for(int j = 0; j < M; j++)
				{
					if(i!=j)
					{
						if(isFacilityOpen[i] && !isFacilityOpen[j])
							swap_move_done = swapMove(i,j) || swap_move_done;
					}
				}
			}
					
			if(!(add_move_done|delete_move_done|swap_move_done))
				break;
	}
}

FacilityLocation::~FacilityLocation()
{
	delete facilityCost;
	delete facilityAssign;
	delete isFacilityOpen;
	for (long n = 0; n < N; n++)
	{
		delete connectionCost[n];
	}
	delete connectionCost;
}
#endif