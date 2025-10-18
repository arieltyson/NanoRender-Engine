#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(std140) uniform FrameData
{
    mat4 uViewProjection;
    mat4 uView;
    vec4 uCameraPositionTime;
    vec4 uLightDirection;
    vec4 uLightColor;
};

uniform mat4 uModel;

out vec3 vWorldPos;
out vec3 vNormal;
out vec2 vUV;

void main()
{
    vec4 worldPosition = uModel * vec4(aPosition, 1.0);
    gl_Position = uViewProjection * worldPosition;
    vWorldPos = worldPosition.xyz;
    mat3 normalMatrix = mat3(uModel);
    vNormal = normalize(normalMatrix * aNormal);
    vUV = aTexCoord;
}
