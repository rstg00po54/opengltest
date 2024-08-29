#include <CL/cl.h>
#include <iostream>
#include <vector>

int getinfo() {
    cl_uint platformCount;
    clGetPlatformIDs(0, nullptr, &platformCount);
    
    std::vector<cl_platform_id> platforms(platformCount);
    clGetPlatformIDs(platformCount, platforms.data(), nullptr);
    
    std::cout << "Number of OpenCL platforms: " << platformCount << std::endl;

    for (cl_uint i = 0; i < platformCount; ++i) {
        char platformName[128];
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 128, platformName, nullptr);
        std::cout << "Platform " << i + 1 << ": " << platformName << std::endl;

        cl_uint deviceCount;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);

        std::vector<cl_device_id> devices(deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices.data(), nullptr);

        std::cout << "  Number of devices: " << deviceCount << std::endl;

        for (cl_uint j = 0; j < deviceCount; ++j) {
            char deviceName[128];
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 128, deviceName, nullptr);
            std::cout << "  Device " << j + 1 << ": " << deviceName << std::endl;

            cl_uint computeUnits;
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(computeUnits), &computeUnits, nullptr);
            std::cout << "    Number of compute units: " << computeUnits << std::endl;
        }
    }

    return 0;
}

#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// 读取文件内容为字符串
std::string loadKernelSource(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

int main() {
    const int elements = 1024;
    std::vector<int> A(elements), B(elements), C(elements);

    // 初始化输入数据
    for(int i = 0; i < elements; ++i) {
        A[i] = i;
        B[i] = i * 2;
    }
    getinfo();
    // 获取平台和设备信息
    cl_platform_id platform;
    cl_device_id device;
    cl_uint numPlatforms;
    clGetPlatformIDs(1, &platform, &numPlatforms);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);

    // 创建上下文
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, nullptr);

    // 创建命令队列
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, nullptr, nullptr);

    // 创建内存缓冲区
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                    sizeof(int) * elements, A.data(), nullptr);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                    sizeof(int) * elements, B.data(), nullptr);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
                                    sizeof(int) * elements, nullptr, nullptr);

    // 读取并编译内核文件
    std::string kernelSource = loadKernelSource("vector_add_kernel.cl");
    const char* source = kernelSource.c_str();
    cl_program program = clCreateProgramWithSource(context, 1, &source, nullptr, nullptr);
    clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    // 创建内核对象
    cl_kernel kernel = clCreateKernel(program, "vecAdd", nullptr);

    // 设置内核参数
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

    // 定义全局工作组大小
    size_t globalSize = elements;

    // 执行内核
    clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &globalSize, nullptr, 0, nullptr, nullptr);

    // 等待内核执行完成
    clFinish(queue);

    // 读取结果
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, sizeof(int) * elements, C.data(), 0, nullptr, nullptr);

    // 显示前10个结果
    std::cout << "Result of vector addition:" << std::endl;
    for(int i = 0; i < 10; ++i) {
        std::cout << A[i] << " + " << B[i] << " = " << C[i] << std::endl;
    }

    // 释放资源
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}



