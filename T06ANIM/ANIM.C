/* FILENAME: ANIM.C
 * PROGRAMMER: AM1
 * PURPOSE: Animation system module.
 * LAST UPDATE: 09.06.2015
 */

#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Системный контекст анимации */
static am1ANIM AM1_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Получение значения оси джойстика */
#define AM1_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Сохраненные мышиные координаты */
static INT
  AM1_MouseOldX, AM1_MouseOldY;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  POINT pt;
  
  memset(&AM1_Anim, 0, sizeof(am1ANIM));
  AM1_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  AM1_Anim.hDC = CreateCompatibleDC(hDC);
  AM1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AM1_Anim.hDC, AM1_Anim.hBmFrame);
  AM1_Anim.W = 30;
  AM1_Anim.H = 30;
  AM1_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AM1_Anim.IsPause = FALSE;
  FrameCounter = 0;         

  /* Инициализация ввода */
  GetCursorPos(&pt);
  ScreenToClient(AM1_Anim.hWnd, &pt);
  AM1_MouseOldX = pt.x;
  AM1_MouseOldY = pt.y;
  GetKeyboardState(AM1_Anim.KeysOld);
} /* End of 'AM1_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < AM1_Anim.NumOfUnits; i++)
  {
    AM1_Anim.Units[i]->Close(AM1_Anim.Units[i], &AM1_Anim);
    free(AM1_Anim.Units[i]);
    AM1_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(AM1_Anim.hBmFrame);
  DeleteDC(AM1_Anim.hDC);
} /* End of 'AM1_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AM1_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(AM1_Anim.hBmFrame);
  AM1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AM1_Anim.hDC, AM1_Anim.hBmFrame);

  /* Сохранение размера */
  AM1_Anim.W = W;
  AM1_Anim.H = H;

  ReleaseDC(AM1_Anim.hWnd, hDC);
} /* End of 'AM1_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

   /* очистка фона */
  SelectObject(AM1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AM1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AM1_Anim.hDC, RGB(250, 230, 250));
  Rectangle(AM1_Anim.hDC, 0, 0, AM1_Anim.W + 1, AM1_Anim.H + 1);

   /* рисование объектов */
  for (i = 0; i < AM1_Anim.NumOfUnits; i++)
  {
    SelectObject(AM1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AM1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AM1_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AM1_Anim.hDC, RGB(0, 0, 0));

    AM1_Anim.Units[i]->Render(AM1_Anim.Units[i], &AM1_Anim);
  }

  
  /*** Обновление таймера ***/
  QueryPerformanceCounter(&li);
  AM1_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AM1_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!AM1_Anim.IsPause)
    AM1_Anim.DeltaTime = AM1_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    AM1_Anim.DeltaTime = 0;
  }

  AM1_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq; 

  /*** Обновление ввода ***/

  /* Клавиатура */
  GetKeyboardState(AM1_Anim.Keys);
  for (i = 0; i < 256; i++)
    AM1_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AM1_Anim.KeysClick[i] = AM1_Anim.Keys[i] && !AM1_Anim.KeysOld[i];
  memcpy(AM1_Anim.KeysOld, AM1_Anim.Keys, sizeof(AM1_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  AM1_Anim.MsWheel = AM1_MouseWheel;
  AM1_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(AM1_Anim.hWnd, &pt);
  AM1_Anim.MsX = pt.x;
  AM1_Anim.MsY = pt.y;
  /* относительное перемещение */
  AM1_Anim.MsDeltaX = pt.x - AM1_MouseOldX;
  AM1_Anim.MsDeltaY = pt.y - AM1_MouseOldY;
  AM1_MouseOldX = pt.x;
  AM1_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(AM1_Anim.JButsOld, AM1_Anim.JButs, sizeof(AM1_Anim.JButs));
        for (i = 0; i < 32; i++)
          AM1_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AM1_Anim.JButs[i] = AM1_Anim.JButs[i] && !AM1_Anim.JButsOld[i];

        /* Оси */
        AM1_Anim.JX = AM1_GET_AXIS_VALUE(X);
        AM1_Anim.JY = AM1_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          AM1_Anim.JZ = AM1_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          AM1_Anim.JU = AM1_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          AM1_Anim.JV = AM1_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          AM1_Anim.JR = AM1_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AM1_Anim.JPOV = 0;
          else
            AM1_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
  /* опрос на изменение состояний объектов */
  for (i = 0; i < AM1_Anim.NumOfUnits; i++)
    AM1_Anim.Units[i]->Response(AM1_Anim.Units[i], &AM1_Anim);
     
  
} /* End of 'AM1_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(AM1_Anim.hWnd);
  BitBlt(hDC, 0, 0, AM1_Anim.W, AM1_Anim.H, AM1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AM1_Anim.hWnd, hDC);
} /* End of 'AM1_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       am1UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimAddUnit( am1UNIT *Unit )
{
  if (AM1_Anim.NumOfUnits < AM1_MAX_UNITS)
  {
    AM1_Anim.Units[AM1_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AM1_Anim);
  }
} /* End of 'AM1_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(AM1_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(AM1_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AM1_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AM1_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(AM1_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AM1_AnimFlipFullScreen' function */




/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimSetPause( BOOL NewPauseFlag )
{
  AM1_Anim.IsPause = NewPauseFlag;
} /* End of 'AG6_AnimSetPause' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AM1_AnimDoExit( VOID )
{
  PostMessage(AM1_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AM1_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
