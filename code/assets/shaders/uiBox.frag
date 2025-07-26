#version 460 core

in vec2 vUV;
in vec2 vSnappedSize;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform mat3 uTransform;
uniform vec2 uTextureSize;   // Size of the texture in pixels
uniform vec4 uBorder;        // left, right, top, bottom (in pixels)

float uScale = uTransform[0][0];

vec2 getTiledUV()
{
    vec2 pixelPos = (vUV * vSnappedSize)/uScale;

    // Split border values
    float l = uBorder.x;
    float r = uBorder.y;
    float t = uBorder.z;
    float b = uBorder.w;

    float centerWidth  = vSnappedSize.x - l - r;
    float centerHeight = vSnappedSize.y - t - b;

    float texL = l / uTextureSize.x;
    float texR = r / uTextureSize.x;
    float texT = t / uTextureSize.y;
    float texB = b / uTextureSize.y;

    float texMidStartX = l;
    float texMidEndX   = uTextureSize.x - r;

    float texMidStartY = b;
    float texMidEndY   = uTextureSize.y - t;

    vec2 tiledUV;

    // Horizontal
    if (pixelPos.x < l) {
        // Left border
        tiledUV.x = pixelPos.x;
    } else if (pixelPos.x > (vSnappedSize.x / uScale - r)) {
        // Right border
        tiledUV.x = uTextureSize.x - (vSnappedSize.x / uScale - pixelPos.x);
    } else {
        // Tiled center or edge
        float tiledRegionWidth = texMidEndX - texMidStartX;
        float xInTiled = mod(pixelPos.x - l, tiledRegionWidth);
        tiledUV.x = texMidStartX + xInTiled;
    }

    // Vertical
    if (pixelPos.y < b) {
        // Bottom border
        tiledUV.y = pixelPos.y;
    } else if (pixelPos.y > (vSnappedSize.y / uScale - t)) {
        // Top border
        tiledUV.y = uTextureSize.y - (vSnappedSize.y / uScale - pixelPos.y);
    } else {
        // Tiled center or edge
        float tiledRegionHeight = texMidEndY - texMidStartY;
        float yInTiled = mod(pixelPos.y - b, tiledRegionHeight);
        tiledUV.y = texMidStartY + yInTiled;
    }

    return tiledUV / uTextureSize;
}

void main()
{
    vec2 tiledUV = getTiledUV();
    FragColor = texture(uTexture, tiledUV);
}
