#version 460

// Attributes
layout(location = 0) in vec2 aPos; // Quad vertex (-1 to 1 or 0 to 1)
layout(location = 1) in vec2 aUV;

// Outputs
out vec2 vUV;

// Uniforms
uniform mat4 uProjection;     // Orthographic projection matrix
uniform mat3 uTransform;


float uScale = uTransform[0][0];
vec2 uPosition = uTransform[2].xy;

void main() {

    // Transform aPos * snappedSize by uTransform
    vec2 pixelPos = uPosition + aPos * uScale;

    // Project to clip space
    gl_Position = uProjection * vec4(pixelPos, 0.0, 1.0);
    vUV = aUV; // pass through UV
}
