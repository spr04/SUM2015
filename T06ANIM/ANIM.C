/* FILENAME: ANIM.C
 * PROGRAMMER: AM1
 * PURPOSE: Animation system module.
 * LAST UPDATE: 09.06.2015
 */

#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* ��������� �������� �������� */
am1ANIM AM1_Anim;

/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ��������� �������� ��� ��������� */
#define AM1_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* ����������� ������� ���������� */
static INT
  AM1_MouseOldX, AM1_MouseOldY;

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AM1_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  POINT pt;
  
  memset(&AM1_Anim, 0, sizeof(am1ANIM));
  AM1_Anim.hWnd = hWnd;
  /* �������������� ����� ����� */
  AM1_Anim.hDC = CreateCompatibleDC(hDC);
  AM1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AM1_Anim.hDC, AM1_Anim.hBmFrame);
  AM1_Anim.W = 30;
  AM1_Anim.H = 30;
  AM1_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AM1_Anim.IsPause = FALSE;
  FrameCounter = 0;         

  /* ������������� ����� */
  GetCursorPos(&pt);
  ScreenToClient(AM1_Anim.hWnd, &pt);
  AM1_MouseOldX = pt.x;
  AM1_MouseOldY = pt.y;
  GetKeyboardState(AM1_Anim.KeysOld);
} /* End of 'AM1_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < AM1_Anim.NumOfUnits; i++)
  {
    AM1_Anim.Units[i]->Close(AM1_Anim.Units[i], &AM1_Anim);
    free(AM1_Anim.Units[i]);
    AM1_Anim.Units[i] = NULL;
  }

  /* ������� ������� GDI */
  DeleteObject(AM1_Anim.hBmFrame);
  DeleteDC(AM1_Anim.hDC);
} /* End of 'AM1_AnimClose' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AM1_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AM1_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(AM1_Anim.hBmFrame);
  AM1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AM1_Anim.hDC, AM1_Anim.hBmFrame);

  /* ���������� ������� */
  AM1_Anim.W = W;
  AM1_Anim.H = H;

  ReleaseDC(AM1_Anim.hWnd, hDC);

  /* ������������� ���������� ������������� */
  if (W > H)
    AM1_RndWp = (DBL)W / H * 3, AM1_RndHp = 3;
  else
    AM1_RndHp = (DBL)H / W * 3, AM1_RndWp = 3;
} /* End of 'AM1_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  
  /*** ���������� ������� ***/
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
  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AM1_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� �������� ����� */
  TimeOld = li.QuadPart;

  /*** ���������� ����� ***/
  
  /* ���������� */
  GetKeyboardState(AM1_Anim.Keys);
  for (i = 0; i < 256; i++)
    AM1_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AM1_Anim.KeysClick[i] = AM1_Anim.Keys[i] && !AM1_Anim.KeysOld[i];
  memcpy(AM1_Anim.KeysOld, AM1_Anim.Keys, sizeof(AM1_Anim.KeysOld));

  /* ���� */
  /* ������ */
  AM1_Anim.MsWheel = AM1_MouseWheel;
  AM1_MouseWheel = 0;
  /* ���������� ������� */
  GetCursorPos(&pt);
  ScreenToClient(AM1_Anim.hWnd, &pt);
  AM1_Anim.MsX = pt.x;
  AM1_Anim.MsY = pt.y;
  /* ������������� ����������� */
  AM1_Anim.MsDeltaX = pt.x - AM1_MouseOldX;
  AM1_Anim.MsDeltaY = pt.y - AM1_MouseOldY;
  AM1_MouseOldX = pt.x;
  AM1_MouseOldY = pt.y;
  
  /* �������� */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;
   
    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(AM1_Anim.JButsOld, AM1_Anim.JButs, sizeof(AM1_Anim.JButs));
        for (i = 0; i < 32; i++)
          AM1_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AM1_Anim.JButs[i] = AM1_Anim.JButs[i] && !AM1_Anim.JButsOld[i];
   
        /* ��� */
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
  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < AM1_Anim.NumOfUnits; i++)
    AM1_Anim.Units[i]->Response(AM1_Anim.Units[i], &AM1_Anim);

 /* ������� ���� */
  SelectObject(AM1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AM1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AM1_Anim.hDC, RGB(250, 230, 250));
  Rectangle(AM1_Anim.hDC, 0, 0, AM1_Anim.W + 1, AM1_Anim.H + 1);

   /* ��������� �������� */
  for (i = 0; i < AM1_Anim.NumOfUnits; i++)
  {
    SelectObject(AM1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AM1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AM1_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AM1_Anim.hDC, RGB(0, 0, 0));

    AM1_Anim.Units[i]->Render(AM1_Anim.Units[i], &AM1_Anim);
  }
    
  
} /* End of 'AM1_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(AM1_Anim.hWnd);
  BitBlt(hDC, 0, 0, AM1_Anim.W, AM1_Anim.H, AM1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AM1_Anim.hWnd, hDC);
} /* End of 'AM1_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       am1UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AM1_AnimAddUnit( am1UNIT *Unit )
{
  if (AM1_Anim.NumOfUnits < AM1_MAX_UNITS)
  {
    AM1_Anim.Units[AM1_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AM1_Anim);
  }
} /* End of 'AM1_AnimAddUnit' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(AM1_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(AM1_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
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
    /* ��������������� ������ ���� */
    SetWindowPos(AM1_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AM1_AnimFlipFullScreen' function */




/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID AM1_AnimSetPause( BOOL NewPauseFlag )
{
  AM1_Anim.IsPause = NewPauseFlag;
} /* End of 'AG6_AnimSetPause' function */

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AM1_AnimDoExit( VOID )
{
  PostMessage(AM1_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AM1_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
