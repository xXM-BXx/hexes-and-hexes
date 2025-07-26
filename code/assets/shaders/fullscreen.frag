#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    vec4 scene = texture(tex1, TexCoord);
    vec4 ui = texture(tex2, TexCoord);

    // Alpha blending: UI over Scene
    FragColor = mix(scene, ui, ui.a);
}