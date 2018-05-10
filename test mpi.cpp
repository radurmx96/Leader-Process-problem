// ProcesLider.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<time.h>
#include "stdafx.h"
#include<Windows.h>
#include<mpi.h>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv)
{
	int size, procid, rc, to_send[2], to_receive[2], leaderid;

	MPI_Status status;
	MPI_Request request;

	rc = MPI_Init(&argc, &argv);

	if (rc != MPI_SUCCESS)
	{
		printf( "Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);

	leaderid = procid;

	srand(time(NULL));

	int maxprio = rand() % 100;

	for (int i = 0; i < size; i++)
	{
		to_send[0] = leaderid;
		to_send[1] = maxprio;
		MPI_Isend(&to_send, 2, MPI_INT, (procid + 1) % size, 0, MPI_COMM_WORLD, &request);
		MPI_Recv(&to_receive, 2, MPI_INT, (size + procid - 1) % size, 0, MPI_COMM_WORLD, &status);
		printf("Procid %d  %d on step %d ", to_receive[0], to_receive[1], i);

		if (to_receive[1] > maxprio)
		{
			leaderid = to_receive[0];
			maxprio = to_receive[1];
		}
		else
		{
			if ((to_receive[1] == maxprio) && (to_receive[0] > procid))
			{
				leaderid = to_receive[0];
				maxprio = to_receive[1];
			}
		}
	}

	Sleep(2000);
	printf("%d has the leader %d with priority %d", procid, leaderid, maxprio);
	MPI_Finalize();
	return 0;
}
