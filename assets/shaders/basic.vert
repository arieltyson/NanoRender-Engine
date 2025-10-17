#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(std140) uniform FrameData
{
    mat4 uViewProjection;
    float uTime;
    vec3 uPadding;
};

uniform mat4 uModel;

out vec3 vColor;

void main()
{
    gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
    vec3 baseColor = normalize(aNormal * 0.5 + 0.5);
    float pulse = 0.5 + 0.5 * sin(uTime);
    vColor = mix(baseColor, vec3(0.4, 0.6, 0.9), pulse);
}
