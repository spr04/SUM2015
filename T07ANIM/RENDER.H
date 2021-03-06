/* FILENAME: RENDER.H
 * PROGRAMMER: AM1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ������� */
extern MATR
  AM1_RndMatrWorld,
  AM1_RndMatrView,
  AM1_RndMatrProj,
  AM1_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR AM1_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  AM1_RndWp, AM1_RndHp,    /* ������� ������� ������������� */
  AM1_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT AM1_RndProg;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT AM1_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID AM1_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagam1MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} am1MATERIAL;


/* ���������� ������� ���������� */
#define AM1_MATERIAL_MAX 300
extern INT AM1_MtlLibSize;
extern am1MATERIAL AM1_MtlLib[AM1_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       am1MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT AM1_MtlAdd( am1MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT AM1_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT AM1_MtlLoad( CHAR *FileName );

/***
 * ������ � �����������
 ***/

/* ���� ���������� */
typedef enum tagam1PRIM_TYPE
{
  AM1_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  AM1_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} am1PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagam1PRIM
{
  am1PRIM_TYPE Type; /* ��� ��������� (AM1_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI,      /* ���������� �������� ��� ������ ��������� */
    MtlNo;       /* ����� ��������� �� ���������� */
} am1PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagam1UV
{
  FLT U, V; /* ���������� ���������� */
} am1UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (am1UV) ����������� ����������.
 */
__inline am1UV UVSet( FLT U, FLT V )
{
  am1UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagam1VERTEX
{
  VEC P;    /* ������� ������� */
  am1UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} am1VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       am1PRIM *Prim;
 *   - ��� ��������� (AM1_PRIM_***):
 *       am1PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       am1VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID AM1_PrimCreate( am1PRIM *Prim, am1PRIM_TYPE Type,
                     INT NoofV, INT NoofI, am1VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       am1PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AM1_PrimFree( am1PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       am1PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AM1_PrimDraw( am1PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       am1PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AM1_PrimCreatePlane( am1PRIM *Prim,VEC Loc, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       am1PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AM1_PrimCreateSphere( am1PRIM *Prim, VEC C, FLT R, INT N, INT M );

BOOL AM1_PrimCreateHeightField( am1PRIM *Prim, VEC Loc, VEC Du, VEC Dv, DBL Scale, CHAR *FileName );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       am1PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AM1_PrimLoad( am1PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagam1GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  am1PRIM *Prims;      /* ������ ���������� */
} am1GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       am1PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT AM1_GeomAddPrim( am1GEOM *G, am1PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 * ������������ ��������: ���.
 */
VOID AM1_GeomFree( am1GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 * ������������ ��������: ���.
 */
VOID AM1_GeomDraw( am1GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       am1GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL AM1_GeomLoad( am1GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT AM1_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
