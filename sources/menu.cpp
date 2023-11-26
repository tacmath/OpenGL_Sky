#include "menu.h"

Menu::Menu() {
    menuIsOpen = false;
}

Menu::~Menu() {
    Delete();
}


void Menu::SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6;
    style.SeparatorTextAlign.x = 0.5f;
    style.WindowTitleAlign.x = 0.5f;
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    
    ImGui_ImplGlfw_InitForOpenGL(window->context, false);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Menu::Delete() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Menu::Link(const Window *window, const Shader* shader) {
    this->window = window;
    this->shader = shader;
    SetupImgui();
}

bool Menu::IsOpen() {
    return menuIsOpen;
}

void Menu::Open() {
    menuIsOpen = true;
    glfwSetInputMode(window->context, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Menu::Close() {
    menuIsOpen = false;
    glfwSetInputMode(window->context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Menu::Toogle() {
    if (menuIsOpen)
        Close();
    else
        Open();
}

void Menu::fpsTitle(float time, float latence) {
 /*   static float previousUpdateTime_title = 0;
    static float diff = 0;
    static int frame_title = 0;
    char title[100];

    frame_title += 1;
    diff = time - previousUpdateTime_title;
    if (diff >= 0.2f) {
        sprintf(title, "Minecraft :  FPS: %d (%.3f ms)", frame_title * 5, latence * 1000);
        glfwSetWindowTitle(window->context, title);
        previousUpdateTime_title = time - (diff - 0.2f);
        frame_title = 0;
    }*/
}

void Menu::UpdateUniforms() {
    shader->setVec3("SkyColor", uniforms.skyColor);
    shader->setVec3("VoidColor", uniforms.voidColor);
    shader->setVec3("NightColor", uniforms.nightColor);
    for (const CustomUniform& uniform : uniforms.customUniforms) {
        shader->setVec3(uniform.name, uniform.value);
    }
}

void Menu::DrawMenu() {
    ImVec2 buttonSize;

    ImGui::Begin("Menu", 0, ImGuiWindowFlags_NoScrollbar);
    buttonSize.x = ImGui::GetWindowWidth() - 2 * ImGui::GetStyle().WindowPadding.x;
    buttonSize.y = 0;

  //  ImGui::SeparatorText("Options");
   // if (ImGui::CollapsingHeader("Options")) {
        
    if (ImGui::ColorEdit3("SkyColor", &uniforms.skyColor[0]))
        shader->setVec3("SkyColor", uniforms.skyColor);
    if (ImGui::ColorEdit3("VoidColor", &uniforms.voidColor[0]))
        shader->setVec3("VoidColor", uniforms.voidColor);
    if (ImGui::ColorEdit3("NightColor", &uniforms.nightColor[0]))
        shader->setVec3("NightColor", uniforms.nightColor);

    std::string label("##uniform");
    for (CustomUniform& uniform : uniforms.customUniforms) {
        label += " ";

        if (ImGui::ColorEdit3(label.c_str(), &uniform.value[0]))
            shader->setVec3(uniform.name, uniform.value);
        ImGui::SameLine();
        if (ImGui::InputText(label.c_str(), uniform.name, 15))
            shader->setVec3(uniform.name, uniform.value);
    }
    ImGui::PushItemWidth(buttonSize.x);
    ImGui::PopItemWidth();

    if (ImGui::Button("Add New Uniform", buttonSize)) {
        uniforms.customUniforms.push_back(CustomUniform((std::string("Uniform") + std::to_string(uniforms.customUniforms.size() + 1)).c_str(), glm::vec3(0)));
    }

    if (ImGui::Button("Resume", buttonSize))
        Close();

    if (ImGui::Button("Exit", buttonSize))
        exit(0);

    ImGui::End();
}


void Menu::Draw() {
    if (!menuIsOpen) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DrawMenu();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}