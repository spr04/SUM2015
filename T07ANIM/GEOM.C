/* FILENAME: GEOM.C
 * PROGRAMMER: AM1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       am1PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT AM1_GeomAddPrim( am1GEOM *G, am1PRIM *Prim )
{
  am1PRIM *new_bulk;

  /* �������� ������ ��� ����� ������ ���������� */
  if ((new_bulk = malloc(sizeof(am1PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* �������� ������ ��������� */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(am1PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* ��������� �� ����� ������ ���������� */
  G->Prims = new_bulk;

  /* ��������� ����� ������� */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'AM1_GeomAddPrim' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 * ������������ ��������: ���.
 */
VOID AM1_GeomFree( am1GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      AM1_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(am1GEOM));
} /* End of 'AM1_GeomFree' function */

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 * ������������ ��������: ���.
 */
VOID AM1_GeomDraw( am1GEOM *G )
{
  INT i, loc;

  /* �������� ���������� ������ */
  glUseProgram(AM1_RndProg);
  loc = glGetUniformLocation(AM1_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  /* ������ ������������ ������� */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (AM1_MtlLib[G->Prims[i].MtlNo].Kt == 1)
    {
      /* �������� ����� ������� ����� */
      glUseProgram(AM1_RndProg);
      loc = glGetUniformLocation(AM1_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      AM1_PrimDraw(&G->Prims[i]);
    }

  /* ������ ���������� ������� */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (AM1_MtlLib[G->Prims[i].MtlNo].Kt != 1)
    {
      /* �������� ����� ������� ����� */
      glUseProgram(AM1_RndProg);
      loc = glGetUniformLocation(AM1_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      AM1_PrimDraw(&G->Prims[i]);
    }
} /* End of 'AM1_GeomDraw' function */

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL AM1_GeomLoad( am1GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];
  static CHAR
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  memset(G, 0, sizeof(am1GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(AM1_RndPrimMatrConvert));

  /* ������ ��������� */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* ������ ���������� ���������� � ������� */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* ������ � ��������� ���������� ���������� */
  _makepath(path_buffer, drive, dir, MtlName, "");
  AM1_MtlLoad(path_buffer);

  /* ������ ��������� */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    am1VERTEX *Vert;
    am1PRIM P;

    /* ������ ���������� ������ � �������� */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* ������ ��� ��������� �������� ��������� */
    fread(MtlName, 1, 300, F);

    /* �������� ������ ��� ������� � ������� */
    if ((Vert = malloc(sizeof(am1VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* ������ ������ */
    fread(Vert, sizeof(am1VERTEX), nv, F);
    /* ������������ ��������� */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = VecMulMatr(Vert[j].P, AM1_RndPrimMatrConvert);
      Vert[j].N = VecMulMatr3(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* ������� � �������� */
    AM1_PrimCreate(&P, AM1_PRIM_TRIMESH, nv, ni, Vert, Ind);
    P.MtlNo = AM1_MtlFind(MtlName);

    free(Vert);

    /* ��������� �������� � ������� */
    AM1_GeomAddPrim(G, &P);
  }
  fclose(F);
  AM1_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'AM1_GeomDraw' function */

/* END OF 'GEOM.C' FILE */


