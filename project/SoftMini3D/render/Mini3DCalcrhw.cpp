#include "Mini3DRender.h"
#include "Mini3DDevice.h"
#include "Mini3DDraw.h"
#include <stdio.h>
#include <math.h>

void calcrhw(device_t *device, trapezoid_t traps[2]) {

	float rhw1 = 1.f/traps[0].left.v1.pos.w;
	// float rhw1 = traps[0].left.v1.rhw;
	float rhw2 = 1.f/traps[0].left.v2.pos.w;
	float h = traps[0].bottom-traps[0].top;
	int y;
	// pr_debug("---\n");
	float pvalue[2] = {0,1};
	float x_realmax = -100;
	for(y=traps[0].bottom-0.5;y>traps[0].top;y--) {
		vertex_t v1 = traps[0].left.v1;
		vertex_t v2 = traps[0].left.v2;
		float p = (y-v2.pos.y+0.5f)/(v1.pos.y-v2.pos.y);
		if(p>pvalue[0])
			pvalue[0] = p;
		if(p<pvalue[1])
			pvalue[1] = p;


		float u = v2.tc.u + p*(v1.tc.u-v2.tc.u);
		float rhw = v2.rhw + p*(v1.rhw-v2.rhw);
		float u0 = u/rhw;



		// {---------------------------------------------------------
			point_t sposv1 = traps[0].left.v1.spos;
			point_t sposv2 = traps[0].left.v2.spos;
			struct Vertexm {
				float x_w, y_w, z_w;  // 实际三维坐标
				float x_s, y_s;       // 屏幕坐标
				float rhw;            // 透视齐次坐标
			};

			// 已知顶点//(10,10)(-10,-10)
			Vertexm v1m = {sposv1.x, sposv1.y, sposv1.z, traps[0].left.v1.pos.x, traps[0].left.v1.pos.y, traps[0].left.v1.rhw};  // (x_w, y_w, z_w, x_s, y_s, rhw)
			Vertexm v2m = {sposv2.x, sposv2.y, sposv2.z, traps[0].left.v2.pos.x, traps[0].left.v2.pos.y, traps[0].left.v2.rhw}; // (x_w, y_w, z_w, x_s, y_s, rhw)

			// float pp = 0.3f;
			// 目标屏幕坐标
			float x_target = traps[0].left.v2.pos.x + p *(traps[0].left.v1.pos.x-traps[0].left.v2.pos.x) ;
			float y_target = traps[0].left.v2.pos.y + p *(traps[0].left.v1.pos.y-traps[0].left.v2.pos.y) ;

			// 计算插值参数
			float p_x = (x_target - v1m.x_s) / (v2m.x_s - v1m.x_s);
			float p_y = (y_target - v1m.y_s) / (v2m.y_s - v1m.y_s);

			// 线性插值透视齐次坐标
			float rhw_target = v2m.rhw + p_x * (v1m.rhw - v2m.rhw);

			// 线性插值实际三维坐标
			float x_target_w = v1m.x_w*v1m.rhw + p_x * (v2m.x_w*v2m.rhw - v1m.x_w*v1m.rhw);
			float y_target_w = v1m.y_w + p_y * (v2m.y_w - v1m.y_w);
			float z_target_w = v1m.z_w*v1m.rhw + p_y * (v2m.z_w*v2m.rhw - v1m.z_w*v1m.rhw);

			// 应用透视校正
			float x_real = x_target_w / rhw_target;
			float y_real = y_target_w / rhw_target;
			float z_real = z_target_w / rhw_target;

			if(x_real > x_realmax)
				x_realmax = x_real;
			// pr_debug("%3.2f/%3.2f/%3.2f\n", x_real, y_real, z_real);
			if(z_real < 5.f){
				// float x0 = traps[0].left.v1.pos.x + p*(traps[0].left.v2.pos.x-traps[0].left.v1.pos.x);
				// float y0 = traps[0].left.v1.pos.y + p*(traps[0].left.v2.pos.y-traps[0].left.v1.pos.y);
				// pr_debug("%3.2f/%3.2f\n", x0, y0);

				// device_point(device, x_target, y_target, 0xff0000);
				// pr_debug("%3.2f\n", z_real);
				// break;
				// device_point(device, x0, y0, 0xff00);
				// ImGui::SliderFloat("p", (float *)&p,0,1);
				break;
			}
				// pr_debug("x %3.2f p %3.2f\n", x_real, p);

		// }----------------------------------------------------
		// {----------------------------------------------------
			// vertex_t v1 = traps[0].left.v1;
			// vertex_t v2 = traps[0].left.v2;

			// 计算插值参数 p
			// float p = (y - v2.pos.y + 0.5f) / (v1.pos.y - v2.pos.y);
			

			// 计算在 y 位置上的纹理坐标 u 和反向齐次坐标 rhw
			float I1 = v1.tc.u;
			float I2 = v2.tc.u;
			float Z1 = v1.rhw;
			float Z2 = v2.rhw;
			//-----------------------------------------------------------

			// 计算透视校正的属性值
			float I1_div_Z1 = I1;
			float I2_div_Z2 = I2;
			// It/Zt = (I1/Z1 + S(I2/Z2-I1/Z1))
			float It_div_Zt = I2_div_Z2 + p * (I1_div_Z1 - I2_div_Z2);

			// 计算目标位置 Zt
			float Zt = v2.rhw + p * (v1.rhw - v2.rhw);

			// 计算透视校正后的纹理坐标 u0
			float u00 = It_div_Zt / Zt;
			//-----------------------------------------------------------
			float u2 = I2 + p * (I1 - I2);
			float rhw2 = Z2 + p * (Z1 - Z2);
			float u01= u2/rhw2;
			//-----------------------------------------------------------
			// pr_debug("%3.2f\n", u01-u0);

		// }----------------------------------------------------

		float x00 = traps[0].left.v2.spos.x + u00*(traps[0].left.v1.spos.x-traps[0].left.v2.spos.x);
		float y00 = traps[0].left.v2.spos.z + u00*(traps[0].left.v1.spos.z-traps[0].left.v2.spos.z);
		float r = sqrt(pow(x00, 2.f)+pow(y00, 2.f));
		// pr_debug("%3.2f/%3.2f/%3.2f\n", x00, y00, r);
		if(r>8.f){
			float x0 = traps[0].left.v2.pos.x + p*(traps[0].left.v1.pos.x-traps[0].left.v2.pos.x);
			float y0 = traps[0].left.v2.pos.y + p*(traps[0].left.v1.pos.y-traps[0].left.v2.pos.y);
			device_point(device, x0, y0, 0xff00);
			break;
		}


		// if(u0 > 0.875f){
		// 	float x0 = v2.pos.x + p*(v1.pos.x-v2.pos.x);
		// 	float y0 = v2.pos.y + p*(v1.pos.y-v2.pos.y);
		// 	device_point(device, x0, y0, 0xff00);
		// 	ImGui::SliderFloat("p", (float *)&p,0,1);
		// 	break;
		// }
	}
	// pr_debug("%3.2f/%3.2f\n",pmin, pmax);




}