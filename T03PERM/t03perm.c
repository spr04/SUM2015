/* FILE NAME: T03PERM.C
 * PROGRAMMER: Anya Mitina
 * DATE: 03.06.2015
 * PURPOSE: write all perms.
 */
#include <stdio.h>

#define N 5
int
P[N];

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

void main( void )
{
  FILE *F;
  if ((F = fopen("a.log", "a")) != NULL)
  {
    int i, j, n, a;
    for (n = 0; n < N; n++)
      P[n] = n + 1;

    for (i = 0; i < N - 1; i++)
      for (j = 1; j < N; j++)
      {       
        Swap(&P[i], &P[j]);
        for (a = 0; a < N; a++)
          fprintf(F, "%i", P[a]);
          fprintf(F, "\n");
      }
    
    fclose(F);
  }
}
/*void main( void )
{
  Go(3);
} */   
/* END OF 'T02CLOCK.C' FILE */