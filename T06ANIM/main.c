/* FILENAME: MAIN.C
 * PROGRAMMER: AM1
 * PURPOSE: Animation startup module
 * LAST UPDATE: 09.06.2015
 */

#include "anim.h"
#include "units.h"

#define WND_CLASS_NAME "My Window Class Name"

/* ���������� ���������� - ������� ��������� ������ ���� */
INT AM1_MouseWheel;

/* ������ ������ */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 * ���������:
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 *   - ���� ������ ���� (��. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  INT i;
  WNDCLASSEX wc;
  HWND hWnd;
  MSG msg;

  /* ����������� - �������� ������������ ������ ���� */
  wc.cbSize = sizeof(WNDCLASSEX);      /* ������ ��������� ��� ������������� */
  wc.style = CS_VREDRAW | CS_HREDRAW;  /* ����� ����: ��������� ��������������
                                        * ��� ��������� ������������� ���
                                        * ��������������� �������� (��� CS_DBLCLKS) */
  wc.cbClsExtra = 0;                   /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0;                   /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;      /* ������� ���� - ��������� � ������� */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);     /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   /* �������� ����������� (���������) */
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* �������� ����� ����������� (���������) */
  wc.lpszMenuName = NULL;                       /* ��� ������� ���� */
  wc.hInstance = hInstance;                     /* ���������� ����������, ��������������� ����� */
  wc.lpfnWndProc = MainWindowFunc;              /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;            /* ��� ������ */

  /* ������������ ����� */
  if (!RegisterClassEx(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,          /* ����� ���� - ��������������� */
    CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
    CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
    NULL,                         /* ���������� ������������� ���� */
    NULL,                         /* ���������� ������������ ���� */
    hInstance,                    /* ���������� ���������� */
    NULL);                        /* ��������� �� �������������� ��������� */

  /* ���������� � ��������� ����������� */
  ShowWindow(hWnd, ShowCmd);
  /* ���������� ���������� */
  UpdateWindow(hWnd);

  /*** ���������� �������� ***/
  for (i = 0; i < 300; i++)
    AM1_AnimAddUnit(AM1_UnitBallCreate());

  /* ������ ����� ��������� ��������� */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* �������������� ��������� ��������� �� ���������� (->WM_CHAR) */
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    AM1_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    AM1_AnimResize(LOWORD(lParam), HIWORD(lParam));
    AM1_AnimRender();
    return 0;
  case WM_TIMER:
    AM1_AnimRender();
    AM1_AnimCopyFrame();
    return 0;
  case WM_MOUSEWHEEL:
    AM1_MouseWheel += (SHORT)HIWORD(wParam) / WHEEL_DELTA;
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    AM1_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    AM1_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MainWindowFunc' function */

/* END OF 'STARTUP.C' FILE */


