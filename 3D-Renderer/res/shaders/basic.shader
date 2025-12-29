#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
// layout(location = 1) in vec3 color;
layout(location = 1) in vec2 texCoord;

out vec3 u_Color;
out vec2 v_TexCoord;
        
void main()
{
    gl_Position = position;
//    u_Color = color;
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
        
// out vec4 color;
// in vec3 u_Color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
    
void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;

};