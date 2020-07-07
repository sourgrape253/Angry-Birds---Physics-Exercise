#version 430 core

in vec3 vert;
in vec2 vertTexCoord;

uniform mat4 model;

out vec2 fragTexCoord;

void main() 
{
    fragTexCoord = vec2(1.0 - vertTexCoord.x, 1.0 - vertTexCoord.y);
    
    gl_Position = model * vec4(vert, 1);
}