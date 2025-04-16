#version 430

// Base geometry input
layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec2 vs_in_tex;

// Output to fragment shader
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out float vs_out_allforce;
out vec3 vs_out_color;
out float vs_out_size;

// Uniforms
uniform mat4 viewProj;
uniform vec3 position;
uniform vec3 color;
uniform float scale;
uniform float scaleFactor;

void main()
{
    float instanceSize = scaleFactor * scale;

    vec3 normalizedInstancePosition = position / 100.0;

    mat4 world = mat4(1.0);
    world[0][0] = instanceSize;
    world[1][1] = instanceSize;
    world[2][2] = instanceSize;
    world[3] = vec4(normalizedInstancePosition, 1.0);

    mat4 worldIT = transpose(inverse(world));

    gl_Position = viewProj * world * vec4(vs_in_pos, 1.0);
    vs_out_pos  = (world   * vec4(vs_in_pos, 1)).xyz;
    vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
    vs_out_tex = vs_in_tex;
    vs_out_allforce = 0.0;
    vs_out_color = color;
    vs_out_size = instanceSize;
}