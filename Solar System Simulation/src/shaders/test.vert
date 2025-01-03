#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 transformationMatrix;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;
out vec2 tc;
out vec3 n;

void main() {
	tc = texCoord;

	fragPos = vec3(transformationMatrix * vec4(pos, 1.0));
	n = vec3(transpose(inverse(transformationMatrix)) * vec4(normal, 0.0));

	gl_Position = P * V * transformationMatrix * vec4(pos, 1.0);
}
