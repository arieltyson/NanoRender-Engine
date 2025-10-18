#version 410 core

layout(std140) uniform FrameData
{
    mat4 uViewProjection;
    mat4 uView;
    vec4 uCameraPositionTime;
    vec4 uLightDirection;
    vec4 uLightColor;
};

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uAlbedo;

void main()
{
    vec3 baseColor = texture(uAlbedo, vUV).rgb;
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(-uLightDirection.xyz);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

    vec3 viewDirection = normalize(uCameraPositionTime.xyz - vWorldPos);
    vec3 halfVector = normalize(lightDirection + viewDirection);
    float specularFactor = pow(max(dot(normal, halfVector), 0.0), 32.0);

    float ambient = uLightColor.w;
    vec3 lighting = ambient * baseColor;
    lighting += diffuseFactor * uLightColor.xyz * baseColor;
    lighting += specularFactor * uLightColor.xyz;
    lighting = clamp(lighting, 0.0, 1.0);

    FragColor = vec4(lighting, 1.0);
}
