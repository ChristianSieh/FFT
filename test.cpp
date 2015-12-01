#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <algorithm>

using namespace std;

const int MAX = 8096;
const double EPSILON = 1.0e-12;
const complex<double> I (0, 1);
enum  direction {FORWARD, INVERSE};

////////////////////////////////////////////////////////////////////
// complex_round - just set to zero values that are very small    //
//             makes output easier to read                        //
////////////////////////////////////////////////////////////////////

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

void print_polynomial(complex <double> a[], int n, complex <double> total[])
{
	for (int i = 0; i < n / 2; i++)
	{
		total[i] += abs(a[i]);
	}	 
}

////////////////////////////////////////////////////////////////////
//                        FFT based on CLRS page 911              //
////////////////////////////////////////////////////////////////////

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

	fout << "a[]: " << endl;

	for (int i = 0; i < n; i++)
	{
		fin >> a_real[i];
		a[i] = a_real[i];
		//fout << a[i] << endl;
	}

	// Do forward FFT
	fft(a, n, y, FORWARD);

	// Clean up result by setting small values to zero
	complex_round(y, n);

	print_polynomial(y, n, total);

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
		print_polynomial(y, n, total);
	}

	for(int i = 0; i < n/2; i++)
	{
		total[i] = total[i] / k;
	}


	complex <double> * normalTotal = new complex <double> [n/2];

	//Add points
	for(int i = 3; i < (n/2) - 3; i++)
	{
		normalTotal[i] = total[i - 2] + total[i - 1] + total[i] +
			total[i + 1] + total[i + 2];
	}	

	fout << "Total: " << endl;

	for(int i = 0; i < (n/2); i++)
	{
		fout << total[i] << endl;
	}

	fout << "Normal Total: " << endl;

	for(int i = 0; i < (n/2); i++)
	{
		if(abs(normalTotal[i]) > 30)
			fout << normalTotal[i] << endl;
	}

	int index[5] = {0};
	complex <double> maxima[5] = {0};

	for(int i = 0; i < (n/2); i++)
	{
		if(abs(normalTotal[i]) > abs(maxima[4]))
		{
			maxima[4] = normalTotal[i];
			index[4] = i;

			sort(index, index+5);
			sort(maxima, maxima+5);			
		}
	}
}

////////////////////////////////////////////////////////////////////
