/* FILE NAME: T03PERM.C
 * PROGRAMMER: Anya Mitina
 * DATE: 03.06.2015
 * PURPOSE: write all determs of matrix.
 */
#include <stdio.h>

#define MAX 10

double A[MAX][MAX];
int P[MAX];
int N, Parity = 0;
double SUM = 0;

int CountParity( void )
{
  int i, j, cnt = 0;
  for (i = 0; i < N - 1; i++)
    for (j = i + 1; j < N; j++)
      if (P[i] > P[j])
        cnt++;
  return cnt;
}

void Swap( int *A, int *B )
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}

void Load( char *FileName )
{
  FILE *F;
  int i, j;

  if ((F = fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
}

void Go( void )
{

}

void main( void )
{ 
  Load("d.txt");
  int i, Parity;
  double PROD = 1;
  FILE *F;
  if ((F = fopen("d.txt", "a")) != NULL)
  {
    //fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
    PROD *= A[i][P[i]];
    Parity = CountParity();
    if (Parity == 0)
      SUM += PROD;
    else
      SUM -= PROD;
    
    
    fclose(F);
  }
}
/*void main( void )
{
  Go(3);
} */   
/* END OF 'T02CLOCK.C' FILE */