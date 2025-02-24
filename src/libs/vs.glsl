#version 460 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in int packed_instance_data;
layout(std430, binding = 0) buffer ChunkPositions {
    vec4 chunk_positions[];
};

out vec2 tex_coord;

uniform int max_instances;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    //uint chunk_offset = gl_BaseInstance / max_instances;
    //vec3 chunk_coords = chunk_positions[chunk_offset].xyz;
    vec3 chunk_coords = chunk_positions[gl_DrawID].xyz;

    //unpacking the data
    float pos_x = packed_instance_data & 31;
    pos_x += chunk_coords.x * 32;
    float pos_y = (packed_instance_data >> 5) & 31;
    pos_y += chunk_coords.y * 32;
    float pos_z = (packed_instance_data >> 10) & 31;
    pos_z += chunk_coords.z * 32;
    int normal = (packed_instance_data >> 15) & 7;
    int texture = (packed_instance_data >> 18) & 15;
    int height = (packed_instance_data >> 22) & 31;
    int width = (packed_instance_data >> 27) & 31;

    vec3 face_pos = vec3(pos_x, pos_y, pos_z);
    vec3 rotated_pos = vertex_pos;

    tex_coord = vec2(vertex_pos.x, vertex_pos.y) + vec2(0.5, 0.5);
    tex_coord.y = ((texture - 1) + tex_coord.y) * .0625; //calculating v coordinates

    //roating the base face model/mesh and also calcluateing v coordinates
    if (normal == 1) { //front this is like this maybe cause the +z and -z normalisation is diferent in opengl as to the
        rotated_pos = vertex_pos;
        if (tex_coord.x == 0) {
            tex_coord.x = .5;
        }
        else {
            tex_coord.x = .75;
        }
        if (rotated_pos.x > 0) rotated_pos.x += width;
        if (rotated_pos.y > 0) rotated_pos.y += height;
    } else if (normal == 0) { //back
        rotated_pos = vec3(vertex_pos.x, vertex_pos.y, -vertex_pos.z);
        if (tex_coord.x == 0) {
            tex_coord.x = .5;
        }
        else {
            tex_coord.x = .75;
        }
        if (rotated_pos.x > 0) rotated_pos.x += width;
        if (rotated_pos.y > 0) rotated_pos.y += height;
    } else if (normal == 2) { //left
        rotated_pos = vec3(-vertex_pos.z, vertex_pos.y, -vertex_pos.x);
        if (tex_coord.x == 0) {
            tex_coord.x = .75;
        }
        else {
            tex_coord.x = 1;
        }
        if (rotated_pos.z > 0) rotated_pos.z += width;
        if (rotated_pos.y > 0) rotated_pos.y += height;
    } else if (normal == 3) { //right
        rotated_pos = vec3(vertex_pos.z, vertex_pos.y, -vertex_pos.x);
        if (tex_coord.x == 0) {
            tex_coord.x = .75;
        }
        else {
            tex_coord.x = 1;
        }
        if (rotated_pos.z > 0) rotated_pos.z += width;
        if (rotated_pos.y > 0) rotated_pos.y += height;
    } else if (normal == 4) { //bottom
        rotated_pos = vec3(vertex_pos.x, -vertex_pos.z, -vertex_pos.y);
        if (tex_coord.x == 0) {
            tex_coord.x = .25;
        }
        else {
            tex_coord.x = .5;
        }

        if (rotated_pos.z > 0) rotated_pos.z += width;
        if (rotated_pos.x > 0) rotated_pos.x += height;
    } else if (normal == 5) { //top
        rotated_pos = vec3(vertex_pos.x, vertex_pos.z, vertex_pos.y);
        tex_coord.x *= .25;
        if (rotated_pos.z > 0) rotated_pos.z += width;
        if (rotated_pos.x > 0) rotated_pos.x += height;
    }
    vec3 final_pos = face_pos + rotated_pos;

    gl_Position = projection * view * model * vec4(final_pos, 1.0);
}
