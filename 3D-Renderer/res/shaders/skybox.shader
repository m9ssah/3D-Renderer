#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	TexCoord = aPos;
	vec4 pos = u_Projection * u_View * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
};

#shader fragment
#version 330 core

out vec4 color;

in vec3 TexCoord;

uniform samplerCube u_Skybox;

void main()
{
	color = texture(u_Skybox, TexCoord);
}