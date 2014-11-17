#version 150
uniform mat4 ModelView;
uniform mat4 Projection;
in vec4 vPosition;
in vec4 vColor;
out vec4 color;
void main()
{
    color = vColor;
    gl_Position = Projection*ModelView*vPosition;
}
