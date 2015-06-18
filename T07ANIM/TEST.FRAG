/* Filename: TEST.FRAG: */

// версия языка шейдера
#version 330

// время
uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;

// выходные параметры - цвет рисования
out vec4 OutColor;

// входные параметры от вершинного шейдера
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/*** Освещение ***/
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Kp;
uniform float Kt;

/* Текстура */
uniform float IsTextureUse;
uniform sampler2D DrawTexture;


/* Функция вычисления освещенности */
vec4 Shade( vec3 P, vec3 N )
{
  float a = 0.5;
  vec3 color = vec3(0, 0, 0);
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 V = -vec3(-MatrView[3][0], -MatrView[3][1], -MatrView[3][2]);

  V = -normalize(P - vec3(MatrWVInverse * vec4(0, 0, 0, 1)));
  
  // разворачиваем нормаль, если смотрим в "тыл" треугольнику
  N = faceforward(N, V, N);
  N = normalize(N);

 
  vec3
    Ka = vec3(1, 0, 0),
    Kd = vec3(0.4, 0.4, 0.4),
    Ks = vec3(2, 0.9, 0.5);
  float Kp = 40;   
   
  

  // рассеянный (ambient)
  color = Ka ;

  // диффузное отражение
  float nl = dot(N, L); 
  if (nl > 0)
  {
    if (IsTextureUse > 0)
    {
      vec4 tc = texture2D(DrawTexture/*, vec2(1, -1) - vec2(-1, 1) */, DrawTexCoord);
     // return tc;
      a = tc.a;
      color += Kd * tc.rgb * nl;
    }
    else
      color += Kd * nl;
  }

  // зеркальное отражение
  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    color += Ks * 2 * pow(rl, Kp);

  return vec4(0.5 * color, (Kt * a));
}

void main( void )
{
  /*/ туман
  vec4 pos = MatrView * vec4(DrawPosition, 1);
  float d = -pos.z, f, start = 10, end = 18;

  if (d < start)
    f = 1;
  else if (d > end)
    f = 0;
  else
    f = (d - end) / (start - end);
 
  vec4 fcol = vec4(0.3, 0.5, 0.7, 1);
  */
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal); /* f + fcol * (1 - f);*/
  //OutColor = vec4(1, 0, 0, 1);
}
 