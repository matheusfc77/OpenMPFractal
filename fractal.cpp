#include <complex>
#include <cstdio>
#include <chrono>
#include <stdio.h>
 
typedef std::complex<double> complex;
 
int MandelbrotCalculate(complex c, int maxiter)
{
  // iterates z = z�� + c until |z| >= 2 or maxiter is reached,
  // returns the number of iterations.
  complex z = c;
  int n=0;
  for(; n<maxiter; ++n)
  {
      if( std::abs(z) >= 2.0) break;
      z = z*z + c;
  }
  return n;
}
int main()
{
  const int width = 78, height = 44, num_pixels = width*height;
    
  const complex center(-.7, 0), span(2.7, -(4/3.0)*2.7*height/width);
  const complex begin = center-span/2.0;//, end = center+span/2.0;
  const int maxiter = 100000;
  static const char charset[] = ".,c8M@jawrpogOQEPGJ";
  
  auto t_start = std::chrono::high_resolution_clock::now();
  #pragma omp parallel for ordered schedule(dynamic)
  for(int pix=0; pix<num_pixels; ++pix)
  {
    const int x = pix%width, y = pix/width;
      
    complex c = begin + complex(x * span.real() / (width +1.0),
                                y * span.imag() / (height+1.0));
      
    int n = MandelbrotCalculate(c, maxiter);
    if(n == maxiter) n = 0;
        
    #pragma omp ordered
    {
      char c = ' ';
      if(n > 0)
      {
          c = charset[n % (sizeof(charset)-1)];
      }
      std::putchar(c);
      if(x+1 == width) std::puts("|");
    }
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
  printf("Time (milliseconds): %lf", elapsed_time_ms);
 }