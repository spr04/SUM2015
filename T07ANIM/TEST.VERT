/* Filename: TEST.VERT */

// версия языка шейдера
#version 330

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// глобальные переменные
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;
uniform float Time;

// выходные параметры
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main( void )
{
  float a = 2 * Time, s = sin(a),  c = cos(a);
  mat4 M =
  mat4(
    vec4(c, -s, 0,  0),
    vec4(s,  c, 0, 0),
    vec4(0,  0, 1, 0),
    vec4(0,  0, 0, 1)
  );        
 // gl_Position = MatrWVP * M * vec4((InPosition) * 0.4, 1);
 // gl_Position = MatrWVP  * vec4((InPosition) * 0.4, 1);

  gl_Position = MatrWVP * vec4(InPosition + vec3(0,  0.8 * sin(Time * 4 + 4 * InPosition.y) * cos(Time +  0.05 * InPosition.z), 1), 1);
  DrawNormal = mat3(MatrWInverse) * InNormal;
//  DrawPosition = vec3(sin(Time + InPosition.x) * cos(Time + InPosition.z) * 0.3, 0);

  DrawPosition = vec3(MatrWorld * vec4(InPosition, 1));
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}
