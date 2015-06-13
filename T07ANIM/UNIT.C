/* FILENAME: UNIT.C
 * PROGRAMMER: AM1
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitInit( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitClose( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitResponse( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitRender( am1UNIT *Uni, am1ANIM *Ani )
{
} /* End of 'AM1_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_AnimUnitCreate( INT Size )
{
  am1UNIT *Uni;

  if (Size < sizeof(am1UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* заполняем поля по-умолчанию */
  Uni->Size = Size;
  Uni->Init = AM1_AnimUnitInit;
  Uni->Close = AM1_AnimUnitClose;
  Uni->Response = AM1_AnimUnitResponse;
  Uni->Render = AM1_AnimUnitRender;
  return Uni;
} /* End of 'AM1_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
