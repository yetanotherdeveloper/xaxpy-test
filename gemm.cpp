// Intention of this module is to compare diffrent
// implementations of BLAS SAXPY functions

//A*x+y -> y
void iaxpy(const int *A,const int *x, int *y, int m, int n)
{
  for(int j = 0; j < m; ++j) {
    int sum = 0;
    for(int i = 0; i < n; ++i) {
      sum += A[n *j + i] * x[i];
    }
    y[j] += sum;
  }
}

void saxpy(const float *A,const float *x, float *y, int m, int n)
{
  for(int j = 0; j < m; ++j) {
    float sum = 0;
    for(int i = 0; i < n; ++i) {
      sum += A[n *j + i] * x[i];
    }
    y[j] += sum;
  }

}
