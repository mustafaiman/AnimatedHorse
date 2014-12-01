#version 150
in  vec4 color;
in vec2 st;
uniform sampler2D texMap;
//uniform vec4 color;
out vec4 fColor;

void main(void)
{
    fColor = color * texture2D(texMap, st);
}
