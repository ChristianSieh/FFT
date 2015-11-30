#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include "corwinFFT.cpp"

using namespace std;

string outFileName(string fin);

int main(int  argc, char * argv[])
{
	ifstream fin;
	ofstream fout;
	int nValue;
	int kValue;
	float temp;
	vector<float> values;

	fin.open(argv[1]);
	fout.open(outFileName(argv[1]).c_str());

	fin >> nValue;
	fin >> kValue;

	fout << nValue << endl;
	fout << kValue << endl;

	int counter = 0;

	while (fin >> temp)
	{
		values.push_back(temp);
	
		if(counter >= kValue)
		{
			//compute fft		

		}
		
		fout << temp << endl;
	}
}

string outFileName(string inFileName)
{
	int pos = inFileName.find('.');

	string test = inFileName.substr(0, pos) + ".out";

	return test;
}
