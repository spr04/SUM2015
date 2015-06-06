      * тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;


__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR M =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1}
    }
  };

  return M;
}


__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR M =
  {
    {
      { Sx,  0,  0, 0},
      { 0,  Sy,  0, 0},
      { 0,  0,  Sz, 0},
      { 0,  0,  0,  1}
    }
  };

  return M;
}

__inline MATR MatrRotateX( DBL AngleDegree )
{
  DBL a, sine, cosine, len;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }

  MATR M =
  {
    {
      { Sx,  0,  0, 0},
      { 0,  Sy,  0, 0},
      { 0,  0,  Sz, 0},
      { 0,  0,  0,  1}
    }
  };

  return M;
}
__inline MATR MatrRotateY( DBL AngleDegree );
__inline MATR MatrRotateZ( DBL AngleDegree ); 

__inline MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z )
{
  DBL a, sine, cosine, len;
  MATR M;


  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  /* correct source parameters */
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;

  /* Build quaternion matrix */
  X *= sine;
  Y *= sine;
  Z *= sine;

  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * cosine * Z;
  M.A[0][2] = 2 * cosine * Y + 2 * X * Z;
  M.A[0][3] = 0;

  M.A[1][0] = 2 * X * Y + 2 * cosine * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * cosine * X;
  M.A[1][3] = 0;

  M.A[2][0] = 2 * X * Z - 2 * cosine * Y;
  M.A[2][1] = 2 * cosine * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;

  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR M;

  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
}

__inline MATR MatrTranspose( MATR M )
{
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
        M.A[i][j] = M.A[j][i];
  return M;
}

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,                            
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return MatrIdentity();

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)  
      r.A[j][i] =
        (1 - ((i + j) % 2) * 2) *
        MatrDeterm3x3(M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
                      M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
                      M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;

  return r;
}

/*Векторы  */

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

__inline VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} 

__inline VEC VecMulNum( VEC A, DBL N )
{
  VecSet(A.X * N, A.Y * N, A.Z * N);
}

__inline EC VecDivNum( VEC A, DBL N )
{
  VecSet(A.X / N, A.Y / N, A.Z / N);
}

__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}

/* Функция скалярного произведения векторов */
__inline DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}/* End of 'VeDotVec' function */

/* Функция векторного произведения */
__inline VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
}/* End of 'VecCrossVec' function */

/* Функция, ктр возвращает квадрат длины вектора */
__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}/* End of 'VecLen2' function */

__inline DBL VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

__inline VEC VecNormalize( VEC V )
{
  DBL L = VecLen(V);
  VEC r;
  r.X = V.X / L;
  r.Y = V.Y / L;
  r.Z = V.Z / L;
  return r; 
}

__inline VEC VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}

/* для нормалей: *
MATR Q = MatrTranspose(MatrInverse(M));
N1 = VectorTransform(N, Q); */

__inline VEC VecRotateZ( VEC V, DBL AngleInDegree )
{
  DBL sine, cosine, tmp;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  tmp = V.X * cosine - V.Y * sine;
  V.Y = V.X * sine + V.Y * cosine;
  V.X = tmp;
  return V;
} /* End of 'VecRotateZ' function */

__inline MATR MatrSetRotateZ( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  DBL sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[0][0] =  M.A[1][1] = cosine;
  M.A[0][1] = sine;
  M.A[1][0] = -sine;
  return M;
}

__inline MATR MatrSetRotate( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  DBL sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[0][0] =  M.A[1][1] = cosine;
  M.A[0][1] = sine;
  M.A[1][0] = -sine;
  return M;
}

#endif 
/* END OF 'VEC.H' FILE */