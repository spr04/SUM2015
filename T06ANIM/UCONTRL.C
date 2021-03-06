/* FILENAME: UCONTRL.C
 * PROGRAMMER: VG4
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagvg4UNIT_CTRL
{
  VG4_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
} vg4UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitInit( vg4UNIT_CTRL *Uni, vg4ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'VG4_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitClose( vg4UNIT_CTRL *Uni, vg4ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'VG4_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitResponse( vg4UNIT_CTRL *Uni, vg4ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    VG4_AnimDoExit();
  if (Ani->KeysClick['F'])
    VG4_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    VG4_AnimSetPause(!Ani->IsPause);
} /* End of 'VG4_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitRender( vg4UNIT_CTRL *Uni, vg4ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  RECT rc;
  static CHAR Buf[1000];

  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);

  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);

  SelectObject(Ani->hDC, hFntOld);
} /* End of 'VG4_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_UnitControlCreate( VOID )
{
  vg4UNIT_CTRL *Uni;

  if ((Uni = (VOID *)VG4_AnimUnitCreate(sizeof(vg4UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)VG4_AnimUnitInit;
  Uni->Close = (VOID *)VG4_AnimUnitClose;
  Uni->Response = (VOID *)VG4_AnimUnitResponse;
  Uni->Render = (VOID *)VG4_AnimUnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
