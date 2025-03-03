#version 430

// VBO-ból érkező változók
layout( location = 0 ) in vec3 vs_in_pos;
layout( location = 1 ) in vec3 vs_in_norm;
layout( location = 2 ) in vec2 vs_in_tex;
layout( location = 3 ) in vec4 instance_position;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out float vs_out_allforce;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 viewProj;

uniform float scaleFactor;

void main()
{
    // Normalize positions for better display
    vec4 normalized_instance_position = vec4(instance_position.xyz / 100, instance_position.w);

    // Compute world transformation matrix (translation + scale)
    mat4 world = mat4(1.0);
    world[0][0] = scaleFactor;
    world[1][1] = scaleFactor;
    world[2][2] = scaleFactor;
    world[3] = vec4(normalized_instance_position.xyz, 1.0); // Apply translation

    // Compute worldIT (transpose of inverse)
    mat4 worldIT = transpose(inverse(mat4(world)));

	gl_Position = viewProj * world * vec4( vs_in_pos, 1 );
	// gl_Position = viewProj * vec4( vs_in_pos, 1 );
	vs_out_pos  = (world   * vec4(vs_in_pos,  1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
    vs_out_allforce = instance_position.w;
}