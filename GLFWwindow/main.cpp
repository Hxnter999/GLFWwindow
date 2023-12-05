#include "window.hpp"
#include "menuvars.hpp"

void randomExampleFunc(Window* window)
{
    ImVec2 size = window->GetWindowSize().ToImVec2();
    ImGui::GetBackgroundDrawList()->AddRect({ 0,0 }, size, ImColor{ 255,255,0 }, 0, 0, 20);
    
    {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.4,0.4,0.4,1.0 });
        ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4{ 0.9,0.9,0.9,1.0 });
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.1,0.1,0.1,1.0 });
       
        ImGui::SetNextWindowSize(ImGui::CalcTextSize("Fps: 9999 | Frametime: 9.999999"));
        
        ImGui::Begin("Fps", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Text("Fps: %.0f | Frametime: %.4f", window->GetIo()->Framerate, 1000.0f / window->GetIo()->Framerate);
        ImGui::End();

        ImGui::PopStyleColor(3);
    }
}


int main() {
    /* new/delete is optional ofc 
    * Heres a list of window hints
    * https://www.glfw.org/docs/3.3/window_guide.html */
    Window* window = new Window(0, 0, "Window Testing", { // 1920 1080 will turn the screen black, 1 pixel bigger/smaller should be fine
        { GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR   },
        { GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_REVISION},
        //{ GLFW_MAXIMIZED, GLFW_TRUE                        }, // makes it smaller lmao
        { GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE          },
        { GLFW_FLOATING, GLFW_TRUE                         },
        { GLFW_RESIZABLE, GLFW_FALSE                       },
        { GLFW_DECORATED, GLFW_FALSE                       },
        { GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE                },
        { GLFW_FOCUSED, GLFW_TRUE                          },
        { GLFW_REFRESH_RATE, INT_MAX                       },
        { GLFW_SCALE_TO_MONITOR, GLFW_FALSE                } 
        });

    //window->HideTaskbar();
    window->ToggleMenu();
    bool isRunning = true;
    while (isRunning)
    {
        
        window->FrameStart();

        if (values::printfps)
            randomExampleFunc(window);
        
        ImGui::GetBackgroundDrawList()->AddRect({ 500, 600 }, { 900 ,1000 }, ImColor{ 30, 200, 255 }, 50, 0, 25);
        
        window->FrameShow();
        //window->ImGuiMenu();
        window->FrameEnd();
        if (GetAsyncKeyState(VK_END)) isRunning = false;
        //if (GetAsyncKeyState(VK_INSERT) & 1) window->ToggleMenu(); can manually do this instead of FrameShow()
       
    }

    delete window;
    return 0;
}
