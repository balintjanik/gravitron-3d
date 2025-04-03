#version 430

// VBO-ból érkező változók
layout( location = 0 ) in vec3 vs_in_pos;
layout( location = 1 ) in vec3 vs_in_norm;
layout( location = 2 ) in vec2 vs_in_tex;
layout( location = 3 ) in vec4 instancePositionMass;
layout( location = 4 ) in vec4 instanceVelocitySize;
layout( location = 5 ) in vec4 instanceAccelerationForce;
layout( location = 6 ) in vec4 instanceColorMovable;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out float vs_out_allforce;
out vec3 vs_out_color;
out float vs_out_size;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform bool isSingleObject;
uniform vec3 position;
uniform float scale;
uniform mat4 viewProj;

uniform float scaleFactor;

void main()
{
    vec3 instancePosition;
    float instanceScale;
    float instanceForce;
    vec3 instanceColor;
    float instanceSize;
    
    if (isSingleObject) {
        instancePosition = position;
        instanceScale = scale;
        instanceForce = 0;
        instanceColor = vec3(1);
    } else {
        instancePosition = instancePositionMass.xyz;
        instanceScale = instanceVelocitySize.w;
        instanceForce = instanceAccelerationForce.w;
        instanceColor = instanceColorMovable.xyz;
    }
    instanceSize = scaleFactor * instanceScale;

    // Normalize positions for better display
    vec3 normalizedInstancePosition = vec3(instancePosition / 100);

    // Compute world transformation matrix (translation + scale)
    mat4 world = mat4(1.0);
    world[0][0] = instanceSize;
    world[1][1] = instanceSize;
    world[2][2] = instanceSize;
    world[3] = vec4(normalizedInstancePosition, 1.0); // Apply translation

    // Compute worldIT (transpose of inverse)
    mat4 worldIT = transpose(inverse(mat4(world)));

	gl_Position = viewProj * world * vec4( vs_in_pos, 1 );
	// gl_Position = viewProj * vec4( vs_in_pos, 1 );
	vs_out_pos  = (world   * vec4(vs_in_pos,  1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
    vs_out_allforce = instanceForce;
    vs_out_color = instanceColor;
    vs_out_size = instanceSize;
}