#version 400

// Dane wejściowe - współrzędne, normalne i współrzędne tekstury:
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt;
// Macierze transformacji i ich iloczyn:
uniform mat4 MatM, MatV, MatMVP;
out mat4 MatV_translate;

// Wektor normalny do interpolacji:
out vec3 normal;
// Współrzędna tekstury do interpolacji:
out vec2 tex_coord;

out vec3 v_position;

void main() 
{
	// Przekazanie współrzędnej tekstury:
	tex_coord = vt;
	// Transformacja wektora normalnego:
	normal = (MatV * MatM * vec4(vn, 0)).xyz;
	// Transformacja i rzutowanie wierzchołka:
	gl_Position = MatMVP * vec4(vp, 1.0);
	MatV_translate = MatM;
	v_position  = -(MatMVP * MatM[3]).xyz;
}
