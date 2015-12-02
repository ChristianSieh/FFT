#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <algorithm>
#include <vector>

using namespace std;

const int MAX = 8096;
const double EPSILON = 1.0e-12;
const complex<double> I (0, 1);
enum  direction {FORWARD, INVERSE};

//Struct that will be used to find the 5 amplitudes
struct doubleIndex
{
	double x;
	int index;

	bool operator<(doubleIndex other) const
	{
		return x > other.x;
	}
};

////////////////////////////////////////////////////////////////////
// complex_round - just set to zero values that are very small    //
//             makes output easier to read                        //
////////////////////////////////////////////////////////////////////
//Author: Dr. Edward Corwin
//Removes very small numbers from a complex type
void complex_round(complex <double> a[], int n)
{
	float  x;
	float  y;

	for (int i = 0; i < n; i++)
	{
		x = a[i].real();
		y = a[i].imag();
		if (fabs(x) < EPSILON)
			x = 0;
		if (fabs(y) < EPSILON)
			y = 0;
		a[i] = complex<double>(x, y);
	}
}

////////////////////////////////////////////////////////////////////
//                     print a complex polynomial                 //
////////////////////////////////////////////////////////////////////
//Total up the absolute values in array a and add to total array
void addToTotal(complex <double> a[], int n, complex <double> total[])
{
	for (int i = 0; i < n / 2; i++)
	{
		total[i] += abs(a[i]);
	}	 
}

////////////////////////////////////////////////////////////////////
//                        FFT based on CLRS page 911              //
////////////////////////////////////////////////////////////////////
//Author: Dr. Edward Corwin
//Computes the FFT for the numbers given in array a up to values n
//and puts the output in array y
void fft(complex <double> a[], int n, complex <double> y[],
  direction dir)
{
	complex  <double> even[n];
	complex  <double> even_fft[n];
	int      i;
	int      j;
	int      n2;
	complex  <double> odd[n];
	complex  <double> odd_fft[n];
	complex  <double> omega;
	complex  <double> omega_power;

	if (n == 1)
	{
		y[0] = a[0];
		return;
	}
  
	n2 = n / 2;
	j = 0;
	for (i = 0; i < n; i += 2)
	{
		even[j] = a[i];
		odd[j] = a[i + 1];
		j++;
	}
    
	fft(even, n2, even_fft, dir);
	fft(odd, n2, odd_fft, dir);

	if (dir == FORWARD)
		omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);
	else
		omega = cos(2.0 * M_PI / n) + I * sin(2.0 * M_PI / n);
	omega_power = 1;

	for (i = 0; i < n2; i ++)
	{
		y[i] = even_fft[i] + omega_power * odd_fft[i];
		y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
		omega_power = omega * omega_power;
	}
}

string outFileName(string inFileName)
{
	int pos = inFileName.find('.');

	string test = inFileName.substr(0, pos) + ".out";

	return test;
}

////////////////////////////////////////////////////////////////////

/*
Author: Christian Sieh
Class: CSC372
Assignment: Program #3 FFT
Date: 12/1/2015
Usage: fft.cpp file.in

This program will read in a nValue (number of points in FFT) and
a kValue (number of FFTs to compute) and then the file must have
at least n + k points which will be used to compute the multiple
FFTs. The first FFT is computed using the first n points using
Dr. Edward Corwin's FFT code. Then the values outputted by that
FFT are used to compute the next k-1 FFTs. This is done in order
to save time by not having to compute every FFT. Finally, this
program will output the 5 largest "amplitude" values alone with
their index.
*/
int main(int  argc, char * argv[])
{
	double   a_real[MAX];
	complex  <double> a[MAX];
	complex  <double> b[MAX] = { 0 };
	ifstream fin;
	ofstream fout;
	int n;
	double k;
	complex  <double> y[MAX] = { 0 };

	fin.open(argv[1]);
	
	if(!fin)
		cout << "Input file failed to open" << endl;
	
	fout.open(outFileName(argv[1]).c_str());

	if(!fout)
		cout << "Output file failed to open" << endl;

	fin >> n;
	fin >> k;

	complex <double> * total = new complex <double> [n/2];

	//Read in all of our initial values
	for (int i = 0; i < n; i++)
	{
		fin >> a_real[i];
		a[i] = a_real[i];
	}

	// Do forward FFT
	fft(a, n, y, FORWARD);

	// Clean up result by setting small values to zero
	complex_round(y, n);

	addToTotal(y, n, total);

	complex <double> newAVal = 0;
	double temp = 0;
	complex <double> omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);

	//for each 'a' value up through 'k' 'a values'
	for(int i = 1; i < k; i++)
	{
		fin >> temp;
		newAVal = temp;
		complex <double> x = 1;		

		//For each y value
		for(int j = 0; j < n; j++)
		{
			//cout << "Omega[" << j << "]: " << x << endl;
			y[j] = ((y[j] - a[i-1] + newAVal) / x);
				
			x = x * omega;		
		}
		
		//Add absolute total
		addToTotal(y, n, total);
	}

	//Divide the total array by k to get the average
	for(int i = 0; i < n/2; i++)
	{
		total[i] = total[i] / k;
	}

	complex <double> * normalTotal = new complex <double> [n/2];

	//Add the 2 points before and after the local maxima
	for(int i = 3; i < (n/2) - 3; i++)
	{
		if(real(total[i-2]) < real(total[i]) && real(total[i-1]) 
			< real(total[i]) && real(total[i]) > 
			real(total[i+1]) && real(total[i]) >
			real(total[i+2]))
		{
			normalTotal[i] = total[i - 2] + total[i - 1] 
				+ total[i] + total[i + 1] + total[i + 2];
		}
	}	

	vector <doubleIndex> real;

	//Switch the complex array into a doubleIndex array
	//This method of finding the 5 largest values might
	//not be the fastest but it has simple syntax
	for(int i = 0; i < (n/2); i++)
	{
		doubleIndex temp;

		temp.x = normalTotal[i].real();
		temp.index = i;

		real.push_back(temp);
	}

	//sort the double index array
	sort(real.begin(), real.end());

	//output the 5 largest values
	for(int i = 0; i < 5; i++)
	{
		fout << real[i].index << " ";
		fout << fixed << setprecision(2);
		fout << real[i].x << endl;
	}
}

////////////////////////////////////////////////////////////////////
