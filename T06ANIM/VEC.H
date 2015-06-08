/* FILE NAME: VEC.H
 * PROGRAMMER: Anya Mitina
 * DATE: 06.06.2015
 * LAST UPDATE: 08.06.2015
 * PURPOSE: own library of spatial math.
 */
//#ifndef __VEC_H
#include <math.h>
#define PI 3.14159265358979323846
/*#define D2R(A) ((A) * PI / 180.0)
#define N 4
#define M 4*/

#define DEGREE2RADIANL 0.01745329251994329576L
/* Multiplyer degree to radian constant */
static long double MultiplierDegree2Radian = DEGREE2RADIANL;

/* базовый вещественный тип */
typedef double DBL;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;


__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}

__inline VEC VecMulNum( VEC A, DBL N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N);
}

__inline VEC VecRotateZ( VEC V, DBL AngleInDegree )
{
  DBL sine, cosine, tmp;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  tmp = V.X * cosine - V.Y * sine;
  V.Y = V.X * sine + V.Y * cosine;
  V.X = tmp;
  return V;
} /* End of 'VecRotateZ' function */