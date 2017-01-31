#version 330 core

out vec4 color;

void main()
{
    float depth = gl_FragCoord.z;
    depth = (depth - 0.6f) * 4.0;

    color = vec4(1.0f, 1.0f - (depth * 0.9), depth * 0.8, 1.0f);
} 
