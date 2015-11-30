#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <complex>

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
  int    i;
  float  x;
  float  y;

  for (i = 0; i < n; i++)
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

void print_polynomial(complex <double> a[], int n)
  {
  int i;
  for (i = 0; i < n / 2; i++)  // just print first 6
    {
	if(abs(a[i]) > 8)
	  cout << abs(a[i]) << "  ";
    }	  
    cout << endl;
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

////////////////////////////////////////////////////////////////////

int main()
  {
  double   a_real[MAX];
  complex  <double> a[MAX];
  complex  <double> b[MAX] = {0};
  int      i;
  ifstream inf;
  ofstream fout;
  int      n;
  complex  <double> y[MAX] = {0};

  inf.open("p3.in");
  fout.open("test.out");
  if (inf.fail())
    {
    cout << "Unable to open fft_2011.in" << endl;
    exit(1);
    }

  if (fout.fail())
	{
		cout << "wtf" << endl;
	}

  inf >> n;
  cout << "n = " << n << endl;
  for (i = 0; i < n; i++)
    {
    inf >> a_real[i];
    a[i] = a_real[i];
    }

  //cout << "Original:       ";
  //print_polynomial(a, n);

  // Do forward FFT

  fft(a, n, y, FORWARD);

  // Clean up result by setting small values to zero

  complex_round(y, n);

  cout << "Forward FFT:    ";
  print_polynomial(y, n);

  return 0;   // don't do inverse for comparisons
  }

////////////////////////////////////////////////////////////////////
