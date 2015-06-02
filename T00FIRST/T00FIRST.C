#include <stdio.h>
#include <windows.h>

void main( void )
{
  int i;
  int n = MessageBox( NULL, "Applepie?", "Cap", MB_YESNO |  MB_ICONQUESTION);
  if (n = IDYES)
    i = 0;
  else 
    i = 1;
  printf("%i", i);
}
