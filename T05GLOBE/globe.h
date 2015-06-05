#pragma warning(disable: 4244)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <windows.h>

#define PI 3.1415926535897932384
#define N 30
#define M 30

/* Тип хранения координат точки в пространстве */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC;

static VEC Grid[N][M];

VOID GlobeDraw( HDC hDC, INT W, INT H );
VOID GlobeBuild( VOID );
 
