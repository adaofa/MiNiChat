// #version 330 core

// in vec2 fragCoord;
// out vec4 FragColor;

// uniform float iTime;       // 时间变量，用于控制流光速度
// uniform vec2 iResolution;  // 屏幕尺寸

// void main()
// {
//     vec2 uv = gl_FragCoord.xy / iResolution.xy;
//         float wave = sin(5.0 * (uv.x + iTime * 0.25)) * 0.5 + 0.5;
//         vec3 cyan = vec3(0.0, 1.0, 1.0);
//         vec3 white = vec3(1.0);
//         vec3 color = mix(white, cyan, wave);
//         FragColor = vec4(color, 1.0);
// }

#version 330 core

out vec4 FragColor;

uniform float iTime;
uniform vec2 iResolution;

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float wave = sin(2.5 * (uv.x + uv.y + iTime * 0.25)) * 0.5 + 0.5;
    vec3 color = mix(vec3(1.0), vec3(0.0, 1.0, 1.0), wave);
    FragColor = vec4(color, 1.0);
}
