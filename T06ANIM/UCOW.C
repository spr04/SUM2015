/* FILENAME: UBALL.C
 * PROGRAMMER: AM1
 * PURPOSE: cow unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"
#include "UNITS.H"


/* Тип представления мяча */
typedef struct tagam1UNIT_BALL
{
  AM1_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
} am1UNIT_COW;

static VOID AM1_AnimUnitInit( am1UNIT_COW *Uni, am1ANIM *Ani )
{
  ObjLoad("cow.object");
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
} /* End of 'AM1_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitClose( am1UNIT_COW *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitResponse( am1UNIT_COW *Uni, am1ANIM *Ani )
{
  if (Ani->KeysClick[VK_ESCAPE])
    AM1_AnimDoExit();
  if (Ani->KeysClick['F'])
    AM1_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AM1_AnimSetPause(TRUE);  /*   */


} /* End of 'AM1_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitRender( am1UNIT_COW *Uni, am1ANIM *Ani )
{
  SetDCBrushColor(Ani->hDC, Uni->Color);
  ObjDraw(Ani->hDC, Ani->H, Ani->W);
} /* End of 'am1_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_UnitCowCreate( VOID )
{
  am1UNIT_COW *Uni;

  if ((Uni = (VOID *)AM1_AnimUnitCreate(sizeof(am1UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AM1_AnimUnitInit;
  Uni->Close = (VOID *)AM1_AnimUnitClose;
  Uni->Response = (VOID *)AM1_AnimUnitResponse;
  Uni->Render = (VOID *)AM1_AnimUnitRender;
  return (am1UNIT *)Uni;
} /* End of 'AM1_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
