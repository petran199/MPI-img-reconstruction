// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#define MAX_WIDTH_IMSIZE    256
#define MAX_HEIGHT_IMSIZE   MAX_WIDTH_IMSIZE
#define MAX_ORDER           32


typedef struct
{
	int   F[MAX_WIDTH_IMSIZE][MAX_HEIGHT_IMSIZE];
	int   ImWidth;
	int   ImHeight;
}Image;

//Global Declarations
Image InputImage, RecImage;
int Order = 16, N, M;

double K[MAX_ORDER][MAX_ORDER];


//Function Prototypes
void reconstructImage(int worlSz, int WorRnk, int* Faray);
void MainProcess(int worRank, int worSize, double* Karay);
void ProgressBarConfig(void);
void Weight_Function(double *W, double p, int N);
double pochhammer(int a, int k);
double p_norm(int n, double p, int N);
double Krawtchouk_bar_poly_X(int order, double p, int xy, int N, double *W);
double Krawtchouk_bar_poly_Y(int order, double p, int xy, int N, double *W);
double Wx[MAX_WIDTH_IMSIZE], Wy[MAX_WIDTH_IMSIZE];
void imgread();
void recimg();



// TODO: reference additional headers your program requires here
