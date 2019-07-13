/*
TODO:
*/

#ifndef TWOLevelFL_H
#define TWOLevelFL_H
#include "FacilityLocation.h"
#include <set>
using namespace std;


class TWOLevelFL
{
	double epsilon;
	long N, M1, M2;
	double *facilityCost1;
	double *facilityCost2;
	double **connectionCost1;  // N*M1
	double **connectionCost2;  // M1*M2


public:

	bool *isFacOpenL1;
	bool *isFacOpenL2;
	long *facilityAssignL1;
	long *facilityAssignL2;

	void clearFac();
	double computeCost(long *, long *, bool *);
	void initFeasibleSol();
	void printDetails();

	void setFacilityCost1(double *);
	void setFacilityCost2(double *);
	void setConnectionCost1(double **);
	void setConnectionCost2(double **);

	bool addMove(long);
	bool deleteMove(long);
	void localSearch();

	long findnearest(long);
	long findnearestL2(long);
	void updateAssignment(long *, long *, bool *);
	TWOLevelFL(long, long, long);
	~TWOLevelFL();
};

void TWOLevelFL::clearFac()
{
	set<long> fl1, fl2;
	for (long m = 0; m < M1; m++)
	{
		isFacOpenL1[m] = false;
	}
	for (long m = 0; m < M2; m++)
	{
		isFacOpenL2[m] = false;
	}
	for(long n = 0; n < N; n++)
	{
		long fid1 = facilityAssignL1[n];
		long fid2 = facilityAssignL2[fid1];
		fl1.insert(fid1);
		fl2.insert(fid2);
	}
	for (auto s : fl1)
	{
		isFacOpenL1[s] = true;
	}

	for (auto s : fl2)
	{
		isFacOpenL2[s] = true;
	}
}

void TWOLevelFL::printDetails()
{
	cout << "Open Facilities at Level 2:" << endl;
	for(long m2 = 0; m2 < M2; m2++)
	{
		cout << "Id : " << m2 << " :: " << isFacOpenL2[m2] << endl;
	}

	cout << "Open Facilities at Level 1:" << endl;
	for(long m1 = 0; m1 < M1; m1++)
	{
		cout << "Id : " << m1 << " :: " << isFacOpenL1[m1] << endl;
	}

	cout << "Assignment of Faciltiies : " << endl;

	for (long m1 = 0; m1 < M1; m1++)
	{
		cout << "Id : " << m1 << " :: " << facilityAssignL2[m1] << endl;
	}

	cout << "Clients Assignment " << endl;
	for(long n = 0; n < N; n++)
	{
		cout << "Id : " << n << " :: " << facilityAssignL1[n] << endl;
	}
}
long TWOLevelFL::findnearest(long cId)
{
	long i, fId;
	bool openFacFound = false ;
	for (i = 0; i < M1; i++)
	{
		if(isFacOpenL1[i])
		{
			fId = i;
			openFacFound = true ;
			break ;
		}

	}
	if(!openFacFound)
		return -1 ;

	for (; i < M1; i++)
	{
		if (isFacOpenL1[i])
		{
			double oldCost = connectionCost1[cId][i] + connectionCost2[i][facilityAssignL2[i]];
			double newCost = connectionCost1[cId][fId] + connectionCost2[fId][facilityAssignL2[fId]];

			if(oldCost < newCost)
				fId = i;
		}
	}
	return fId;
}

long TWOLevelFL::findnearestL2(long id)
{
	long i, fId;
	bool openFacFound = false ;
	for (i = 0; i < M2; i++)
	{
		if(isFacOpenL2[i])
		{
			fId = i;
			openFacFound = true ;
			break ;
		}

	}
	if(!openFacFound)
		return -1 ;

	for (; i < M2; i++)
	{
		if (isFacOpenL2[i])
		{
			double oldCost = connectionCost2[id][fId];
			double newCost = connectionCost2[id][i];

			if(oldCost > newCost)
				fId = i;
		}
	}
	return fId;
}

bool TWOLevelFL::addMove(long fId)
{

	double oldCost = computeCost(facilityAssignL1, facilityAssignL2, isFacOpenL1);
	isFacOpenL2[fId] = true;

	long *_facilityAssignL2 = new long[M1];
	long *_facilityAssignL1 = new long[N];
	bool *_isFacOpenL1 = new bool[M1];
	double *_facilityCost1 = new double[M1];
	double **cCost = new double*[N];
		
	for (long m1 = 0; m1 < M1; m1++)
	{
		_facilityAssignL2[m1] = findnearestL2(m1);
		_facilityCost1[m1] = (isFacOpenL1[m1]) ? 0:facilityCost1[m1];
	}
	for (long n = 0; n < N; n++)
	{
		cCost[n] = new double[M1];
		for (long m1 = 0; m1 < M1; m1++)
		{
			cCost[n][m1] = connectionCost1[n][m1] + connectionCost2[m1][_facilityAssignL2[m1]];
		}
	}

	FacilityLocation *fl = new FacilityLocation(M1, N);
	fl->setfacilityCost(_facilityCost1);
	fl->setconnectionCost(cCost);
	fl->initFeasibleSol();
	//fl->initAssignment(isFacOpenL1, facilityAssignL1);
	fl->localSearch();

	for (long j = 0; j < N; j++)
	{
		long fId = fl->facilityAssign[j];
		_facilityAssignL1[j] = fId;
	}

	for(long m1 = 0; m1 < M1; m1++)
	{
		_isFacOpenL1[m1] = fl->isFacilityOpen[m1];
	}

	bool flag = false;
	double newCost = computeCost(_facilityAssignL1, _facilityAssignL2, _isFacOpenL1);

	if(newCost < epsilon*oldCost)
	{
		updateAssignment(_facilityAssignL1, _facilityAssignL2, _isFacOpenL1);
		flag = true;
	}
	else
	{
		isFacOpenL2[fId] = false;
	}
	delete _facilityAssignL2;
	delete _facilityAssignL1;
	delete _isFacOpenL1;
	delete _facilityCost1;
	for(long n = 0; n < N; n++)
	{
		delete cCost[n];
	}
	delete cCost;
	return flag;
}

bool TWOLevelFL::deleteMove(long fId)
{
	long f1,f2, assign, mcount = 0;
	double oldCost = 0.0, newCost = 0.0;
	double savings = facilityCost2[fId];
	double cost = computeCost(facilityAssignL1, facilityAssignL2, isFacOpenL1);

	isFacOpenL2[fId] = false;

	long *facilityAssign = new long[N];
	for (long m1 = 0; m1 < M1; m1++)
	{
		if(facilityAssignL2[m1] == fId && isFacOpenL1[m1])
		{
			mcount++;
			isFacOpenL1[m1] = false;
			savings += facilityCost1[m1];
		}
	}	
	if(mcount == 0) return true;

	for (long n = 0; n < N; n++)
	{
		f1 = facilityAssignL1[n];
		f2 = facilityAssignL2[f1];
		facilityAssign[n] = f1;

		if(f2 == fId)
		{
			oldCost = connectionCost1[n][f1] + connectionCost2[f1][f2];
			assign = findnearest(n);

			if(assign == -1)
			{
				isFacOpenL2[fId] = true;
				for(long m1 = 0; m1 < M1; m1++)
				{
					if(facilityAssignL2[m1] == fId)
						isFacOpenL1[m1] = true;
				}
				return false;
			}
			else
			{
				facilityAssign[n] = assign;
				newCost = connectionCost1[n][assign] + connectionCost2[assign][facilityAssignL2[assign]];
				savings += (oldCost - newCost);
			}
		}
	}
	bool flag = false;
	if ((cost - savings) <= epsilon*cost)          
	{
		for (long n = 0; n < N; n++)
		{
			facilityAssignL1[n] = facilityAssign[n];
		}
		flag = true;
	}
	else
	{
		isFacOpenL2[fId] = true;
		for (long m1 = 0; m1 < M1; m1++)
		{
			if(facilityAssignL2[m1] == fId)
				isFacOpenL1[m1] = true;
		}
	}
	delete facilityAssign;
	return flag;

}

void TWOLevelFL::localSearch()
{

	while(true)
	{
			bool add_move_done = false;
			bool delete_move_done = false;
		
			
			for (long m = 0; m < M2; m++)
			{
				if(!isFacOpenL2[m])
				{
					add_move_done = addMove(m)||add_move_done;
					clearFac();
				}
			}
			
			for(long i = 0; i < M2; i++)
			{
				if(isFacOpenL2[i])
				{
					delete_move_done = deleteMove(i)||delete_move_done;
					clearFac();
				}

			}

			if(!(add_move_done|delete_move_done))
				break;
	}
}

void TWOLevelFL::initFeasibleSol()
{
	isFacOpenL1[0] = true;
	isFacOpenL2[0] = true;

	for (long m1 = 0; m1 < M1; m1++)
	{
		facilityAssignL2[m1] = 0;
	}

	for (long n = 0; n < N; n++)
	{
		facilityAssignL1[n] = 0;
	}
}

void TWOLevelFL::updateAssignment(long *_facilityAssignL1, long *_facilityAssignL2, bool *_isFacOpenL1)
{
	for (long m1 = 0; m1 < M1; m1++)
	{
		isFacOpenL1[m1] = _isFacOpenL1[m1];
		facilityAssignL2[m1] = _facilityAssignL2[m1];
	}

	for (long n = 0; n < N; n++)
	{
		facilityAssignL1[n] = _facilityAssignL1[n];
	}
}

double TWOLevelFL::computeCost(long *_facilityAssignL1, long *_facilityAssignL2, bool *_isFacOpenL1)
{
	double cost = 0.0;
	set<long> fl1, fl2;

	for(long n = 0; n < N; n++)
	{
		long fid1 = _facilityAssignL1[n];
		long fid2 = _facilityAssignL2[fid1];
		cost += connectionCost1[n][fid1] + connectionCost2[fid1][fid2];
		fl1.insert(fid1);
		fl2.insert(fid2);
	}
	for (auto s : fl1)
	{
		cost += facilityCost1[s];
	}

	for (auto s : fl2)
	{
		cost += facilityCost2[s];
	}
	return cost;
}

void TWOLevelFL::setFacilityCost1(double *_facilityCost1)
{
	for (long m = 0; m < M1; m++)
	{
		facilityCost1[m] = _facilityCost1[m];
	}
}

void TWOLevelFL::setFacilityCost2(double *_facilityCost2)
{
	for (long m = 0; m < M2; m++)
	{
		facilityCost2[m] = _facilityCost2[m];
	}
}

void TWOLevelFL::setConnectionCost1(double **_connectionCost1)
{
	for (long n = 0; n < N; n++)
	{
		for (long m = 0; m < M1; m++)
		{
			connectionCost1[n][m] = _connectionCost1[n][m];
		}
	}
}

void TWOLevelFL::setConnectionCost2(double **_connectionCost2)
{
	for (long m1 = 0; m1 < M1; m1++)
	{
		for (long m2 = 0; m2 < M2; m2++)
		{
			connectionCost2[m1][m2] = _connectionCost2[m1][m2];
		}
	}
}

TWOLevelFL::TWOLevelFL(long _N, long _M1, long _M2)
{
	N = _N;
	M1 = _M1;
	M2 = _M2;

	epsilon = 0.9999;
	facilityCost1 = new double[M1];
	facilityCost2 = new double[M2];
	connectionCost1 = new double*[N];
	connectionCost2 = new double*[M1];

	isFacOpenL2 = new bool[M2];
	isFacOpenL1 = new bool[M1];
	facilityAssignL1 = new long[N];
	facilityAssignL2 = new long[M1];

	for (long n = 0; n < N; n++)
	{
		connectionCost1[n] = new double[M1];
	}
	for (long m = 0; m < M1; m++)
	{
		connectionCost2[m] = new double[M2];
	}
}

TWOLevelFL::~TWOLevelFL()
{
	delete facilityCost2;
	delete facilityCost1;

	for (long m = 0; m < M1; m++)
		delete connectionCost2[m];

	for (long n = 0; n < N; n++)
		delete connectionCost1[n];

	delete connectionCost1;
	delete connectionCost2;
}
#endif