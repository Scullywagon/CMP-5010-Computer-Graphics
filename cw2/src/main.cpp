
#include "Constants.h"
#include "Entities/FerrisWheel.h"
#include "Renderer.h"
#include "Scene.h"
#include "VarName.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cmath>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace std;

namespace vars
{
bool enableRotation = false;
bool startingRotation = false;
bool betterVisuals = false;
bool showPopcorn = false;
} // namespace vars

int SCREEN_WIDTH = Constants::SCREEN_WIDTH;
int SCREEN_HEIGHT = Constants::SCREEN_HEIGHT;

int lastX = Constants::SCREEN_WIDTH / 2;
int lastY = Constants::SCREEN_HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;
float flightToggleTime = 0.0f;
float activateToggleTime = 0.0f;
float rotationToggleTime = 0.0f;
float visualsToggleTime = 0.0f;
float cameraToggleTime = 0.0f;
float popcornToggleTime = 0.0f;
bool isRotate = false;

Scene *scene;
Renderer *renderer;

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // automaticaly takes the x and y positions of the mouse
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // ensures the first mouse movement is not a jump starts in starting
    // position
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX; // change since last frame
    float yoffset = lastY - ypos; // change since last frame
    lastX = xpos; // updates the last x position for the next frame
    lastY = ypos; // updates the last y position for the next frame

    scene->cam->ProcessMouseMovement(xoffset, yoffset);
}

void input_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        scene->cam->processInput(Camera::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        scene->cam->processInput(Camera::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        scene->cam->processInput(Camera::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        scene->cam->processInput(Camera::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        scene->cam->processInput(Camera::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        scene->cam->processInput(Camera::DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS &&
        currentFrame - flightToggleTime > 0.5)
    {
        scene->Player.flight = !scene->Player.flight;
        flightToggleTime = currentFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS &&
        currentFrame - rotationToggleTime > 0.5)
    {
        cout << "Rotation: " << vars::enableRotation << endl;
        vars::enableRotation = !vars::enableRotation;
        rotationToggleTime = currentFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cout << "position: " << scene->cam->Position.x << " "
             << scene->cam->Position.y << " " << scene->cam->Position.z << endl;
        if (scene->cam == &scene->overview)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - scene->cam->Position );
            scene->worldLight.direction = dir;
            renderer->regenLightView();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS &&
        currentFrame - activateToggleTime > 0.5)
    {
        scene->playerActivate();
        activateToggleTime = currentFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS &&
        currentFrame - cameraToggleTime > 0.5)
    {
        if (scene->cam != &scene->Player)
        {
            scene->cam = &scene->Player;
        }
        else
        {
            scene->cam = &scene->overview;
        }
        cameraToggleTime = currentFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS &&
        currentFrame - visualsToggleTime > 0.5)
    {
        vars::betterVisuals = !vars::betterVisuals;
        visualsToggleTime = currentFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&
        currentFrame - popcornToggleTime > 0.5)
    {
        vars::showPopcorn = !vars::showPopcorn;
        popcornToggleTime = currentFrame;
    }
}

void checkOpenGLError()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }
}

int initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                   3); // Set the major version of the OpenGL context to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                   3); // Set the minor version of the OpenGL context to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE); // Set the OpenGL profile to core
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    // Create windowed mode window and OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL,
                              NULL);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == NULL)
    {
        checkOpenGLError();
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return 1;
    }
    return 0;
}

int main()
{
    cout << "Starting GLFW" << endl;

    if (initGLFW() != 0)
    {
        cout << "Failed to initialize GLFW" << endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    scene = new Scene();
    renderer = new Renderer(scene);

    scene->init();
    renderer->init();

    while (!glfwWindowShouldClose(window))
    {
        checkOpenGLError();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        input_callback(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color of the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->update(deltaTime);
        renderer->update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cout << "GLFW initialized" << endl;
    return 0;
}
