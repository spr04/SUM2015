#include "GLOBE.H"

/* Функция поворота точки вокруг оси X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */

/* Функция векторного произведения */
VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* Функция вычитания векторов */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  VEC Norm = VecCrossVec(VecSubVec(P3, P0), VecSubVec(P1, P0)); 
  POINT pnts[4];

 /* back-face culling */
  if (Norm.Z > 0)
    return; 

  if(P3.X == P0.X && P3.Y == P0.Y  && P3.Z == P0.Z)
    Norm = VecCrossVec(VecSubVec(P2, P0), VecSubVec(P1, P0));

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;


  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

VOID GlobeBuild( VOID )
{
  INT i, j, R = 88;
  DOUBLE t = clock() / (DOUBLE)CLOCKS_PER_SEC;
  DOUBLE phi, theta;
  for (i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * PI;
    for (j = 0; j < M; j++)
    { 
      phi = j / (M - 1.0) * 2 * PI + t * 0.8 /* PI / 180*/;

      Grid[i][j].X = R * sin(theta) * sin(phi);
      Grid[i][j].Y = 0.888 * R * cos(theta);
      Grid[i][j].Z = R * sin(theta) * cos(phi);
      
      Grid[i][j] = RotateX(Grid[i][j], 30);
    }
  }
}

/* Function that make sphere */
VOID GlobeDraw( HDC hDC, INT W, INT H )
{    
  INT i, j;

  srand(30);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {
      SelectObject(hDC, GetStockObject(NULL_PEN));
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hDC, RGB(rand() & 255, rand() & 255, rand() & 255));
      if (Grid[i][j].Z > 0)
        DrawQuad(hDC, Grid[i][j], Grid[i+1][j], Grid[i+1][j+1], Grid[i][j+1], W, H);
    }
  }
}
/* End of 'Sphere' function */