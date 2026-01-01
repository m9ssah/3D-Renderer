#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 u_Color;
out vec2 v_TexCoord;

uniform mat4 u_MVP;
        
void main()
{
    gl_Position = u_MVP * position;
    u_Color = color;
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core
        
out vec4 color;

in vec3 u_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;    
void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
}