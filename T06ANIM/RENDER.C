/* FILENAME: RENDER.C
 * PROGRAMMER: AM1
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"
#include "def.h"

#define AM1_UNIT_MATR  {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}}
        
/* Матрицы */
MATR
  AM1_RndMatrWorld = AM1_UNIT_MATR,
  AM1_RndMatrView = AM1_UNIT_MATR,
  AM1_RndMatrWorldView = AM1_UNIT_MATR;

/* Параметры проецирования */
DBL
  AM1_RndWp = 3, AM1_RndHp = 3,     /* размеры обрасти проецирования */
  AM1_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT AM1_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  P = VectorTransform(P, AM1_RndMatrWorldView);

  Pp.X = P.X * AM1_RndProjDist / P.Z;
  Pp.Y = P.Y * AM1_RndProjDist / P.Z;

  Ps.x = AM1_Anim.W / 2 + Pp.X * AM1_Anim.W / AM1_RndWp;
  Ps.y = AM1_Anim.H / 2 - Pp.Y * AM1_Anim.H / AM1_RndHp;

  return Ps;
} /* End of 'AM1_RndWorldToScreen' function */


/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       am1GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AM1_RndGObjLoad( am1GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(am1GOBJ));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((GObj->V = malloc(sizeof(VEC) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  GObj->F = (INT (*)[3])(GObj->V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &GObj->V[nv].X, &GObj->V[nv].Y, &GObj->V[nv].Z);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }

  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;
  return TRUE;
} /* End of 'AM1_RndGObjLoad' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       am1GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_RndGObjDraw( am1GOBJ *GObj )
{
  INT i;
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* проецируем все точки */
  AM1_RndMatrWorldView = MatrMulMatr(AM1_RndMatrWorld, AM1_RndMatrView);
  for (i = 0; i < GObj->NumOfV; i++)
    pnts[i] = AM1_RndWorldToScreen(GObj->V[i]);

  SelectObject(AM1_Anim.hDC, GetStockObject(BLACK_PEN));
  /* рисуем треугольники */
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n1 = GObj->F[i][0],
      n2 = GObj->F[i][1],
      n3 = GObj->F[i][2];

    MoveToEx(AM1_Anim.hDC, pnts[n1].x, pnts[n1].y, NULL);
    LineTo(AM1_Anim.hDC, pnts[n2].x, pnts[n2].y);
    LineTo(AM1_Anim.hDC, pnts[n3].x, pnts[n3].y);
    LineTo(AM1_Anim.hDC, pnts[n1].x, pnts[n1].y);
  }

  free(pnts);
} /* End of 'AM1_RndGObjDraw' function */

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       am1GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_RndGObjFree( am1GOBJ *GObj )
{
  free(GObj->V);
  memset(GObj, 0, sizeof(am1GOBJ));
} /* End of 'AM1_RndGObjFree' function */



/* END OF 'RENDER.C' FILE */

