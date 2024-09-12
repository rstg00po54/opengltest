
#include "Mini3DCalcLines.h"
#include "Mini3DMath.h"
#include <math.h>
#include <stdio.h>
#include <Mini3DDevice.h>


static void inter(vector_t *pin1, const vector_t *pin0, float t){

	pin1->x = pin1->x+t*(pin0->x-pin1->x);
	pin1->y = pin1->y+t*(pin0->y-pin1->y);
	pin1->z = pin1->z+t*(pin0->z-pin1->z);
	pin1->w = pin1->w+t*(pin0->w-pin1->w);
}

// -w < x < w，-w < y < w，-w < z < w
point_t transform_home0(const device_t *device, const vector_t *from, const vector_t *to) {
	vector_t *pin0, *pin1;
	float hw;
	vector_t tn = {0,0,0,0};
	// ImGui::Text("-------");
	// ImGui::SliderFloat4("输入0", (float *)from, -50.0f, 50.0f);
	// ImGui::SliderFloat4("输入1", (float *)to, -50.0f, 50.0f);
	// if(abs(from->w) < 0.0001f){
	// 	ImGui::Text("---from----");
	// }
	// if(abs(to->w) < 0.0001f){
	// 	ImGui::Text("---to----");
	// }
	pin0 = (vector_t *)malloc(sizeof(vector_t));
	pin1 = (vector_t *)malloc(sizeof(vector_t));
	*pin0 = *from;
	*pin1 = *to;
	static bool v = true;
	// ImGui::Checkbox("clip", &v);
	// if(abs(pin1->x)>abs(pin1->w)){
	// 	ImGui::Text("need clip x");
	// }
	// if(abs(pin1->y)>abs(pin1->w)){
	// 	ImGui::Text("need clip y");
	// }
	// if(abs(pin1->z)>abs(pin1->w)){
	// 	ImGui::Text("need clip z");
	// }


	bool clipZMins = pin1->z < -std::abs(pin1->w);
	bool clipZPlus = pin1->z >  std::abs(pin1->w);

	bool clipXPlus = pin1->x >  std::abs(pin1->w);
	bool clipXMins = pin1->x < -std::abs(pin1->w);

	bool clipYPlus = pin1->y >  std::abs(pin1->w);
	bool clipYMins = pin1->y < -std::abs(pin1->w);
	// ImGui::Checkbox("clipZMins", &clipZMins);

	// if(v){
		if(pin1->z < -abs(pin1->w)){
			// ImGui::Text("clip z -");
			//t = (w1-z1)/((w1-z1)-(w2-z2))
			//I = Q1+t*(Q2-Q1)		
		}
		if(pin1->z > abs(pin1->w)){
			// ImGui::Text("clip z +, %f", pin1->z - abs(pin1->w));
			//t = (w1-z1)/((w1-z1)-(w2-z2))
			//I = Q1+t*(Q2-Q1)	
			// vector_t ret;
			float t = (pin1->w - pin1->z)/((pin1->w-pin1->z)-(pin0->w-pin0->z));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);

		}
		if(pin1->x > abs(pin1->w)){
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w - pin1->x)/((pin1->w-pin1->x)-(pin0->w-pin0->x));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::Text("clip x + t = %f", t);
			// ImGui::SliderFloat4("裁剪x+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->x < -abs(pin1->w)){
			// ImGui::Text("clip x -");
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w + pin1->x)/((pin1->w+pin1->x)-(pin0->w+pin0->x));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::SliderFloat4("裁剪x-", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->y > abs(pin1->w)){
			// ImGui::Text("clip y +, pin1->y %f", pin1->y);
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w - pin1->y)/((pin1->w-pin1->y)-(pin0->w-pin0->y));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::SliderFloat4("裁剪y+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->y < -abs(pin1->w)){
			// ImGui::Text("clip y -, pin1->y %f", pin1->y);
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w + pin1->y)/((pin1->w+pin1->y)-(pin0->w+pin0->y));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::SliderFloat4("裁剪y-", (float *)pin1, 0.0f, 50.0f);
		}
	// }
	// hw = 1.f/pin1->w;
	// pin1->x = hw*pin1->x;
	// pin1->y = hw*pin1->y;
	// pin1->z = hw*pin1->z;
	// pin1->w = hw*pin1->w;
	
	// ImGui::SliderFloat4("pin1归一化", (float *)pin1, 0.0f, 50.0f);
	// ImGui::SliderFloat4("o", (float *)&o, 0.0f, 50.0f);

	// pout0->x = (pin0->x+1)*ts->w*0.5f;//400
	// pout0->y = (pin0->y+1)*ts->h*0.5f;//300
	// pout0->z = pin0->z;
	// pout1->x = (pin1->x+1)*ts->w*0.5f;
	// pout1->y = (pin1->y+1)*ts->h*0.5f;

	// pout0->y = 600 - pout0->y;
	// pout1->y = 600 - pout1->y;
	
	// ImGui::SliderFloat4("pout0", (float *)pout0, 0.0f, 50.0f);
	// ImGui::SliderFloat4("pout1", (float *)pin1, 0.0f, 50.0f);
	// float t = (pin1->x-pin0->x)/(src.x-pin0->x);
	// pr_debug("t = %3.2f\n", t);
	// ImGui::SliderFloat("tt", &t, 0, 1);
// ImGui::Text("ok");
	return *pin1;

}

int  transform_Normalization(device_t *device, vector_t *po1, vector_t *po0, vector_t *pin1, vector_t*pin0, int c = 0) {
	point_t po00,po11;
	float rhw1, rhw2;
	po00 = transform_home0(device, pin0, pin1);
	po11 = transform_home0(device, pin1, pin0);
	if (po00.w == 0){
		ImGui::Text("po00 is null");
		// pr_debug("po00 is null\n");
		return -1;
	}
	if (po11.w == 0){
		ImGui::Text("po00 is null");
		// pr_debug("po11 is null\n");
		return -1;
	}
	float t1, t2;
	t1 = (po00.x-pin0->x)/(pin1->x-pin0->x);
	t2 = (po11.x-pin0->x)/(pin0->x-pin1->x);
	if (c == 1) {
		ImGui::SliderFloat("t1", (float *)&t1, 0.0f, 50.0f);
		ImGui::SliderFloat4("po00", (float *)&po00, 0.0f, 50.0f);
		ImGui::SliderFloat("t2", (float *)&t2, 0.0f, 50.0f);
		ImGui::SliderFloat4("po11", (float *)&po11, 0.0f, 50.0f);
		ImGui::Text("size %d, %d", sizeof(po00), sizeof(po11));
	}

	rhw1 = 1.f/po00.w;
	po0->x = (po00.x * rhw1 + 1.0f) * device->transform.w * 0.5f;
	po0->y = (1.0f - po00.y * rhw1) * device->transform.h * 0.5f;
	rhw2 = 1.f/po11.w;
	po1->x = (po11.x * rhw2 + 1.0f) * device->transform.w * 0.5f;
	po1->y = (1.0f - po11.y * rhw2) * device->transform.h * 0.5f;
	if(po0->x <  -0.1  || po0->y <  -0.1  || po0->x > device->width+0.1 || po0->y > device->height+0.1){
		ImGui::Text("error4!! t %f", t1);
		ImGui::SliderFloat4("po00", (float *)&po00, 0.0f, 50.0f);
		ImGui::SliderFloat4("po0", (float *)po0, 0.0f, 50.0f);
		return -1;
	}
	if(po1->x < -0.1 || po1->y <  -0.1  || po1->x > device->width+0.1 || po1->y > device->height+0.1){
		ImGui::Text("error5!! t %f", t2);
		 ImGui::SliderFloat4("po11", (float *)&po11, 0.0f, 50.0f);
		 ImGui::SliderFloat4("po1", (float *)po1, 0.0f, 50.0f);
		return -1;
	}
	return 0;
}

void drawLine(device_t *device, point_t p0, point_t p1, int c = 0) {
	point_t v0,v1;
	point_t vs0,vs1;
	vs0 = {0,0,0,1};
	vs1 = {0,0,0,1};
	matrix_apply( &v0, &p0, &device->transform.mvp);
	matrix_apply( &v1, &p1, &device->transform.mvp);

	if ( v0.w < 0 && v1.w < 0){
		ImGui::Text("do not");
		return;
	}
	int ret = transform_Normalization(device, &vs0, &vs1, &v0, &v1, c);
	if (ret < 0){
		return;
	}
	// float rhw;
	// rhw = 1.0f / v0.w;
	// v0.x = v0.x*rhw;
	// v0.y = v0.y*rhw;
	// v0.z = v0.z*rhw;
	// v0.w = v0.w*rhw;
	// rhw = 1.0f / v1.w;
	// v1.x = v1.x*rhw;
	// v1.y = v1.y*rhw;
	// v1.z = v1.z*rhw;
	// v1.w = v1.w*rhw;
	// ImGui::SliderFloat4("v0", (float *)&v0, 0, 1);
	// ImGui::SliderFloat4("v1", (float *)&v1, 0, 1);
	// int c[] = {0xff,0xff00,0x0000,0,0};
	// if(vs0.x < 0 || vs0.y < 0 || vs0.x > device->width || vs0.y > device->height){
	// 	ImGui::Text("error1!!");
	// 	return;
	// }
	// if(vs1.x < 0 || vs1.y < 0 || vs1.x > device->width || vs1.y > device->height){
	// 	ImGui::Text("error0!!");
	// 	return;
	// }
	device_draw_line(device, vs0, vs1, 0);
}
