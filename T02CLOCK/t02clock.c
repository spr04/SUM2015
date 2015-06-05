/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: Anya Mitina
 * DATE: 02.06.2015
 * PURPOSE: clock visualisation.
 */

#pragma warning(disable: 4244)

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <windows.h>

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"
#define PI 3.1415926535897932384

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� ��������
                                       * ��� ����� CS_DBLCLKS ��� ����������
                                       * ��������� �������� ������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      600, 700,                     /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* ��������� ��������� �� ���������� */
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */


/* ������� ����� ��������� � �������� */
/*VOID DrawArrow(  HDC hDC, INT X1, INT Y1, INT Len, DOUBLE Angle )
{
  DOUBLE si = sin(Angle * PI / 180), co = cos(Angle * PI / 180);
  MoveToEx(hDC, X1, Y1, NULL);
  LineTo(hDC, X1 + Len * si, Y1 - Len * co);

}/* End of 'DrawArrow' function */

/* ������� ����� ��������� � �������� */
VOID DrawHand(  HDC hDC, INT X1, INT Y1, INT L, INT W, FLOAT Angle )
{
  INT i;
  //DOUBLE si = sin(Angle * PI / 180), co = cos(Angle * PI / 180);
  POINT pnts[]=
  {
    {X1, -W}, {-W, Y1}, {X1, L}, {W, Y1}
  }, pntdraw[sizeof pnts / sizeof pnts[0]];

  for (i = 0; i < sizeof pnts / sizeof pnts[0]; i++)
  {
    pntdraw[i].x = pnts[i].x * cos(Angle) - pnts[i].y * sin(Angle);
    pntdraw[i].y = pnts[i].x * sin(Angle) + pnts[i].y * cos(Angle);
  }
  Polygon(hDC, pnts, 4);

 /* DOUBLE si = sin(Angle * PI / 180), co = cos(Angle * PI / 180);
  MoveToEx(hDC, X1, Y1, NULL);
  LineTo(hDC, X1 + Len * si, Y1 - Len * co);  */

}/* End of 'DrawArrow' function */


/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  INT x, y;
  CREATESTRUCT *cs;
  //POINT pt;
 // PAINTSTRUCT ps;
  SYSTEMTIME st;
  CHAR Buf[100];
  HFONT hFnt, hOldFnt;
 // RECT rc;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);

    /*hBmLogo = LoadImage(NULL, "clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);*/

    /* ������� �������� � ������ */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCLogo, hBmLogo);
    SetPixel(hMemDCLogo, 10, 10, RGB(255, 0, 0));
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* ������� �������� �������� � ���� */
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 111, 0);
    return 0;

  case WM_TIMER:
    /* Clear Background */
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 225, 225));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    /*StretchBlt(hMemDC, 0, 0, w, h - 100,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);*/

    GetLocalTime(&st);
    

    hFnt = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "");
    hOldFnt = SelectObject(hMemDC, hFnt);
    GetLocalTime(&st);
    SetTextColor(hMemDC, RGB(0, 0, 0));
    SetBkColor(hMemDC, RGB(255, 255, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    TextOut(hMemDC, w / 6, h - 80, Buf,
      sprintf(Buf, "%02d:%02d:%02d (%02d.%02d.%d)",
        st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear));

    DeleteObject(hFnt);

    hFnt = CreateFont(50, 0, 0, 0, FW_BOLD, TRUE, FALSE,
      TRUE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_SWISS, "");
    SelectObject(hMemDC, hFnt);

    SelectObject(hMemDC, hOldFnt);
    DeleteObject(hFnt);

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    DrawHand(hMemDC, w / 2, h / 2 - 50, 200, 30, (st.wSecond * 6) * PI / 180 );
    /*DrawArrow(hMemDC, w / 2, h / 2 - 50, 200, st.wMinute * 6);
    DrawArrow(hMemDC, w / 2, h / 2 - 50, 120, (st.wHour % 12) * 30); */
    

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);

    x = LOWORD(lParam);
    y = HIWORD(lParam);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_MOUSEMOVE:
    x = (SHORT)LOWORD(lParam);
    y = (SHORT)HIWORD(lParam);
    return 0;

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmLogo);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T02CLOCK.C' FILE */