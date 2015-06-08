/* FILENAME: UBALL.C
 * PROGRAMMER: PD6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"


/* Тип представления мяча */
typedef struct tagam1UNIT_BALL
{
  AM1_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift; /* Масштаб времени */
} am1UNIT_BALL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitInit( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AM1_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitClose( am1UNIT_BALL *Uni, am1ANIM *Ani )
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
static VOID AM1_AnimUnitResponse( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    AM1_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    AM1_AnimFlipFullScreen();
  if (GetAsyncKeyState('P') & 0x8000)
    AM1_AnimSetPause(TRUE);
  if (GetAsyncKeyState('O') & 0x8000)
    AM1_AnimSetPause(FALSE);

} /* End of 'AM1_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitRender( am1UNIT_BALL *Uni, am1ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'am1_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_UnitBallCreate( VOID )
{
  am1UNIT_BALL *Uni;

  if ((Uni = (VOID *)AM1_AnimUnitCreate(sizeof(am1UNIT_BALL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AM1_AnimUnitInit;
  Uni->Close = (VOID *)AM1_AnimUnitClose;
  Uni->Response = (VOID *)AM1_AnimUnitResponse;
  Uni->Render = (VOID *)AM1_AnimUnitRender;
  return (am1UNIT *)Uni;
} /* End of 'AM1_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
