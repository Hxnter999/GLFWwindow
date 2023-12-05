#pragma once
#include <string_view>
#include <vector>
#include <iostream>
#include <thread>
#include <Windows.h>
#define GLEW_STATIC
#define GLFW_EXPOSE_NATIVE_WIN32
/* Include GLEW before GLFW */
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>



struct Flag {
    constexpr Flag(const int hint, const int value) noexcept : hint(hint), value(value) {}
    int hint, value;
};


struct WVec2 {

    constexpr WVec2(const int x = 0, const int y = 0) noexcept : x(x), y(y) {}

    constexpr ImVec2 ToImVec2() const noexcept {
        return ImVec2(static_cast<float>(x), static_cast<float>(y));
    }
    constexpr WVec2 operator/(const float divisor) const noexcept {
        return WVec2(x / divisor, y / divisor);
    }
    int x, y;
};



/* Kinda bloated, easily portable tho */
class Window
{
    class WindowClass
    {
    public:
        WVec2 m_Size{};
        WVec2 m_Pos{};
        std::string m_WindowName;
        GLFWwindow* m_Window{};
        GLFWmonitor* m_Monitor{};
        const GLFWvidmode* m_Vidmode{};
        WindowClass() = default;
        explicit WindowClass(int width, int height, std::string window_name)
            : m_Size(width, height), m_WindowName(window_name)
        {
            m_Window = glfwCreateWindow(m_Size.x, m_Size.y, m_WindowName.c_str(), nullptr, nullptr);
        }
    };

    WindowClass m_wClass;
    HWND m_Hwnd{};
    std::vector<Flag> m_Flags{};
    ImGuiIO* m_IO{};
    bool m_MenuState{};

    void Init();
    void InitImGui() noexcept;
    void ShutdownImGui() noexcept;
    void Destruct() noexcept;
    
public:
    HWND GetHwnd() const noexcept;
    bool GetMenuState() const noexcept;
    void ToggleMenu() noexcept;

    void SetWindowTitle(std::string_view newTitle) noexcept;
    std::string_view GetWindowTitle() const noexcept;

    void SetWindowSize(WVec2 newSize) noexcept;
    WVec2 GetWindowSize() const noexcept;

    void SetWindowPos(WVec2 newPos) noexcept;
    WVec2 GetWindowPos() const noexcept;

    void SetAttribute(int attribute, int value) noexcept;
    int GetAttribute(int attribute) const noexcept;
    

    void HideTaskbar() noexcept;  // Hides the taskbar icon
    void ToggleCapture() noexcept; // Hides/Shows the overlay for monitor captures, basically "Streamproof"

    ImGuiIO* GetIo() const noexcept; // Not required really, ImGui::GetIO()

    void FrameStart() noexcept;
    void FrameEnd() noexcept;
    void FrameShow() noexcept;
    void ImGuiMenu() noexcept;
        
    Window() = default;
    explicit Window(int width, int height, std::string_view windowName, const std::vector<Flag>& flags)
    {
        if (!glfwInit()) {
            std::cerr << "Init() failed Exiting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::exit(EXIT_FAILURE);
        }

        /* All the window flags */
        /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_REFRESH_RATE, INT_MAX);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);*/
        
        for (const auto& x : flags)
            glfwWindowHint(x.hint, x.value), this->m_Flags.push_back(x);

        
        this->m_wClass.m_Monitor = glfwGetPrimaryMonitor();
        this->m_wClass.m_Vidmode = glfwGetVideoMode(this->m_wClass.m_Monitor);
        if (!width || !height)
            width = this->m_wClass.m_Vidmode->width, height = this->m_wClass.m_Vidmode->height + 1; // perfect fullscreen wont render properly so +1
        this->m_wClass.m_Window = glfwCreateWindow(width, height, windowName.data(), nullptr, nullptr);


        Init();
    }

    ~Window()
    {
        Destruct();
    }
};
