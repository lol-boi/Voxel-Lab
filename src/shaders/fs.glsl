
#version 460 core

in vec2 tex_coord;
flat in int texture_index;
out vec4 color;

uniform sampler2DArray texture_array;

void main()
{

    // Sample the texture using tex_coord
    vec4 tex_color = texture(texture_array, vec3(tex_coord,float(texture_index)));

    // Output the color
    color = tex_color;
}
