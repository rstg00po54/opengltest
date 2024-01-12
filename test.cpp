#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef unsigned int uint;
class vec2{
public:
    vec2(float x, float y){};
    float x;
    float y;
};



class vec3{
public:
    vec3(float x, float y, float z){};
    float x;
    float y;
    float z;
    vec3 operator*(float){

    }
    vec3 operator/(float){

    }
    vec3 operator+(vec3){

    }
};

class vec4{
public:
    vec4(float x, float y, float z, float w){};
    vec4(vec3 v, float w){};
    float x;
    float y;
    float z;
    float w;
    vec3 operator*(float){

    }
    vec3 operator/(float){

    }
    vec3 operator+(vec3){

    }
};

int gl_VertexIndex;
vec3 v;
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

// layout (location = 0) out vec3 pos;

// self.map = np.array(range(6*9), dtype = np.uint32)
// self.dirs = np.zeros(6*9, dtype = np.uint32)
uint32_t dirs[6*9];

void main() 
{   

    // glm.lookAt();
    uint id_vert = gl_VertexIndex % 6; 
    uint id_sq_out = gl_VertexIndex / 6;    
    uint id_in_face_out = id_sq_out % 9;
    uint id_face_out = id_sq_out / 9;    
    uint id_x_out = id_in_face_out % 3;
    uint id_y_out = id_in_face_out / 3;

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

    uint id_sq_in = get_value(map, id_sq_out);    
    uint id_in_face_in = id_sq_in % 9;
    uint id_face_in = id_sq_in / 9;    
    uint id_x_in = id_in_face_in % 3;
    uint id_y_in = id_in_face_in / 3;      

    uint dir_in = get_value(dirs, id_sq_out);

    vec3 o_in = positions[id_face_in*3];
    vec3 ox_in = positions[id_face_in*3+1] - o_in;
    vec3 oy_in = positions[id_face_in*3+2] - o_in;

    vec2 idv_in = indices[6*dir_in + id_vert];
    vec3 pos_in = o_in + ox_in *(float(id_x_in)+idv_in.x)/3.0 + oy_in *(float(id_y_in)+idv_in.y)/3.0; 

    pos = pos_in;
}
