/* FILENAME: RENDER.H
 * PROGRAMMER: AM1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagam1GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} am1GOBJ;

/* ������� */
extern MATR
  AM1_RndMatrWorld,
  AM1_RndMatrView;

/* ��������� ������������� */
extern DBL
  AM1_RndWp, AM1_RndHp,    /* ������� ������� ������������� */
  AM1_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT AM1_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       am1GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AM1_RndGObjLoad( am1GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       am1GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AM1_RndGObjFree( am1GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       am1GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AM1_RndGObjDraw( am1GOBJ *GObj );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
