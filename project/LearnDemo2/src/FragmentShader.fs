layout (location = 0) in vec3 pos;
layout (location = 0) out vec4 outColor;

void main() 
{
    vec3 dir = normalize(pos);
    outColor = texture(arr_cubemap[0], dir);
}