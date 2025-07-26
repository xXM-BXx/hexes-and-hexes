#version 460 core
layout (location = 0) in vec2 aPos; // Normalized quad: [0,0] to [1,1]
layout (location = 1) in vec2 aUV;  // [0,0] to [1,1]

uniform mat3 uTransform;
uniform mat4 uProjection;
uniform vec2 uRegionSize;     // Size of the sprite region in pixels (e.g. 16x16)
uniform vec2 uRegionOrigin;   // Pivot inside the region (e.g. 8x8)

float uScale = uTransform[0][0];
vec2 uPosition = uTransform[2].xy;

out vec2 vUV;

void main()
{
    // Offset quad so pivot is at (0,0), then scale to size
    vec2 localPos = (aPos * uRegionSize) - uRegionOrigin;

    // Translate to screen position
    vec2 worldPos = uPosition + localPos * uScale;

    // Project to clip space
    gl_Position = uProjection * vec4(worldPos, 0.0, 1.0);

    // Pass through [0–1] UVs (no atlas sampling here)
    vUV = aUV;
}