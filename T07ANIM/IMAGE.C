/* FILENAME: IMAGE.H
 * PROGRAMMER: AM1
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 05.06.2015
 */


#include <string.h>

#include "image.h"

/* Load image function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName )
{
  HDC hDC, hMemDC;
  BITMAP bm;
  HBITMAP hBm;
  BITMAPINFOHEADER bih = {0};

  memset(Img, 0, sizeof(IMAGE));

  /* Load image from file */
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return FALSE;

  /* Create compatible context and select image into one */
  hDC = GetDC(NULL);
  Img->hDC = CreateCompatibleDC(hDC);
  hMemDC = CreateCompatibleDC(hDC);
  ReleaseDC(NULL, hDC);
  SelectObject(hMemDC, hBm);

  /* Obtain image size */
  GetObject(hBm, sizeof(BITMAP), &bm);
  Img->W = bm.bmWidth;
  Img->H = bm.bmHeight;

  /* строим DIB - device-independed-bitmap секция */
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = bm.bmWidth;
  bih.biHeight = bm.bmHeight;
  bih.biBitCount = 32;
  bih.biCompression = BI_RGB;
  bih.biPlanes = 1;
  bih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;

  Img->hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bih, DIB_RGB_COLORS, &Img->Bits, NULL, 0);
  SelectObject(Img->hDC, Img->hBm);

  /* отображаем в DIB картинку */
  BitBlt(Img->hDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

  DeleteDC(hMemDC);
  DeleteObject(hBm);
  return TRUE;
} /* End of 'ImageLoad' function */

/* Free memory from image function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 * RETURNS: None.
 */
VOID ImageFree( IMAGE *Img )
{
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);
  if (Img->hDC != NULL)
    DeleteDC(Img->hDC);
  memset(Img, 0, sizeof(IMAGE));
} /* End of 'ImageFree' function */

/* Obtain image pixel color function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) TRUE if success, FALSE otherwise.
 */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y )
{
  if (Img->hBm == NULL)
    return 0;

  if (X < 0 || Y < 0 || X >= Img->W || Y >= Img->H)
    return 0;

  return Img->Bits[Y * Img->W + X];
} /* End of 'ImageGetP' function */


/* END OF 'IMAGE.H' FILE */