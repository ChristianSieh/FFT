#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <complex>

using namespace std;

const int MAX = 8096;
const double EPSILON = 1.0e-12;
const complex<double> I (0, 1);
enum  direction {FORWARD, INVERSE};

using namespace std;

string outFileName(string fin);
void fft(complex <double> a[], int n, complex <double> y[], 
	direction dir);


int main(int  argc, char * argv[])
{
	ifstream fin;
	ofstream fout;
	int nValue;
	int kValue;
	float temp;

	fin.open(argv[1]);
	fout.open(outFileName(argv[1]).c_str());

	fin >> nValue;
	fin >> kValue;

	fout << nValue << endl;
	fout << kValue << endl;

	complex < double > * values = new complex <double> [nValue];
	complex < double > * y = new complex <double> [nValue];

	for(int i = 0; i < nValue; i++)
	{
		fin >> values[i];
	}

	fft(values, nValue, y, FORWARD);

	for(int i = 0; i < nValue; i++)
	{
		fout << y[i] << endl;
	}	
}

string outFileName(string inFileName)
{
	int pos = inFileName.find('.');

	string test = inFileName.substr(0, pos) + ".out";

	return test;
}

void fft(complex <double> a[], int n, complex < double > y[],
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
    j ++;
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
