/* FILENAME: ANIM.H
 * PROGRAMMER: AM1
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "DEF.H"
#include "RENDER.H"

/* Максимальное количество элементов анимации */
#define AM1_MAX_UNITS 300

         
/* Объявление типа объектов анимации "вперед" */
typedef struct tagam1UNIT am1UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст окна вывода */
  HGLRC hGLRC;        /* Контекст рендеринга */

  /* Массив элементов анимации и их количество */
  am1UNIT *Units[AM1_MAX_UNITS]; 
  INT NumOfUnits;
  
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */
   
  /* Подсистема ввода */
  BYTE
    Keys[256],       /* Сотояние клавиш клавиатуры и мыши */
    KeysOld[256],    /* Сотояние клавиш на предыдущем кадре */
    KeysClick[256];  /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты курсора мыши */
    MsX, MsY,        /* Абсолютные координаты курсора мыши */
    MsWheel;         /* Состояние колеса мыши */
  BYTE
    JButs[32],       /* Сотояние кнопок джойстика */
    JButsOld[32],    /* Сотояние кнопок джойстика на предыдущем кадре */
    JButsClick[32],  /* Флаги однократного нажатия кнопок джойстика */
    JPOV;            /* Переключатель POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV;/* Оси джойстика (-1.0 .. 1.0*/
} am1ANIM;

extern am1ANIM AM1_Anim;
/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( am1UNIT *Unit, am1ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( am1UNIT *Unit, am1ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( am1UNIT *Unit, am1ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( am1UNIT *Unit, am1ANIM *Ani );
 */
#define AM1_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( am1UNIT *Unit, am1ANIM *Ani );       \
  VOID (*Close)( am1UNIT *Unit, am1ANIM *Ani );      \
  VOID (*Response)( am1UNIT *Unit, am1ANIM *Ani );   \
  VOID (*Render)( am1UNIT *Unit, am1ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagam1UNIT
{
  AM1_UNIT_BASE_FIELDS;
};

/* Глобальная переменная - счетчик прокрутки колеса мыши */
extern INT AM1_MouseWheel;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimInit( HWND hWnd )
{
}

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       am1UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimAddUnit( am1UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (am1UNIT *) указатель на созданный объект анимации.
 */
am1UNIT * AM1_AnimUnitCreate( INT Size );

VOID AM1_AnimSetPause( BOOL NewPauseFlag );
                              
/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimDoExit( VOID );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */