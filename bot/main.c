#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "neuron.h"

double truedot[5][2] = {0.9,0.1,
						0.7,0.3,
						0.7,0.1,
						0.9,0.3,
						0.8,0.2 };

double falseedot[5][2] = { 0.1,0.9,
						   0.1,0.7,
						   0.3,0.9,
						   0.3,0.7,
						   0.2,0.8 };

COLOR16 colFunc(double input_a, double input_b)
{
	input_a = input_a;
	input_b = input_b;
	double* ansv = queryNeuron((double)input_a, (double)input_b);
	COLOR16 col;
	if(ansv[0]>0.8)
	col = RGB(0, 255, 0);
	else
	if(ansv[0]<0.2)
	col=RGB(255,0, 0);
	else
	col = RGB(255 * (1 - ansv[0]), 255 * ansv[0], 0);


	return col;
}

void draw(HDC dc)
{
	HDC memDc = CreateCompatibleDC(dc);
	HBITMAP memBm = CreateCompatibleBitmap(dc, 100, 100);
	SelectObject(memDc, memBm);

	for (int y = 0; y < 100; y++)
		for (int x = 0; x < 100; x++)
			SetPixel(memDc, x, y, colFunc(x, y));
	
	for(int i=0;i<5;i++)
		Ellipse(memDc, falseedot[i][0] * 100 - 3, falseedot[i][1] * 100 - 3, falseedot[i][0] * 100 + 3, falseedot[i][1] * 100 + 3);
	for (int i = 0; i < 5; i++)
		Ellipse(memDc, truedot[i][0] * 100 - 3, truedot[i][1] * 100 - 3, truedot[i][0] * 100 + 3, truedot[i][1] * 100 + 3);


//	outNeuron();
	

	BitBlt(dc, 0, 0, 200, 200, memDc, 0, 0, SRCCOPY);
	DeleteDC(memDc);
	DeleteObject(memBm);

}

int main()
{

	initNeuron(3, 2,7, 1);
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hDC, Pen);
	MoveToEx(hDC, 0, 85, NULL);
	LineTo(hDC, 200, 85);
	MoveToEx(hDC, 100, 0, NULL);
	LineTo(hDC, 100, 170);
	printf("\t\terr:  ");
	while (1)
	{
		int errcount = 0;
		for (int i = 0; i < 5; i++) {
			double* a = queryNeuron(truedot[i][0], truedot[i][1]);
			if (a[0] < 0.8)
			{
				errcount++;
				learningNeuron(1.f);
			}
		}
		for (int i = 0; i < 5; i++) {
			double* a = queryNeuron(falseedot[i][0], falseedot[i][0]);
			if (a[0] > 0.2)
			{
				errcount++;
				learningNeuron(0.f);
			}
		}
		printf("\b\b%2d", errcount);
	
		draw(hDC);
		//getchar();
		//delNeuron();
	}

	getchar();
	delNeuron();
}