/* FILENAME: ANIM.H
 * PROGRAMMER: AM1
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "DEF.H"
#include "RENDER.H"

/* ������������ ���������� ��������� �������� */
#define AM1_MAX_UNITS 300

         
/* ���������� ���� �������� �������� "������" */
typedef struct tagam1UNIT am1UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

  /* ������ ��������� �������� � �� ���������� */
  am1UNIT *Units[AM1_MAX_UNITS]; 
  INT NumOfUnits;
  
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */
   
  /* ���������� ����� */
  BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV;/* ��� ��������� (-1.0 .. 1.0*/
} am1ANIM;

extern am1ANIM AM1_Anim;
/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( am1UNIT *Unit, am1ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( am1UNIT *Unit, am1ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( am1UNIT *Unit, am1ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( am1UNIT *Unit, am1ANIM *Ani );
 */
#define AM1_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( am1UNIT *Unit, am1ANIM *Ani );       \
  VOID (*Close)( am1UNIT *Unit, am1ANIM *Ani );      \
  VOID (*Response)( am1UNIT *Unit, am1ANIM *Ani );   \
  VOID (*Render)( am1UNIT *Unit, am1ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagam1UNIT
{
  AM1_UNIT_BASE_FIELDS;
};

/* ���������� ���������� - ������� ��������� ������ ���� */
extern INT AM1_MouseWheel;

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AM1_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AM1_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       am1UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AM1_AnimAddUnit( am1UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (am1UNIT *) ��������� �� ��������� ������ ��������.
 */
am1UNIT * AM1_AnimUnitCreate( INT Size );

VOID AM1_AnimSetPause( BOOL NewPauseFlag );
                              
/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimDoExit( VOID );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */