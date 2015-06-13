/* FILENAME: UCONTRL.C
 * PROGRAMMER: AM1
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagam1UNIT_CTRL
{
  AM1_UNIT_BASE_FIELDS;
  HFONT hFnt; /* Шрифт для вывода FPS */
} am1UNIT_CTRL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitInit( am1UNIT_CTRL *Uni, am1ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'AM1_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitClose( am1UNIT_CTRL *Uni, am1ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'AM1_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitResponse( am1UNIT_CTRL *Uni, am1ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AM1_AnimDoExit();
  if (Ani->KeysClick['F'])
    AM1_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AM1_AnimSetPause(!Ani->IsPause);
} /* End of 'AM1_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitRender( am1UNIT_CTRL *Uni, am1ANIM *Ani )
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
} /* End of 'AM1_AnimUnitRender' function */

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_UnitControlCreate( VOID )
{
  am1UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AM1_AnimUnitCreate(sizeof(am1UNIT_CTRL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AM1_AnimUnitInit;
  Uni->Close = (VOID *)AM1_AnimUnitClose;
  Uni->Response = (VOID *)AM1_AnimUnitResponse;
  Uni->Render = (VOID *)AM1_AnimUnitRender;
  return (am1UNIT *)Uni;
} /* End of 'AM1_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
