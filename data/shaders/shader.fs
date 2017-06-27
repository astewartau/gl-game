#version 330 core

in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D fTexture1;
uniform sampler2D fTexture2;

void main() {
	FragColor = mix(texture(fTexture1, fTexCoord), texture(fTexture2, fTexCoord), 0.2);
};