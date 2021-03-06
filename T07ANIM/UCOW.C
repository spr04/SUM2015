/* FILENAME: UBALL.C
 * PROGRAMMER: AM1
 * PURPOSE: cow unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <stdio.h>
#include "anim.h"

/* Тип представления коровы */
typedef struct tagam1UNIT_MODEL
{
  AM1_UNIT_BASE_FIELDS;
  
  am1GEOM Model;
  am1GEOM Geom;
  am1PRIM Pr;
  INT TextId;
} am1UNIT_MODEL;

static VOID AM1_AnimUnitInit( am1UNIT_MODEL *Uni, am1ANIM *Ani )
{
    am1VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  /* */
  //Uni->TextId = AM1_TextureLoad("M.BMP");
  //AM1_PrimCreatePlane(&Uni->Pr, DU, DV, 30, 30); 
  //AM1_PrimCreate(&Uni->Pr, AM1_PRIM_TRIMESH, 4, 6, V, I);
  AM1_RndPrimMatrConvert = MatrMulMatr(MatrScale(0.3, 0.3, 0.3), MatrRotateX(-90));
  //AM1_GeomLoad(&Uni->Geom, "X6.G3D");
  AM1_GeomLoad(&Uni->Model, "Z:\\SUM2014\\T07ANIM\\BOAT.G3D");
 
} /* End of 'AM1_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitClose( am1UNIT_MODEL *Uni, am1ANIM *Ani )
{
  AM1_GeomFree(&Uni->Model);
  AM1_GeomFree(&Uni->Geom);
  AM1_PrimFree(&Uni->Pr);
} /* End of 'AM1_AnimUnitClose' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       am1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       am1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AM1_AnimUnitRender( am1UNIT_MODEL *Uni, am1ANIM *Ani )
{
  INT i, j;
  CHAR str[256];
  AM1_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0)); 
  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  sprintf(str, "%lf %lf %lf lf %lf %lf", Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV);
  SetWindowText(Ani->hWnd, str);   

  
  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      AM1_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
        MatrTranslate(Ani->Keys['a'] * 30, 0, Ani->Keys['d'] * 30),
          MatrScale(1, 1, 1)),
          MatrRotate(-90, 1, 0, 1)),
          MatrTranslate(0, Ani->Keys['w'] * 30, 0));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      AM1_GeomDraw(&Uni->Model);
    }   
  //AM1_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  AM1_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
 // AM1_PrimDraw(&Uni->Pr);
} /* End of 'am1_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_UnitModelCreate( VOID )
{
  am1UNIT_MODEL *Uni;

  if ((Uni = (VOID *)AM1_AnimUnitCreate(sizeof(am1UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AM1_AnimUnitInit;
  Uni->Close = (VOID *)AM1_AnimUnitClose;
  Uni->Render = (VOID *)AM1_AnimUnitRender;
  return (am1UNIT *)Uni;
} /* End of 'AM1_UnitCowCreate' function */

/* END OF 'UBALL.C' FILE */
