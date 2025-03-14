#version 330 core

in vec2 fraguv;

out vec4 col;

const mat2x3 colmat = mat2x3(1, 0.5, 0,
                             0, 0.5, 1);

void main()
{
    col = vec4((colmat*fraguv),1);
}