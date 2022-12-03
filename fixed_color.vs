#version 330 core

layout (location = 0) in vec3 Position;  // position 變量的屬性值為0

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
}