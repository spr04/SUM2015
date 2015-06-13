/* FILENAME: RENDER.H
 * PROGRAMMER: AM1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagam1GOBJ
{
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfV,      /* количество вершин */
    NumOfF;      /* количество граней */
} am1GOBJ;

/* Матрицы */
extern MATR
  AM1_RndMatrWorld,
  AM1_RndMatrView;

/* Параметры проецирования */
extern DBL
  AM1_RndWp, AM1_RndHp,    /* размеры обрасти проецирования */
  AM1_RndProjDist;         /* расстояние до плоскости проекции */


/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT AM1_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       am1GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AM1_RndGObjLoad( am1GOBJ *GObj, CHAR *FileName );

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       am1GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_RndGObjFree( am1GOBJ *GObj );

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       am1GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_RndGObjDraw( am1GOBJ *GObj );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
