#version 410 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uScene;

void main()
{
    FragColor = texture(uScene, vUV);
}
