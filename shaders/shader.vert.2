#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTextureCoordinate;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTextureCoordinate;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTextureCoordinate = inTextureCoordinate;
}
