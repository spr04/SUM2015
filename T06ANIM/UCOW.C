/* FILENAME: UBALL.C
 * PROGRAMMER: AM1
 * PURPOSE: cow unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"
#include "RENDER.H"



/* Тип представления коровы */
typedef struct tagam1UNIT_COW
{
  AM1_UNIT_BASE_FIELDS;

  am1GOBJ Model;
} am1UNIT_COW;

static VOID AM1_AnimUnitInit( am1UNIT_COW *Uni, am1ANIM *Ani )
{
  AM1_RndGObjLoad(&Uni->Model, "cow.object");
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
  AM1_RndGObjFree(&Uni->Model);
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
  if (Ani->Keys['P'])
    AM1_AnimSetPause(TRUE);   


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
  INT i;
  AM1_RndMatrView = MatrView(VecSet(30, 30, 30),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0)); 
  for (i = 0; i < 8; i++)
  {
  AM1_RndMatrWorld = 
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
        MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
        MatrScale(0.1, 0.1, 0.1)),
        MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
        MatrTranslate((i - 2) * 2, 0, 100 * Ani->JZ));
  }
  AM1_RndGObjDraw(&Uni->Model);
} /* End of 'am1_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_UnitModelCreate( VOID )
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
} /* End of 'AM1_UnitCowCreate' function */

/* END OF 'UBALL.C' FILE */
