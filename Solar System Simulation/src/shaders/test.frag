#version 330 core

in vec3 fragPos;
in vec3 n;
in vec2 tc;

uniform sampler2D sampler;
uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform bool isLightSource;

float specularStrength = 0.4;
float ambient = 0.05;

out vec4 color;

void main() {
	
	vec4 d = texture(sampler, tc);
	color = d;

	vec3 lightVector = normalize(lightPosition - fragPos);
	vec3 normal = normalize(n);
	float diff = max(dot(lightVector, normal), 0.0);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightVector, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  

	if(!isLightSource)
		color = (ambient + diff + spec) * d;

	if(d.a < 0.01)
        discard; // If the texture is transparent, don't draw the fragment
	
} 
