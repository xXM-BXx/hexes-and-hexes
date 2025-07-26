#version 460 core


in vec2 vUV;

out vec4 FragColor;

uniform vec2 uAtlasOffset;   // Offset of region in atlas (in pixels)
uniform vec2 uRegionSize;    // Size of region (in pixels)
uniform vec2 uAtlasSize;     // Full atlas size (in pixels)
uniform sampler2D uTexture;

void main()
{
    vec2 atlasUV = (uAtlasOffset + vUV * uRegionSize) / uAtlasSize;
    FragColor = texture(uTexture, atlasUV);
}


