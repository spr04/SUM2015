/* FILENAME: UNITS.C
 * PROGRAMMER: AM1
 * PURPOSE: linking for files.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (am1UNIT *) ��������� �� ��������� ������ ��������.
 */
//am1UNIT * AM1_UnitBallCreate( VOID );

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (am1UNIT *) ��������� �� ��������� ������ ��������.
 */
am1UNIT * AM1_UnitControlCreate( VOID );

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (am1UNIT *) ��������� �� ��������� ������ ��������.
 */
am1UNIT * AM1_UnitModelCreate( VOID );

am1UNIT * AM1_UnitSeaCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */