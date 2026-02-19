#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_MVP * vec4(aPos, 1.0);
	FragPos = vec3(u_Model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform vec3 u_LightColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * u_LightColor;

	vec3 texColor = texture(u_Texture, TexCoord).rgb;
	vec3 result = (ambient + diffuse + specular) * texColor;
	FragColor = vec4(result, 1.0);
}