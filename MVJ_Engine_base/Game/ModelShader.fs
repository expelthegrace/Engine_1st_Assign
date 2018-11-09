#version 330 core

out vec4 color;

in vec2 uv0;

uniform sampler2D texture0;
uniform int drawTexture;

void main()
{
    if (drawTexture == 1) color = texture2D(texture0, uv0);
	else color = vec4(1,1,1,1);
}
