#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 u_Color;
        
void main()
{
    gl_Position = position;
    u_Color = color;
};

#shader fragment
#version 330 core
        
out vec4 color;
in vec3 u_Color;
    
void main()
{
    color = vec4(u_Color, 1.0);

};