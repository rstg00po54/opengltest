#version 330 core
const vec3 positions[18] = { 
    vec3(3.0, 3.0, 3.0), vec3(3.0, 3.0, -3.0), vec3(3.0, -3.0, 3.0),
    vec3(-3.0, 3.0, -3.0), vec3(-3.0, 3.0, 3.0), vec3(-3.0, -3.0, -3.0),
    vec3(-3.0, 3.0, -3.0), vec3(3.0, 3.0, -3.0), vec3(-3.0, 3.0, 3.0),
    vec3(-3.0, -3.0, 3.0), vec3(3.0, -3.0, 3.0), vec3(-3.0, -3.0, -3.0),
    vec3(-3.0, 3.0, 3.0), vec3(3.0, 3.0, 3.0), vec3(-3.0, -3.0, 3.0),
    vec3(3.0, 3.0, -3.0), vec3(-3.0, 3.0, -3.0), vec3(3.0, -3.0, -3.0) };

const vec2 indices[24] = { 
    vec2(0.0,0.0), vec2(0.0,1.0), vec2(1.0,1.0), vec2(1.0,1.0), vec2(1.0,0.0), vec2(0.0,0.0),
    vec2(1.0,0.0), vec2(0.0,0.0), vec2(0.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0), vec2(1.0,0.0), 
    vec2(1.0,1.0), vec2(1.0,0.0), vec2(0.0,0.0), vec2(0.0,0.0), vec2(0.0,1.0), vec2(1.0,1.0),
    vec2(0.0,1.0), vec2(1.0,1.0), vec2(1.0,0.0), vec2(1.0,0.0), vec2(0.0,0.0), vec2(0.0,1.0) };

layout (location = 0) out vec3 pos;
uniform mat4 matrix;
void main() 
{
    highp int  id_vert = gl_VertexID % 6; 
    highp int id_sq_out = gl_VertexID / 6;    
    highp int id_in_face_out = id_sq_out % 9;
    highp int id_face_out = id_sq_out / 9;    
    highp int id_x_out = id_in_face_out % 3;
    highp int id_y_out = id_in_face_out / 3;

    vec3 o_out = positions[id_face_out*3];
    vec3 ox_out = positions[id_face_out*3+1] - o_out;
    vec3 oy_out = positions[id_face_out*3+2] - o_out;

    vec2 idv_out = indices[id_vert];
    vec3 pos_out = o_out + ox_out *(float(id_x_out)+idv_out.x)/3.0 + oy_out *(float(id_y_out)+idv_out.y)/3.0;    

    vec4 wpos = vec4(pos_out, 1.0);
    vec4 ppos = matrix * wpos;    
    ppos.y = -ppos.y;
    ppos.z = (ppos.z + ppos.w) / 2.0;
    gl_Position = ppos;

    uint id_sq_in = id_sq_out;// get_value(map, id_sq_out);    
    uint id_in_face_in = id_sq_in % 9;
    uint id_face_in = id_sq_in / 9;    
    uint id_x_in = id_in_face_in % 3;
    uint id_y_in = id_in_face_in / 3;

    uint dir_in = 0;//get_value(dirs, id_sq_out);

    vec3 o_in = positions[id_face_in*3];
    vec3 ox_in = positions[id_face_in*3+1] - o_in;
    vec3 oy_in = positions[id_face_in*3+2] - o_in;

    vec2 idv_in = indices[6*dir_in + id_vert];
    vec3 pos_in = o_in + ox_in *(float(id_x_in)+idv_in.x)/3.0 + oy_in *(float(id_y_in)+idv_in.y)/3.0; 

    pos = pos_in;
}