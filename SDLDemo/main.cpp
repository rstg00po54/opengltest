#include <SDL2/SDL.h>
#include "glad.h"
#include <GL/gl.h>
#undef main
int main() {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // 处理初始化失败的情况
        return -1;
    }

    // 创建窗口和上下文
    SDL_Window* window = SDL_CreateWindow("SDL with Partial OpenGL",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_OPENGL);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // 初始化 OpenGL
    if (gladLoadGL() == 0) {
        // 处理 OpenGL 初始化失败的情况
        return -1;
    }

    // 设置 OpenGL 视口（部分窗口）
    glViewport(0, 0, 400, 300);  // 设置为窗口左上角的一部分

    // 主循环
    SDL_Event event;
    while (true) {
        // 处理事件循环
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // 处理窗口关闭事件
                goto exit0;
            }

            // 处理其他事件...
        }

        // 渲染 OpenGL 图形
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 在这里进行其他 OpenGL 渲染操作...

        // 交换缓冲区
        SDL_GL_SwapWindow(window);
    }
exit0:
    // 清理资源
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    // 清理 SDL
    SDL_Quit();

    return 0;
}
