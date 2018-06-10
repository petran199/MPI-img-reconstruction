// stdafx.cpp : source file that includes just the standard includes
// mpiProj.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <mpi.h>

#include <opencv2/opencv.hpp>

/*C:\opencv\install\x64\vc14\lib;*/

using namespace cv;
using namespace std;
// global vars
double p1, p2, Kraw_px, Kraw_qy;
using namespace std;

//ipologismos varwn
void Weight_Function(double *W, double p, int N)
{
	int x;

	W[0] = pow(1 - p, N);
	for (x = 0; x <= N - 1; x++)
		W[x + 1] = ((double)(N - x) / (x + 1))*(double)(p / (1 - p))*(double)W[x];
}
//ipologismos paragodikou
double Factorial(int end)
{
	double result;
	int i;

	if (end == 0)
		return(1.0);

	result = 1;
	for (i = 1; i <= end; i++)
		result = result*i;

	return(result);
}
//ipologismos tou pochammer
double pochhammer(int a, int k)
{
	double poch;
	int i;

	if (k == 0)
		poch = 1;
	else
	{
		poch = 1;
		for (i = 1; i <= k; i++)
			poch = poch*(a + i - 1);
	}

	return(poch);
}


//ipologismos tis normas
double p_norm(int n, double p, int N)
{
	double pnorm;

	pnorm = pow(-1, n)*pow((1 - p) / p, n)*(Factorial(n) / pochhammer(-N, n));

	return(pnorm);
}
// ropes gia ton X
double Krawtchouk_bar_poly_X(int order, double p, int xy, int N, double *W)
{
	double k[MAX_ORDER], A, B, w;
	int n, ni;

	w = W[xy];

	switch (order)
	{
	case 0:
		k[0] = sqrt(w / p_norm(0, p, N));
		break;
	case 1:
		k[1] = (1 - (xy / (p*N)))*sqrt(w / p_norm(1, p, N));
		break;
	default:
		k[0] = sqrt(w / p_norm(0, p, N));
		k[1] = (1 - (xy / (p*N)))*sqrt(w / p_norm(1, p, N));
		for (n = 1; n<order; n++)
		{

			A = (double)sqrt(p*(N - n) / (double)((1 - p)*(n + 1)));
			B = (double)sqrt((pow(p, 2)*(N - n)*(N - n + 1)) / (double)(pow(1 - p, 2)*(n + 1)*n));
			k[n + 1] = (A*(N*p - 2 * n*p + n - xy)*k[n] - B*n*(1 - p)*k[n - 1]) / (p*(N - n));
		}
		break;
	}
	return(k[order]);
}
//ropes gia ton Y
double Krawtchouk_bar_poly_Y(int order, double p, int xy, int N, double *W)
{
	double k[MAX_ORDER], A, B, w;
	int n, ni;

	w = W[xy];

	switch (order)
	{
	case 0:
		k[0] = sqrt(w / p_norm(0, p, N));
		break;
	case 1:
		k[1] = (1 - (xy / (p*N)))*sqrt(w / p_norm(1, p, N));
		break;
	default:
		k[0] = sqrt(w / p_norm(0, p, N));
		k[1] = (1 - (xy / (p*N)))*sqrt(w / p_norm(1, p, N));
		for (n = 1; n<order; n++)
		{

			A = (double)sqrt(p*(N - n) / (double)((1 - p)*(n + 1)));
			B = (double)sqrt((pow(p, 2)*(N - n)*(N - n + 1)) / (double)(pow(1 - p, 2)*(n + 1)*n));
			k[n + 1] = (A*(N*p - 2 * n*p + n - xy)*k[n] - B*n*(1 - p)*k[n - 1]) / (p*(N - n));
		}
		break;
	}
	return(k[order]);
}
//sinartisi gia na vroume to K[p][q] pou xriazete gia tin anakataskevi
void MainProcess(int worRank, int worSize, double* Karay)
{
	int p, q, x, y, NumMoments;
	double s1, s2, kpx, kqy, fxy, KMpq;

	NumMoments = 0;

	p1 = 0.5;
	p2 = 0.5;

	Weight_Function(Wx, p1, N - 1);
	Weight_Function(Wy, p2, M - 1);
	//((Order / worSize)*worRank); p < ((Order / worSize)*worRank) + (Order / worSize);
	for (p = worRank; p < Order; p += worSize)
	{
		//((Order / worSize)*worRank); q < ((Order / worSize)*worRank) + (Order / worSize);
		for (q = 0; q < Order; q++)
		{
			s2 = 0;
			for (x = 0; x<N; x++)
			{
				kpx = Krawtchouk_bar_poly_X(p, p1, x, N - 1, Wx);
				s1 = 0;
				for (y = 0; y<M; y++)
				{
					kqy = Krawtchouk_bar_poly_Y(q, p2, y, M - 1, Wy);
					fxy = (double)(InputImage.F[x][y]);
					s1 = s1 + kqy*fxy;
					//cout << "kqy=" << kqy << "\tfxy=" << fxy << endl;
				}
				s2 = s2 + kpx*s1;
			}
			MPI_Barrier(MPI_COMM_WORLD);
			NumMoments++;

			KMpq = s2;
			//Karay[(p*q) + q] = KMpq;
			K[p][q] = KMpq;
			cout << K[p][q] << " ";
			//if (p == 7 && q == 7) cout << K[p][q] << " ";



		}
		//MPI_Barrier(MPI_COMM_WORLD);
		cout << endl;
	}

}
//sinartisi anakataskevis eikonas
void reconstructImage(int WorRnk, int worlSz, int* Faray) {
	int x, y, p, q;
	double kpx, kqy, s1, s2, MIN, MAX;

	for (x = WorRnk; x < N; x += WorRnk)
	{
		for (y = 0; y < M; y++)
		{
			s2 = 0;
			for (p = 0; p < Order; p++)
			{

				kpx = Krawtchouk_bar_poly_X(p, p1, x, N - 1, Wx);
				s1 = 0;
				for (q = 0; q < Order; q++)
				{
					kqy = Krawtchouk_bar_poly_Y(q, p2, y, M - 1, Wy);
					s1 = s1 + K[p][q] * kqy;
				}
				s2 = s2 + kpx*s1;
			}
			//Faray[(x*y) + y] = ceil(s2);
			RecImage.F[x][y] = ceil(s2);
			cout << RecImage.F[x][y] << " ";
		}
		cout << endl;
	}
}
// fortwse diastaseis eikonas, ta values kathe pixel tis ston F[][]
void imgread()
{
	Mat img1;

	//double localArray[64] = { 0 };
	img1 = imread("C:\\Users\\Petran\\Desktop\\MpiProject\\MpiProject\\Lena Image.bmp", 0);
	if (img1.empty()) {
		cout << "Error : Image cannot be loaded..!!" << endl;
		exit(EXIT_FAILURE);
	}
	//vriskoume to width, height tis eikonas
	Size s = img1.size();
	//vazoume ston F[i][j] ta pixels tis eikonas tis lenas
	for (int i = 0; i < s.width; i++) {
		for (int j = 0; j < s.height; j++) {
			InputImage.F[i][j] = img1.ptr<uchar>(j)[i];
			cout << InputImage.F[i][j] << "\t";
		}
		cout << endl;
	}
	InputImage.ImWidth = s.width;
	InputImage.ImHeight = s.height;
	N = InputImage.ImWidth;
	M = InputImage.ImHeight;
}

void recimg()
{
	Mat recImg;// (128, 128, CV_8UC1);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {

			recImg.ptr<uchar>(j)[i] = RecImage.F[i][j];
		}
	}
	imwrite("C:\\Users\\Petran\\Desktop\\MpiProject\\MpiProject\\Lena Reconstr.bmp", recImg);
}