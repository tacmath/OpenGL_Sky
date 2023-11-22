#include "shader.h"
#include "window.h"
#include "VAO.h"
#include "VBO.h"
#include "Stars.h"
#include "camera.h"

std::string glStringEnum(GLenum glenum)
{
    std::string enumString = "NONE";

    switch (glenum)
    {
    case GL_INVALID_ENUM:                   enumString = "INVALID_ENUM"; break;
    case GL_INVALID_VALUE:                  enumString = "INVALID_VALUE"; break;
    case GL_INVALID_OPERATION:              enumString = "INVALID_OPERATION"; break;
    case GL_OUT_OF_MEMORY:                  enumString = "OUT_OF_MEMORY"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:  enumString = "INVALID_FRAMEBUFFER_OPERATION"; break;
    
    case GL_DEBUG_SEVERITY_MEDIUM:          enumString = "GL_DEBUG_SEVERITY_MEDIUM"; break;
    case GL_DEBUG_SEVERITY_HIGH:            enumString = "GL_DEBUG_SEVERITY_HIGH"; break;
    case GL_DEBUG_SEVERITY_LOW:             enumString = "GL_DEBUG_SEVERITY_LOW"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:    enumString = "GL_DEBUG_SEVERITY_NOTIFICATION"; break;
    
    case GL_DEBUG_SOURCE_API:               enumString = "GL_DEBUG_SOURCE_API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     enumString = "GL_DEBUG_SOURCE_WINDOW_SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:   enumString = "GL_DEBUG_SOURCE_SHADER_COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:       enumString = "GL_DEBUG_SOURCE_THIRD_PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION:       enumString = "GL_DEBUG_SOURCE_APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER:             enumString = "GL_DEBUG_SOURCE_OTHER"; break;

    case GL_DEBUG_TYPE_ERROR:               enumString = "GL_DEBUG_TYPE_ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: enumString = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  enumString = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         enumString = "GL_DEBUG_TYPE_PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         enumString = "GL_DEBUG_TYPE_PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER:              enumString = "GL_DEBUG_TYPE_MARKER"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          enumString = "GL_DEBUG_TYPE_PUSH_GROUP"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           enumString = "GL_DEBUG_TYPE_POP_GROUP"; break;
    case GL_DEBUG_TYPE_OTHER:               enumString = "GL_DEBUG_TYPE_OTHER"; break;
    }
    return enumString;
}



static void debug(void) {
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSetErrorCallback([](int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    });
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        (void)id;
        (void)userParam;
        (void)length;
        if (type != GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR && (severity == GL_DEBUG_SEVERITY_MEDIUM ||  severity == GL_DEBUG_SEVERITY_HIGH || type == GL_DEBUG_TYPE_ERROR))
            std::cerr << "Opengl : " << " severity : " << glStringEnum(severity) << " type : " << glStringEnum(type) << " source : " << glStringEnum(source) << std::endl << "message : " << message << std::endl << std::endl;
        }, 0);
}

#include <functional>
struct EventData {
    Shader* shader;
    std::function<void(double mouseX, double mouseY)>					onMouseMouvement;
    std::function<void(int width, int height)>							onFramebufferSize;
};

void run(const Window& window) {
    float previousFrameTime = 0;
    float diff = 0;
    float time = 0;
    int status = 1;
    EventData eventData;
    Shader  shader;
    VBO     vbo;
    VAO     vao;
    float   vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    Camera camera;
    Stars stars;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    eventData.shader = &shader;
    eventData.onMouseMouvement = [&](double mouseX, double mouseY) {
        static double lastMouseX = DEFAULT_WINDOW_WIDTH / 2;
        static double lastMouseY = DEFAULT_WINDOW_HEIGHT / 2;
        
        camera.Rotate((float)(mouseX - lastMouseX) * 0.5f, (float)(mouseY - lastMouseY) * 0.5f);
        camera.Update();
        stars.SetVP(camera);
        glm::mat4 IVP = glm::inverse(camera.GetProjection() * glm::mat4(glm::mat3(camera.GetView())));
        shader.setMat4("IVP", IVP);
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    };

    eventData.onFramebufferSize = [&](int width, int height) {
        glViewport(0, 0, width, height);
        camera.ChangePerspective(80, (float)width, (float)height, 0.0001f, 100.0f);
        stars.SetVP(camera);
    };
    glfwSetWindowUserPointer(window.context, &eventData);
    glfwSetKeyCallback(window.context, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static Shader* shader = ((EventData*)glfwGetWindowUserPointer(window))->shader;

        if (key == GLFW_KEY_R && action == GLFW_PRESS) { // reload the shader when r is pressed
            Shader  newShader;

            newShader.Load("shaders/skyVS.glsl", "shaders/skyFS.glsl");
            if (newShader.isCompiled())
                *shader = std::move(newShader);
        }
    });
    glfwSetCursorPosCallback(window.context, [](GLFWwindow* window, double xpos, double ypos) {
        static EventData* callbacks = (EventData*)glfwGetWindowUserPointer(window);
        callbacks->onMouseMouvement(xpos, ypos);
    });

    glfwSetFramebufferSizeCallback(window.context, [](GLFWwindow* window, int width, int height) {
        static EventData* callbacks = (EventData*)glfwGetWindowUserPointer(window);
        callbacks->onFramebufferSize(width, height);
    });
    camera.Init((float)window.size.x, (float)window.size.y, glm::vec3(0));
    stars.SetVP(camera);
    shader.Load("shaders/skyVS.glsl", "shaders/skyFS.glsl");
    shader.Activate();
    glm::mat4 IVP = glm::inverse(camera.GetProjection() * glm::mat4(glm::mat3(camera.GetView())));
    shader.setMat4("IVP", IVP);
    vbo.Gen(vertices, sizeof(vertices));
    vao.Gen();
    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, sizeof(float), 0);
    vao.Bind();
    while (status) {
        time = (float)glfwGetTime();

        diff = time - previousFrameTime;
        if (diff >= 0.03f) {
            glfwPollEvents();
            if (glfwWindowShouldClose(window.context) == 1 || glfwGetKey(window.context, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                status = 0;

            shader.setFloat("time", time);
            shader.setVec3("sunPos", glm::vec3(0.0, sin(time * 0.2), cos(time * 0.2)));
            glClear(GL_COLOR_BUFFER_BIT);
            vao.Bind();
            shader.Activate();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            stars.Draw();
            glfwSwapBuffers(window.context);
            previousFrameTime = time/* - (diff - MIN_FRAME_DELAY)*/;
        }
    }
    vao.Delete();
    vbo.Delete();
    shader.Delete();
}

/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
*/
int main(void) {
    Window window;

    //    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    debug();
    run(window);

	return (0);
}

