#include "window.hpp"
#include "menuvars.hpp"


void Window::Init()
{
    if (!this->m_wClass.m_Window) {
        std::cerr << "Window faield Exiting...\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->m_wClass.m_Window);
    glfwSwapInterval(0); // Disable vsync

    /* Define all the public window variables */
    this->m_Hwnd = glfwGetWin32Window(this->m_wClass.m_Window);
    glfwGetWindowSize(this->m_wClass.m_Window, &this->m_wClass.m_Size.x, &this->m_wClass.m_Size.y);
    glfwGetWindowPos(this->m_wClass.m_Window, &this->m_wClass.m_Pos.x, &this->m_wClass.m_Pos.y);


    InitImGui();
}

void Window::InitImGui() noexcept
{
    ImGui::CreateContext();
    
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->m_wClass.m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    /* Define all the public Imgui variables */
    this->m_IO = &ImGui::GetIO();
}

/* Hide the taskbar icon */
void Window::HideTaskbar() noexcept
{
    SetWindowLong(this->GetHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}

/* Streamproof */
void Window::ToggleCapture() noexcept
{
    DWORD old;
    GetWindowDisplayAffinity(this->GetHwnd(), &old);
    SetWindowDisplayAffinity(this->GetHwnd(), old ^= WDA_EXCLUDEFROMCAPTURE);
}


void Window::ShutdownImGui() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiIO* Window::GetIo() const noexcept
{
    return this->m_IO;
}

void Window::FrameStart() noexcept
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}


void Window::FrameEnd() noexcept
{
	ImGui::Render();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(this->m_wClass.m_Window);
}

void Window::ImGuiMenu() noexcept
{
    if (ImGui::Begin("Test"))
    {
        ImGui::Text("How are you today");
        ImGui::Checkbox("Fps", &values::fps);

        if (ImGui::Checkbox("Streamproof", &values::streamproof))
            this->ToggleCapture();

        ImGui::NewLine();
    }
    ImGui::End();
}

/* Menu state updator */
void Window::FrameShow() noexcept
{
	if (!glfwWindowShouldClose(this->m_wClass.m_Window))
	{
		//this->FrameStart();

		if (GetAsyncKeyState(VK_INSERT) & 1) {
            this->ToggleMenu();
		}

		if (this->m_MenuState) {
			this->ImGuiMenu();
		}

        //this->FrameEnd();
	}
}



int Window::GetAttribute(const int attribute) const noexcept
{
    return glfwGetWindowAttrib(this->m_wClass.m_Window, attribute);
}


void Window::SetAttribute(const int attribute, const int value) noexcept
{
    glfwSetWindowAttrib(this->m_wClass.m_Window, attribute, value);
}

void Window::Destruct() noexcept
{
    this->ShutdownImGui();
    glfwDestroyWindow(this->m_wClass.m_Window);
    glfwTerminate();
}

void Window::ToggleMenu() noexcept {
    glfwSetWindowAttrib(this->m_wClass.m_Window, GLFW_MOUSE_PASSTHROUGH, this->m_MenuState);
    this->m_MenuState = !this->m_MenuState; 
}

void Window::SetWindowTitle(std::string_view newTitle) noexcept {
    this->m_wClass.m_WindowName = newTitle;
    glfwSetWindowTitle(this->m_wClass.m_Window, newTitle.data());
}

std::string_view Window::GetWindowTitle() const noexcept
{
    return this->m_wClass.m_WindowName;
}


void Window::SetWindowSize(WVec2 newSize) noexcept
{
    this->m_wClass.m_Size = newSize;
    glfwSetWindowSize(this->m_wClass.m_Window, newSize.x, newSize.y);
}


WVec2 Window::GetWindowSize() const noexcept
{
    return this->m_wClass.m_Size;
}

void Window::SetWindowPos(WVec2 newPos) noexcept
{
    this->m_wClass.m_Pos = newPos;
    glfwSetWindowPos(this->m_wClass.m_Window, newPos.x, newPos.y);
}

WVec2 Window::GetWindowPos() const noexcept
{
    return this->m_wClass.m_Pos;
}


bool Window::GetMenuState() const noexcept {
    return this->m_MenuState; 
};

HWND Window::GetHwnd() const noexcept {
    return this->m_Hwnd; 
}
