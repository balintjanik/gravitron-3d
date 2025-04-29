#version 430

// Inputs from vertex shader
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in float vs_out_allforce;
in vec3 vs_out_color;
in float vs_out_size;

// Output
out vec4 fs_out_col;

// Uniforms
uniform sampler2D texImage;
uniform vec3 cameraPos;

uniform vec4 lightPos = vec4( 0.0, -1.0, 0.0, 0.0);
uniform vec3 La = vec3(0.2, 0.2, 0.2 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );
uniform float lightConstantAttenuation    = 0.0;
uniform float lightLinearAttenuation      = 1.0;
uniform float lightQuadraticAttenuation   = 0.0;

vec3 Ka = vec3( 1.0 );
vec3 Kd = vec3( 1.0 );
vec3 Ks = vec3( 1.0 );
float Shininess = 8.0;
uniform int colorType;
uniform vec3 negativeColor;

uniform float minVal = 0.0;
uniform float maxVal = 100.0;

// Calculate a color based on force value
vec3 valueToColor(float value)
{
    value = clamp(value, minVal, maxVal);
    
    // Normalize value to [0, 1]
    value = (value - minVal) / (maxVal - minVal);

    // Blue to Green (0 - 0.25)
    if (value <= 0.25) {
        float mixFactor = value / 0.25;
        return mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0), mixFactor);
    }
    // Green to Yellow (0.25 - 0.5)
    else if (value <= 0.5) {
        float mixFactor = (value - 0.25) / 0.25;
        return mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0), mixFactor);
    }
    // Yellow to Red (0.5 - 1.0)
    else {
        float mixFactor = (value - 0.5) / 0.5;
        return mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), mixFactor);
    }
}

void main()
{
	vec4 normalizedLightPos = vec4(lightPos.xyz / 100, lightPos.w);

	// Fixed negative color relative to background with small transparency
	if (colorType == 2) {
		fs_out_col = vec4(negativeColor, 0.4);
		return;
	}
	// Color based on force
	else if (colorType == 1) {
		Ka = valueToColor(vs_out_allforce);
		Kd = valueToColor(vs_out_allforce);
		Ks = valueToColor(vs_out_allforce);
	}
	// Original color based on color attribute of object
	else {
		Ka = vs_out_color;
		Kd = vs_out_color;
		Ks = vs_out_color;
	}

	// Light attributes
	vec3 ambient = La * Ka;
	vec3 normal = normalize(vs_out_norm);
	vec3 toLight;

	float lightDistance = 0.0;
	float attenuation = 1.0;
	float diffuseIntensity = 0.0;

	// Constant light type
	if (normalizedLightPos.w == 0.5)
	{
		fs_out_col = vec4((Ka + Kd + Ks) * 0.5, 1.0) * texture(texImage, vs_out_tex);
		return;
	}
	// Directional light type (xyz direction)
	else if (normalizedLightPos.w == 0.0)
	{
		toLight = -normalize(normalizedLightPos.xyz);
	}
	// Point light type (xyz point)
	else if (normalizedLightPos.w == 1.0)
	{
		toLight = normalize(normalizedLightPos.xyz - vs_out_pos);
		lightDistance = length(normalizedLightPos.xyz - vs_out_pos);
		attenuation = 1.0 /
			(lightConstantAttenuation +
			lightLinearAttenuation * lightDistance +
			lightQuadraticAttenuation * lightDistance * lightDistance);

		// Constant light if light is inside object
		if (lightDistance <= vs_out_size + 0.1) {
			diffuseIntensity = 1.0;
		}
	}

	// Diffuse factor
	if (diffuseIntensity == 0.0)
		diffuseIntensity = max(dot(toLight, normal), 0) * attenuation;
	vec3 diffuse = Ld * Kd * diffuseIntensity;

	// Specular factor
	vec3 r = reflect(-toLight, normal);
	vec3 c = normalize(cameraPos - vs_out_pos);
	float specularIntensity = pow(max(dot(c, r), 0), Shininess) * attenuation;
	vec3 specular = Ls * Ks * specularIntensity;

	// Final color
	fs_out_col = vec4(ambient + diffuse + specular, 1.0) * texture(texImage, vs_out_tex);
}