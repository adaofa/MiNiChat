// #version 330 core

// layout(location = 0) in vec2 position;
// out vec2 fragCoord;

// void main()
// {
//     fragCoord = position * 0.5 + 0.5; // 从 [-1,1] 转为 [0,1]
//     gl_Position = vec4(position, 0.0, 1.0);
// }

#version 330 core

layout(location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}

