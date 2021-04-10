#pragma once
int initNeuron(int layers,...);
void outNeuron();
double activateFunck(double input);
double* queryNeuron(double input, ...);
void delNeuron();
void test();
void learningNeuron(double output, ...);