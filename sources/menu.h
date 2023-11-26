#pragma once

#include "shader.h"
#include "window.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

struct CustomUniform {
    char name[16];
    glm::vec3 value;

    CustomUniform(const char *name, glm::vec3 value) : value(value) {
        memcpy(this->name, name, 16);
        this->name[15] = 0;
    }
};

struct Uniforms {
    glm::vec3 skyColor;
    glm::vec3 voidColor;
    glm::vec3 nightColor;
    std::vector<CustomUniform> customUniforms;

    Uniforms() {
        skyColor = glm::vec3(0.047f, 0.622f, 0.8f);
        voidColor = glm::vec3(0.9f, 0.95f, 0.95f);
        nightColor = glm::vec3(0, 0, 0.12f);
    }
};

class Menu {
    const Window    *window;
    const Shader    *shader;
    Uniforms        uniforms;
    bool            menuIsOpen;

public:

    Menu();
    
    ~Menu();

    void Link(const Window* window, const Shader* shader);

    void Open();

    bool IsOpen();

    void Toogle();

    void Draw();

    void UpdateUniforms();

    void InstallCallbacks() { ImGui_ImplGlfw_InstallCallbacks(window->context); };

    void fpsTitle(float time, float latence);
    
private:
    void SetupImgui();

    void DrawMenu();

    void Close();

    void Delete();
};
