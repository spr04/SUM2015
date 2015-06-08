/* FILENAME: LOADOBJ.C
 * PROGRAMMER: AM1
 * PURPOSE: Load 3D nmodel from *.OBJ files.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vec.h"
#include "cow.h"
/* Global model data */

/* Vertex array */
VEC *ObjV; /* Vertex coordinates */
INT ObjNumOfV; /* Number of model vertices */

/* Draw object functioln.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ObjDraw( HDC hDC, INT H, INT W )
{
  INT i;

  for (i = 0; i < ObjNumOfV; i++)
  {
    /* рисуем точку ObjV[i] */
     
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 0, 0));  
    Ellipse(hDC, ObjV[i].X + W / 2, ObjV[i].Y + H / 2, ObjV[i].X + W / 2 + 5, ObjV[i].Y + H / 2 + 5);
  }              
} /* End of 'ObjDraw' function */

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName )
{
  FILE *F;
  INT nv = 0;
  static CHAR Buf[10000];
 

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
  }

  /* Allocate memory for data */
  if ((ObjV = malloc(sizeof(VEC) * nv)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Read vertices */
  rewind(F);
  nv = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &ObjV[nv].X, &ObjV[nv].Y, &ObjV[nv].Z);
      ObjV[nv] = VecMulNum(ObjV[nv], 40);
      ObjV[nv] = VecRotateZ(ObjV[nv], 180);
      nv++;
    }
  }

  fclose(F);

  ObjNumOfV = nv;
  return TRUE;
} /* End of 'ObjLoad' function */

/* END OF 'LOADOBJ.C' FILE */
