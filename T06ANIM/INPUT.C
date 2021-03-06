ANIM.H

vg4ANIM:
  . . .

  /* ���������� ����� */
  BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0*/
. . .
���-�� ����

/* ���������� ���������� - ������� ��������� ������ ���� */
extern INT VG4_MouseWheel;

MAIN.C
. . .
/* ���������� ���������� - ������� ��������� ������ ���� */
INT VG4_MouseWheel;
. . .
MainWindowFunc:
  . . .
  case WM_MOUSEWHEEL:
    VG4_MouseWheel += (SHORT)HIWORD(wParam) / WHEEL_DELTA;
    return 0;
  . . .

ANIM.C
#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* ��������� �������� ��� ��������� */
#define VG4_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* ����������� ������� ���������� */
static INT
  VG4_MouseOldX, VG4_MouseOldY;

VG4_AnimInit:
  . . .
  POINT pt;
  
  memset(&VG4_Anim, 0, sizeof(vg4ANIM));
  . . .
  /* ������������� ����� */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_MouseOldX = pt.x;
  VG4_MouseOldY = pt.y;
  GetKeyboardState(VG4_Anim.KeysOld);

VG4_AnimRender:
  . . .
  POINT pt;
  . . .
  /*** ���������� ����� ***/

  /* ���������� */
  GetKeyboardState(VG4_Anim.Keys);
  for (i = 0; i < 256; i++)
    VG4_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    VG4_Anim.KeysClick[i] = VG4_Anim.Keys[i] && !VG4_Anim.KeysOld[i];
  memcpy(VG4_Anim.KeysOld, VG4_Anim.Keys, sizeof(VG4_Anim.KeysOld));

  /* ���� */
  /* ������ */
  VG4_Anim.MsWheel = VG4_MouseWheel;
  VG4_MouseWheel = 0;
  /* ���������� ������� */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.MsX = pt.x;
  VG4_Anim.MsY = pt.y;
  /* ������������� ����������� */
  VG4_Anim.MsDeltaX = pt.x - VG4_MouseOldX;
  VG4_Anim.MsDeltaY = pt.y - VG4_MouseOldY;
  VG4_MouseOldX = pt.x;
  VG4_MouseOldY = pt.y;

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
        memcpy(VG4_Anim.JButsOld, VG4_Anim.JButs, sizeof(VG4_Anim.JButs));
        for (i = 0; i < 32; i++)
          VG4_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          VG4_Anim.JButs[i] = VG4_Anim.JButs[i] && !VG4_Anim.JButsOld[i];

        /* ��� */
        VG4_Anim.JX = VG4_GET_AXIS_VALUE(X);
        VG4_Anim.JY = VG4_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          VG4_Anim.JZ = VG4_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          VG4_Anim.JU = VG4_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          VG4_Anim.JV = VG4_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          VG4_Anim.JR = VG4_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            VG4_Anim.JPOV = 0;
          else
            VG4_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
  . . .
