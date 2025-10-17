#version 410 core

in vec3 vColor;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uAlbedo;

void main()
{
    vec3 albedo = texture(uAlbedo, vUV).rgb;
    FragColor = vec4(albedo * vColor, 1.0);
}
