#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#undef main
int main(int argc, char** argv) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // 创建窗口和渲染器
    SDL_Window* window = SDL_CreateWindow("ImGui SDL2 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // 启用垂直同步

    // 初始化Dear ImGui
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);

    // 主循环
    bool running = true;
    while (running) {
        // 处理事件
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
        }

        // 开始新的ImGui帧
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // 在ImGui窗口中显示文本
        ImGui::Begin("Main Window");
        ImGui::Text("Hello, ImGui!");
        ImGui::End();

        // 渲染ImGui
        ImGui::Render();
        SDL_GL_MakeCurrent(window, gl_context);
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
