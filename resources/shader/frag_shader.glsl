#version 460 core

in VS_OUT {
    vec4 vertColor;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;

void main() {
    fragColor = fs_out.vertColor;
}

