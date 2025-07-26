#version 460

in vec2 vUV;

uniform sampler2D uTexture;
uniform vec4 uColor;

out vec4 FragColor;

void main() {
    FragColor = uColor * texture(uTexture, vUV);
}
