/* Filename: TEST.FRAG: */

// ������ ����� �������
#version 330

// �����
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

// �������� ��������� - ���� ���������
out vec4 OutColor;

// ������� ��������� �� ���������� �������
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/*** ��������� ***/
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Kp;
uniform float Kt;

/* �������� */
uniform float IsTextureUse;
uniform sampler2D DrawTexture;


/* ������� ���������� ������������ */
vec4 Shade( vec3 P, vec3 N )
{
  float a = 0.8;
  vec3 color = vec3(0, 0, 0);
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 V = -vec3(-MatrView[3][0], -MatrView[3][1], -MatrView[3][2]);

  V = -normalize(P - vec3(MatrWVInverse * vec4(0, 0, 0, 1)));
  
  // ������������� �������, ���� ������� � "���" ������������
  N = faceforward(N, V, N);
  N = normalize(N);

  
  vec3
    Ka = vec3(0.3, 0.1, 0.1),
    Kd = vec3(0.4, 0.4, 0.4),
    Ks = vec3(2, 0.9, 0.5);
  float Kp = 40;      
    
   

  // ���������� (ambient)
  color = Ka * 1;

  // ��������� ���������
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

  // ���������� ���������
  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    color += Ks * 3 * pow(rl, Kp);

  return vec4(0.6 * color, (Kt * a));
}

void main( void )
{
  // �����
 vec4 pos = MatrView * vec4(DrawPosition, 1);
  float d = -pos.z, f, start = 10, end = 18;

  if (d < start)
    f = 1;
  else if (d > end)
    f = 0;
  else
    f = (d - end) / (start - end);
 
  vec4 fcol = vec4(0.3, 0.5, 0.5, 1);
  
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal) /* f + fcol * (1 - f)*/;
//  OutColor = vec4(1, 1, 0, 1);                             
}
 