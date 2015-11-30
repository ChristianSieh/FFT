#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

string outFileName(string fin);

int main(int  argc, char * argv[])
{
	ifstream fin;
	ofstream fout;
	int nValue;
	int kValue;
	float temp;


	fin.open(argv[1]);
	fout.open(outFileName(argv[1]));

	fin >> nValue;
	fin >> kValue;

	fout << nValue << endl;
	fout << kValue << endl;

	while (fin >> temp)
	{
		fout << temp << endl;
	}
}

string outFileName(string inFileName)
{
	int pos = inFileName.find('.');

	string test = inFileName.substr(0, pos) + ".out";

	return test;
}