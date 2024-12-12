
#version 460 core

in vec2 tex_coord;
out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // Sample the texture using tex_coord
    vec4 tex_color = texture(texture2, tex_coord);

    // Output the color
    color = tex_color;
}