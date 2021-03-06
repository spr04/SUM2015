/* FILENAME: UNIT.C
 * PROGRAMMER: AM1
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitInit( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitClose( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitResponse( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       am1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       am1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AM1_AnimUnitRender( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (am1UNIT *) ��������� �� ��������� ������ ��������.
 */
am1UNIT * AM1_AnimUnitCreate( INT Size )
{
  am1UNIT *Uni;

  if (Size < sizeof(am1UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ���� ��-��������� */
  Uni->Size = Size;
  Uni->Init = AM1_AnimUnitInit;
  Uni->Close = AM1_AnimUnitClose;
  Uni->Response = AM1_AnimUnitResponse;
  Uni->Render = AM1_AnimUnitRender;
  return Uni;
} /* End of 'AM1_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
