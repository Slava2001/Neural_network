#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "neuron.h"
#include <math.h>


int layers;
int *countInlayers;
double learningCoff = 0.85;

double ***weight;
double **neuronState;
double **neuronError;

void test()
{
	initNeuron(3, 2, 2, 1);
	
	weight[0][0][0] = -1;
	weight[0][0][1] = 2;
	weight[0][0][2] = 0;

	weight[0][1][0] = 1;
	weight[0][1][1] = 1;
	weight[0][1][2] = 0;

	weight[0][2][0] = 3;
	weight[0][2][1] = -2 ;
	weight[0][2][2] = 0;

	weight[1][0][0] = 1;
	weight[1][0][1] = 2;
	weight[1][0][2] = 4;
	weight[1][0][3] = 0;

	queryNeuron(0.2, 0.5);
	outNeuron();
	learningNeuron(0.4);
	outNeuron();
}


int initNeuron(int layer, ...)
{
	va_list factor;   
	va_start(factor, layer);

	layers = layer;

	srand(time(NULL));

	countInlayers = (int*)malloc(sizeof(int) * layers);
	neuronState = (int**)malloc(sizeof(int*) * layers);
	neuronError = (int**)malloc(sizeof(int*) * layers);
	weight = (double***)malloc(sizeof(double**) * (layers - 1));

	for (int i = 0; i < layers; i++)
	{
		countInlayers[i] = va_arg(factor, int);
		neuronState[i] = (double*)malloc(sizeof(double) * countInlayers[i]);
		neuronError[i] = (double*)malloc(sizeof(double) * countInlayers[i]);
	}
	va_end(factor);
	for (int i = 0; i < layers - 1; i++) 
	{
		weight[i] = (double**)malloc(sizeof(double*) * countInlayers[i + 1]);
		
		for (int t = 0; t < countInlayers[i + 1]; t++)
		{
			weight[i][t] = (double*)malloc(sizeof(double) * (countInlayers[i]+1));
			for (int q=0;q<=countInlayers[i];q++)
			{
				weight[i][t][q] =  (double)(rand() % 2000 - 1000) / 100;
			}
		}
	}
}
void outNeuron()
{
	for (int i = 0; i < layers; i++)
	{
		for (int q=0;q<countInlayers[i];q++)
		{
			printf("%2.4f(%2.4f) ",neuronState[i][q], neuronError[i][q]);
		}
		printf("\n");
	}



	for (int i = 0; i< layers - 1; i++)
	{
		printf("W%d\n", i);
		for (int q = 0; q < countInlayers[i+1]; q++)
		{
			for (int t = 0; t <= countInlayers[i];t++)
			{
				printf("%f\t",weight[i][q][t]);
			}
			printf("\n");

		}
	}


}
double* queryNeuron(double input,...)//возврашает массив ответов
{

	va_list factor;
	va_start(factor, input);
	neuronState[0][0] = input;
	for (int i = 1; i < countInlayers[0]; i++)
		neuronState[0][i] = va_arg(factor, double);
	
	va_end(factor);



	for(int i=0;i<layers-1;i++)
		for (int t = 0; t < countInlayers[i + 1]; t++)
		{
			neuronState[i + 1][t] = 0;
			
			for (int q = 0; q < countInlayers[i]; q++)
			{
				neuronState[i + 1][t] += weight[i][t][q]*neuronState[i][q];
			}
			neuronState[i + 1][t] += weight[i][t][countInlayers[i]];

			neuronState[i + 1][t] = activateFunck(neuronState[i + 1][t]);
		}
	int maxNeuronIndex = 0;

	return neuronState[layers-1];
}
double activateFunck(double input) 
{
	return 1/(1+exp(-input));
}
void delNeuron()
{
	for (int i = 0; i < layers - 1; i++)
			free(weight[i]);
	free(weight);
	for (int i = 0; i < layers; i++)
		free(neuronState[i]);
	free(neuronState);
	free(countInlayers);
	layers = 0;
}
void learningNeuron(double output, ...)
{


	va_list factor;
	va_start(factor, output);
	neuronError[layers - 1][0] = (output - neuronState[layers - 1][0])* neuronState[layers - 1][0]*(1- neuronState[layers - 1][0]);
	for(int i=1;i<countInlayers[layers-1];i++)
		neuronError[layers - 1][i] =(va_arg(factor, double) - neuronState[layers - 1][i])* neuronState[layers - 1][i]*(1- neuronState[layers - 1][i]);
	va_end(factor);




	
	for (int i = layers-2; i >0; i--)
		for (int t = 0; t < countInlayers[i]; t++)
		{	
			double ErrorSum = 0;

			for (int q = 0; q < countInlayers[i+1]; q++)
			{
				ErrorSum += neuronState[i + 1][q] * weight[i][q][t];
			}
			neuronError[i][t] = ErrorSum * neuronState[i][t] * (1 - neuronState[i][t]);
		}


	for (int i = 0; i < layers ; i++)
		for (int t = 0; t < countInlayers[i + 1]; t++)
			for (int q = 0; q <= countInlayers[i]+1; q++)
			{
				weight[i][t][q] += neuronState[i][q] * neuronError[i + 1][t] * learningCoff;
			}
		
	


}