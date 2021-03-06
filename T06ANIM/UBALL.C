/* FILENAME: UBALL.C
 * PROGRAMMER: AM1
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"


/* ��� ������������� ���� */
typedef struct tagam1UNIT_BALL
{
  AM1_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� ���� */
  DWORD Color; /* ���� ���� */
  DBL
    Amplitude,  /* ��������� */
    PhaseShift, /* ����� �� ���� */
    ScaleShift; /* ������� ������� */
} am1UNIT_BALL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitInit( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AM1_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitClose( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitResponse( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
  if (Ani->KeysClick[VK_ESCAPE])
    AM1_AnimDoExit();
  if (Ani->KeysClick['F'])
    AM1_AnimFlipFullScreen(); 
  if (Ani->KeysClick['P'])
    AM1_AnimSetPause(TRUE);
  if (Ani->KeysClick['O'])
    AM1_AnimSetPause(FALSE);

  Uni->Amplitude += 30 * Ani->MsWheel;

} /* End of 'AM1_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitRender( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'am1_AnimUnitRender' function */

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (am1UNIT *) ��������� �� ��������� ������ ��������.
 */
am1UNIT * AM1_UnitBallCreate( VOID )
{
  am1UNIT_BALL *Uni;

  if ((Uni = (VOID *)AM1_AnimUnitCreate(sizeof(am1UNIT_BALL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)AM1_AnimUnitInit;
  Uni->Close = (VOID *)AM1_AnimUnitClose;
  Uni->Response = (VOID *)AM1_AnimUnitResponse;
  Uni->Render = (VOID *)AM1_AnimUnitRender;
  return (am1UNIT *)Uni;
} /* End of 'AM1_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
