#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

void main() {
    gl_Position = vProjection * vView * vModel * vec4(vPos, 1.0);
	fTexCoord = vTexCoord;
};