#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 3) in vec2 aTexCoord;

layout(std140, binding = 0) uniform Matrices {
    mat4 proj;
};

uniform mat4 model;

out VS_OUT {
    vec4 vertColor;
    vec2 texCoord;
} vs_out;

void main() {
    gl_Position = proj * model * vec4(aPos, 1);
    vs_out.vertColor = aColor;
    vs_out.texCoord = aTexCoord;
}

