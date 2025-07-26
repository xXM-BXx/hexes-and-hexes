#version 460 core

// Attributes
layout(location = 0) in vec2 aPos; // Quad vertex (-1 to 1 or 0 to 1)
layout(location = 1) in vec2 aUV;

// Outputs
out vec2 vUV;
out vec2 vSnappedSize;

// Uniforms
uniform mat4 uProjection;     // Orthographic projection matrix
uniform mat3 uTransform;
uniform vec2 uBoxSize;        // Original (un-snapped) size in pixels
uniform vec2 uTextureSize;    // Texture size in pixels
uniform vec4 uBorder;         // Border: left, right, top, bottom (pixels)

float uScale = uTransform[0][0];
vec2 uPosition = uTransform[2].xy;


vec2 getSnappedBoxSize() {
    vec2 borderSize = vec2(uBorder.x + uBorder.y, uBorder.z + uBorder.w) * uScale;
    vec2 tileSize = (uTextureSize - vec2(uBorder.x + uBorder.y, uBorder.z + uBorder.w)) * uScale;
    vec2 centerArea = max(vec2(0.0), uBoxSize - borderSize);

    //Enforce minimum center size: at least one tile
    vec2 minCenter = tileSize;
    centerArea = max(centerArea, minCenter);
    //Round up so all tiles fit
    vec2 snappedCenter = ceil(centerArea / tileSize) * tileSize;

    return snappedCenter + borderSize;
}

void main() {
    vec2 snappedSize = getSnappedBoxSize();

    // Transform aPos * snappedSize by uTransform
    vec2 pixelPos = uPosition + aPos * snappedSize;

    // Project to clip space
    gl_Position = uProjection * vec4(pixelPos, 0.0, 1.0);
    vSnappedSize = snappedSize;
    vUV = aUV; // pass through UV
}