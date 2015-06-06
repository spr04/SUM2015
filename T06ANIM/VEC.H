/* FILE NAME: VEC.H
 * PROGRAMMER: Anya Mitina
 * DATE: 06.06.2015
 * LAST UPDATE: 06.06.2015
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
long double MultiplierDegree2Radian = DEGREE2RADIANL;

/* базовый вещественный тип */
typedef double DBL;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;


