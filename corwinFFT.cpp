#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "complex.h"  // complex code from Dr. Weiss with additions
using namespace std;

const int MAX = 1024;
const double EPSILON = 1.0e-12;

////////////////////////////////////////////////////////////////////
//     round - just set to zero values that are very small        //
//             makes output easier to read                        //
////////////////////////////////////////////////////////////////////

void round(complex a[], int n)
  {
  int    i;
  float  x;
  float  y;

  for (i = 0; i < n; i++)
    {
    x = a[i].realpart();
    y = a[i].imagpart();
    if (fabs(x) < EPSILON)
      x = 0;
    if (fabs(y) < EPSILON)
      y = 0;
    a[i] = complex(x, y);
    }
  }

////////////////////////////////////////////////////////////////////
//        swap two complex numbers - used by reverse function     //
////////////////////////////////////////////////////////////////////

void swap(complex &x, complex &y)
  {
  complex old_x;
  complex old_y;

  old_x = x;
  old_y = y;
  x = old_y; 
  y = old_x;
  }

////////////////////////////////////////////////////////////////////
//                    reverse a complex array                     //
//    Used since inverse transform does some things reversed      //
////////////////////////////////////////////////////////////////////

void reverse(complex a[], int n)
  {
  int i;
  for (i = 0; i < n / 2; i++)
    swap(a[i], a[n - i - 1]);
  }

////////////////////////////////////////////////////////////////////
//                     print a complex polynomial                 //
////////////////////////////////////////////////////////////////////

void print_polynomial(complex a[], int n)
  {
  int i;
  for (i = 0; i < n; i++)
    cout << a[i] << "  ";
  cout << endl;
  }

////////////////////////////////////////////////////////////////////
//                forward FFT based on CLRS page 835              //
////////////////////////////////////////////////////////////////////

void forward_fft(complex a[], int n, complex y[])
  {
  complex  even[MAX];
  complex  even_fft[MAX];
  int      i;
  int      j;
  int      n2;
  complex  odd[MAX];
  complex  odd_fft[MAX];
  complex  omega;
  complex  omega_power;

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
    
  forward_fft(even, n2, even_fft);
  forward_fft(odd, n2, odd_fft);

  omega = complex(cos(2.0 * M_PI / n), sin(2.0 * M_PI / n));
  omega_power = 1;
  for (i = 0; i < n2; i ++)
    {
    omega_power = omega * omega_power;
    y[i] = even_fft[i] + omega_power * odd_fft[i];
    y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
    }
  }

////////////////////////////////////////////////////////////////////
//                            inverse FFT                         //
////////////////////////////////////////////////////////////////////

void inverse_fft(complex y[], int n, complex a[])
  {
  complex  even[MAX];
  complex  even_fft[MAX];
  int      i;
  int      j;
  int      n2;
  complex  odd[MAX];
  complex  odd_fft[MAX];
  complex  omega;
  complex  omega_power;

  if (n == 1)
    {
    a[0] = y[0];
    return;
    }
  
  n2 = n / 2;
  j = 0;
  for (i = 0; i < n; i += 2)
    {
    even[j] = y[i];
    odd[j] = y[i + 1];
    j ++;
    }
    
  inverse_fft(even, n2, even_fft);
  inverse_fft(odd, n2, odd_fft);

  omega = complex(cos(-2.0 * M_PI / n), sin(-2.0 * M_PI / n));
  omega_power = 1;
  for (i = 0; i < n2; i ++)
    {
    omega_power = omega * omega_power;
    a[i] = even_fft[i] + omega_power * odd_fft[i];
    a[i + n2] = even_fft[i] - omega_power * odd_fft[i];
    }
  }

////////////////////////////////////////////////////////////////////
/*
int main()
  {
  complex  a[MAX];
  complex  b[MAX] = {0};
  ifstream inf;
  int      i;
  int      n;
  complex  y[MAX] = {0};

  inf.open("fft.in");
  if (inf.fail())
    {
    cout << "Unable to open fft.in" << endl;
    exit(1);
    }

  inf >> n;
  cout << "n = " << n << endl;
  for (i = 0; i < n; i++)
    {
    inf >> a[i];
    }

  cout << "Original:       ";
  print_polynomial(a, n);

  // Do forward FFT

  forward_fft(a, n, y);

  // Clean up result by setting small values to zero

  round(y, n);

  cout << "Forward FFT:    ";
  print_polynomial(y, n);

  // Do inverse FFT, first reverse the y vector

  reverse(y, n);

  inverse_fft(y, n, b);

  // Clean up result by setting small values to zero

  round(b, n);
  cout << "Inverse FFT:    ";
  print_polynomial(b, n);

  // Reconstruct original polynomial by reversing and dividing by n

  reverse(b, n);
  for (i = 0; i < n; i++)
    {
    b[i] = b[i] / n;
    }
  cout << "Reconstructed:  ";
  print_polynomial(b, n);


  }*/

////////////////////////////////////////////////////////////////////
