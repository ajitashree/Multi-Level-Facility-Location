/*
Test Case generator generates the input for the Multi-level Facility Location implementation
*/

#include<iostream>
#include<sstream>
#include<cstdio>
#include<cmath>
#include<string>
#include<cstring>
#include<ctime>

using namespace std;

int main(int argc,char ** argv)
{

	int N,M,K;
	N = atoi(argv[1]);
	M = atoi(argv[2]);
	K = atoi(argv[3]);

	long arr1[M];
	long double arr2[K];
	long double cor[M][2];
	long double cor2[K][2];
	long double connection_b_fac[M][K];
	long double connection_b_cf[N][M];

	for(int i = 0; i < K; i++)
	{
		cor2[i][0] = rand() % 1000;
		cor2[i][1] = rand() % 1000;
		arr2[i] = rand() % 1000;
	}

	for(int i = 0; i < M; i++)
	{
		cor[i][0] = rand() % 1000;
		cor[i][1] = rand() % 1000;
		arr1[i] = rand() % 1000;
	}

	for(int i = 0; i < M; i++)
		for(int j = 0; j < K; j++)
			connection_b_fac[i][j] = sqrt((cor2[j][0] - cor[i][0]) * (cor2[j][0] - cor[i][0]) + (cor2[j][1] - cor[i][1]) * (cor2[j][1] - cor[i][1]));

	for(int i = 0; i < N; i++)\
	{
			int x = rand() % 1000;
			int y = rand() % 1000;
			for(int j = 0; j < M; j++)
				connection_b_cf[i][j] = sqrt((cor[j][0] - x) * (cor[j][0] - x) + (cor[j][1] - y) * (cor[j][1] - y));
	}

	int ii = 0;
	int amm[] = {000, 001, 002, 10, 11, 12, 20, 21, 22
				  ,100, 101, 102, 110, 111, 112, 120, 121, 122
				  ,200, 201, 202, 210, 211, 212, 220, 221, 222};

	
	long double valu[] = {.01, 1.0, 1000};
	
	for(ii = 0; ii < 27; ii++)
	{
		cout << amm[ii] << endl;
	}

	for(ii = 0; ii < 27; ii++)
	{
		string s;
		char tab2[1024];
		s = "input_files/" + to_string(N) + "_" + to_string(M) + "_" + to_string(K) + "_" + to_string(amm[ii] / 100) + to_string((amm[ii] % 100) / 10) + to_string(amm[ii] % 10) + ".txt";
		
		strcpy(tab2, s.c_str());
		freopen (tab2, "w", stdout);

		cout << N << " " << M << " " << K << endl;

		for(int i = 0; i < M; i++)
			cout << arr1[i] << " ";
			cout << endl;

		for(int i = 0; i < K; i++)
			cout << arr2[i] * valu[amm[ii] / 100] << " ";
			cout << endl;

		for(int i = 0; i < N; i++, cout << endl)
		for(int j = 0; j < M; j++)
			cout << connection_b_cf[i][j] * valu[(amm[ii] % 100) / 10] << " ";

		for(int i = 0; i < M; i++, cout << endl)
		
		for(int j = 0; j < K; j++)
			cout << connection_b_fac[i][j] * valu[amm[ii] % 10] << " ";
		
	}

	fclose(stdout);

}