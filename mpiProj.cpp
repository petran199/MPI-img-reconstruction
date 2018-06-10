// mpiProj.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mpi.h>
#include <iostream>
using namespace std;

int main()
{
	//i eikona lena 128x128
	imgread();
	//cout << "n=" << N << "\tm=" << M << endl;
	MPI_Init(NULL, NULL);

	// ta id twn processes
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//tosinolo twn processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_rank == 0)
	{
		//imgread();
		//cout << "n=" << N << "\tm=" << M << endl;
		/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
		cout << K[i][j] << " ";
		}
		cout << endl;
		}*/
		cout << " i am 0\t";
	}
	else
	{
		switch (world_rank)
		{
		case 1: cout << "i am 1\t"; break;
		case 2: cout << "i am 2\t"; break;
		default: cout << "i am 3\t";
			break;
		}
	}
	//trexe gia kathe procces parallila to akoloutho gia na vrethoun oi times tou K[MAX_ORDER][MAX_ORDER]
	//MainProcess(world_rank, world_size, K[0]);
	////tropos sychronization twn processes
	//MPI_Barrier(MPI_COMM_WORLD);
	//// parallila vres ta F[x][y] tis anakataskevasmenis eikonas apo tis ropes
	//reconstructImage(world_rank, world_size, RecImage.F[0]);
	//MPI_Barrier(MPI_COMM_WORLD);

	//recimg();
	MPI_Finalize();
    return 0;
}

