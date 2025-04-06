#version 430

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in float vs_out_allforce;
in vec3 vs_out_color;
in float vs_out_size;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

// textúra mintavételező objektum
uniform sampler2D texImage;

uniform vec3 cameraPos;

// fenyforras tulajdonsagok
uniform vec4 lightPos = vec4( 0.0, -1.0, 0.0, 0.0);

uniform vec3 La = vec3(0.2, 0.2, 0.2 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );

uniform float lightConstantAttenuation    = 0.0;
uniform float lightLinearAttenuation      = 1.0;
uniform float lightQuadraticAttenuation   = 0.0;

// anyag tulajdonsagok

vec3 Ka = vec3( 1.0 );
vec3 Kd = vec3( 1.0 );
vec3 Ks = vec3( 1.0 );

float Shininess = 8.0;

/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
				reflect(beérkező_vektor, normálvektor);
		- pow: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
				pow(alap, kitevő);
*/

uniform int colorType;

uniform float minVal = 0.0;  // Adjust this to the minimum expected value
uniform float maxVal = 100.0;  // Adjust this to the maximum expected value

vec3 valueToColor(float value)
{
    value = clamp(value, minVal, maxVal);
    
    // Normalize value to [0, 1]
    value = (value - minVal) / (maxVal - minVal);

    // Blue to Green (0 -> 0.25)
    if (value <= 0.25)
    {
        float mixFactor = value / 0.25;
        return mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0), mixFactor);  // Interpolate from blue to green
    }
    // Green to Yellow (0.25 -> 0.5)
    else if (value <= 0.5)
    {
        float mixFactor = (value - 0.25) / 0.25;
        return mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0), mixFactor);  // Interpolate from green to yellow
    }
    // Yellow to Red (0.5 -> 1)
    else
    {
        float mixFactor = (value - 0.5) / 0.5;
        return mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), mixFactor);  // Interpolate from yellow to red
    }
}

void main()
{
	vec4 normalizedLightPos = vec4(lightPos.xyz / 100, lightPos.w);

	if (colorType == 2) {
		fs_out_col = vec4(1.0, 1.0, 1.0, 0.3);
		return;
	}
	else if (colorType == 1) {
		Ka = valueToColor(vs_out_allforce);
		Kd = valueToColor(vs_out_allforce);
		Ks = valueToColor(vs_out_allforce);
	}
	else {
		Ka = vs_out_color;
		Kd = vs_out_color;
		Ks = vs_out_color;
	}

	vec3 ambient = La * Ka;

	vec3 normal = normalize(vs_out_norm);
	vec3 toLight;

	float lightDistance = 0.0;
	float attenuation = 1.0;
	float diffuseIntensity = 0.0;

	if (normalizedLightPos.w == 0.5)
	{
		fs_out_col = vec4((Ka + Kd + Ks) * 0.5, 1.0) * texture(texImage, vs_out_tex);
		return;
	}
	else if (normalizedLightPos.w == 0.0) //Irány fényforrás, xyz -> irány
	{
		toLight = -normalize(normalizedLightPos.xyz);
	}
	else if (normalizedLightPos.w == 1.0) //Pont fényforrás, xyz -> pont
	{
		toLight = normalize(normalizedLightPos.xyz - vs_out_pos);
		lightDistance = length(normalizedLightPos.xyz - vs_out_pos);
		attenuation = 1.0 /
			(lightConstantAttenuation +
			lightLinearAttenuation * lightDistance +
			lightQuadraticAttenuation * lightDistance * lightDistance);

		if (lightDistance <= vs_out_size + 0.1) {
			diffuseIntensity = 1.0;
		}
	}

	//diffuseFactor
	if (diffuseIntensity == 0.0)
		diffuseIntensity = max(dot(toLight, normal), 0) * attenuation;
	vec3 diffuse = Ld * Kd * diffuseIntensity;

	//Spekulálris becsillanás
	vec3 r = reflect(-toLight, normal);
	vec3 c = normalize(cameraPos - vs_out_pos);
	float specularIntensity = pow(max(dot(c, r), 0), Shininess) * attenuation;
	vec3 specular = Ls * Ks * specularIntensity;

	fs_out_col = vec4(ambient + diffuse + specular, 1.0) * texture(texImage, vs_out_tex);
}