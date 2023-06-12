#version 400

// Natężenie składowej diffuse i ambient:
const vec3 LigthDiffuse = vec3(0.8, 0.8, 0.8);
const vec3 LightAmbient = vec3(0.2, 0.2, 0.2);
// Współczynnik rozbłysków materiału:
const vec3 MatSpecular = vec3(0.4, 0.4, 0.4);
const int MatShininess = 24;

// Stały wektor "patrzenia" obserwatora wzdłuż osi Z:
const vec3 lookvec = vec3(0, 0, 1);
// Położenie źródła światła - dane przekazane z aplikacji:
uniform vec3 LightPos;

// Interpolowany wektor normalny:
in vec3 normal;
// Interpolowana współrzędna tekstury:
in vec2 tex_coord;
// Wynikowy kolor piksela 
out vec4 frag_colour;
// Sampler dla tekstury:
uniform sampler2D Tex0;

in vec3 v_position;

void main()
{
	// Wektor padania światła (źródło kierunkowe):
	vec3 lightDir = normalize(LightPos);
	// Normalizacja interpolowanej normalnej:
	vec3 n = normalize(normal);
	// Jasność (diffuse):
	float brightness = max( dot(lightDir, n), 0.0 );

	// Model oświetlenia Blinna-Phonga:
	vec3 HV = vec3( normalize(lightDir + lookvec) );
	float NdotHV = max(dot(n, HV), 0.0);
	float spec = pow(NdotHV, MatShininess);

	// Teksel tekstury:
	vec4 texel = texture2D(Tex0, tex_coord);

	// Kolor wynikowy z teksturą (bez rozbłysków):
	vec4 color = texel * vec4(brightness * LigthDiffuse + LightAmbient, 1.0);
	//color = mix(vec4(skyColor, 1.0), color, visibility);
	// Rozbłyski lub nie:
	if(brightness > 0.0 && spec > 0.0)
			frag_colour = clamp(color + spec * vec4(MatSpecular, 1.0), 0.0, 1.0);
	else	frag_colour = clamp(color, 0.0, 1.0);


}
