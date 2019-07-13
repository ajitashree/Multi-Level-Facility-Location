
#include "TWOLevelFL.h"
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <thread>
using namespace std;

int main(int argc, char const *argv[])
{
	double value;
	long N, M1, M2;

	cin >> N >> M1 >> M2;

	double *facilityCost1 = new double[M1];
	double *facilityCost2 = new double[M2];
	double **connectionCost1 = new double*[N];
	double **connectionCost2 = new double*[M1];

	for (long m1 = 0; m1 < M1; m1++)
	{
		cin >> value;
		facilityCost1[m1] = value;
	}

	for (long m2 = 0; m2 < M2; m2++)
	{
		cin >> value;
		facilityCost2[m2] = value;
	}

	for (long n = 0; n < N; n++)
	{
		connectionCost1[n] = new double[M1];
		for (long m1 = 0; m1 < M1; m1++)
		{
			cin >> value;
			connectionCost1[n][m1] = value;
		}
	}
	for (long m1 = 0; m1 < M1; m1++)
	{
		connectionCost2[m1] = new double[M2];
		for (long m2 = 0; m2 < M2; m2++)
		{
			cin >> value;
			connectionCost2[m1][m2] = value;
		}
	}

	TWOLevelFL *tFL = new TWOLevelFL(N, M1, M2);
	tFL->setFacilityCost1(facilityCost1);
	tFL->setFacilityCost2(facilityCost2);
	tFL->setConnectionCost1(connectionCost1);
	tFL->setConnectionCost2(connectionCost2);
	tFL->initFeasibleSol();
	//cout << "Before Local Search" << endl;
	//tFL->printDetails();
	//cout << "Cost is : "<< tFL->computeCost(tFL->facilityAssignL1, tFL->facilityAssignL2, tFL->isFacOpenL1);
	std::clock_t c_start = std::clock();
	tFL->localSearch();
	std::clock_t c_end = std::clock();
	//cout << "After Local Search" << endl;
	//tFL->printDetails();
	//cout << "Cost is : "<< tFL->computeCost(tFL->facilityAssignL1, tFL->facilityAssignL2, tFL->isFacOpenL1) << endl;
	cout<< N<<" "<<M1<<" "<<M2<<" : "<< std::setprecision(5)<<tFL->computeCost(tFL->facilityAssignL1, tFL->facilityAssignL2, tFL->isFacOpenL1)<<" : "<<1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n";
	return 0;
}