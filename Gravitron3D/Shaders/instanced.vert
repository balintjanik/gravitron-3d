#version 430

// Base geometry input
layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec2 vs_in_tex;

// Instancing data
layout(location = 3) in vec4 instancePositionMass;
layout(location = 4) in vec4 instanceVelocitySize;
layout(location = 5) in vec4 instanceAccelerationForce;
layout(location = 6) in vec4 instanceColorMovable;

// Output to fragment shader
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out float vs_out_allforce;
out vec3 vs_out_color;
out float vs_out_size;

// Uniforms
uniform mat4 viewProj;
uniform float scaleFactor;

void main()
{
    // Create World and WorldIT matrices
    vec3 instancePosition = instancePositionMass.xyz;
    float instanceScale = instanceVelocitySize.w;
    float instanceForce = instanceAccelerationForce.w;
    vec3 instanceColor = instanceColorMovable.xyz;
    float instanceSize = scaleFactor * instanceScale;
    vec3 normalizedInstancePosition = instancePosition / 100.0;

    mat4 world = mat4(
        vec4(instanceSize, 0.0, 0.0, 0.0),
        vec4(0.0, instanceSize, 0.0, 0.0),
        vec4(0.0, 0.0, instanceSize, 0.0),
        vec4(normalizedInstancePosition, 1.0)
    );
    mat4 worldIT = transpose(inverse(world));

    // Outputs
    gl_Position = viewProj * world * vec4(vs_in_pos, 1.0);
    vs_out_pos  = (world   * vec4(vs_in_pos, 1)).xyz;
    vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
    vs_out_tex = vs_in_tex;
    vs_out_allforce = instanceForce;
    vs_out_color = instanceColor;
    vs_out_size = instanceSize;
}