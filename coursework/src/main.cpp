#include "Camera.h"
#include "Cube.h"
#include "Floor.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"
#include "ParentCamera.h"
#include "PersonCamera.h"
#include "Skybox.h"
#include "glm/detail/type_mat.hpp"
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
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

Camera freeCamera;
PersonCamera personCamera;
ParentCamera *camera = &personCamera;
int lastX = SCREEN_WIDTH / 2;
int lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// handled by the glfw callback function
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

    camera->ProcessMouseMovement(xoffset, yoffset);
}

// handled by the glfw callback function
void input_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->processInput(Camera::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->processInput(Camera::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->processInput(Camera::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->processInput(Camera::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera->processInput(Camera::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera->processInput(Camera::DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        camera = &freeCamera;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        camera = &personCamera;
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

int main()
{
    camera = &personCamera;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                   3); // Set the major version of the OpenGL context to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                   3); // Set the minor version of the OpenGL context to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE); // Set the OpenGL profile to core
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create windowed mode window and OpenGL context
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    // depth testing for 3D renders
    glEnable(GL_DEPTH_TEST);

    Skybox skybox;
    Floor floor;
    Cube cube;
    Sun sun = {{-0.6f, -0.6f, 0.4f},
               {0.5f, 0.5f, 0.5f},
               {0.8f, 0.8f, 0.8f},
               {0.6f, 0.6f, 0.6f}};
    /*
    Light light = {{50.0f, 50.0f, 50.f},
                   {0.0f, 0.0f, 0.0f},
                   {1.0f, 0.0f, 0.0f},
                   {1.0f, 0.0f, 0.0f},
                   1.0f,
                   0.0002f,
                   0.00016f};
    */

    Model mod("assets/wheel2/wheel2.obj");
    mod.translate(glm::vec3(0.0f, 12.7f, 0.0f));
    Model mod2("assets/Cart/1.obj");
    glm::vec3 pos = glm::vec3(0.0f, 2.7f, 0.0f);
    mod2.translate(pos);
    Shader shader("shaders/world.vs.glsl", "shaders/world.fs.glsl");

    glm::vec3 center(0.0f, 12.7f, 0.0f);
    glm::vec3 initialOffset(0.0f, 10.0f, 0.0f); // Radius = 10.0f

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        input_callback(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the color of the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(
            camera->fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
            10000.0f);
        glm::mat4 view = camera->GetViewMatrix();

        skybox.use(view, projection);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setVec3("sunDir", sun.direction);
        shader.setVec3("sunAmbient", sun.ambient);
        shader.setVec3("sunDiffuse", sun.diffuse);
        shader.setVec3("sunSpecular", sun.specular);
        shader.setVec3("cameraPos", camera->Position);

        floor.use(shader);
        mod.Draw(shader);
        shader.setMat4("model", model);
        mod2.Draw(shader);
        shader.setMat4("model", model);

        mod.rotate(0.1f, glm::vec3(1.0f, 0.0f, 0.0f));

        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(0.1f),
                                     glm::vec3(1.0f, 0.0f, 0.0f));

        glm::vec3 newPos =
            glm::vec3(rotationMatrix * glm::vec4(initialOffset, 1.0f)) + center;

        glm::vec3 tans = newPos - pos;
        pos = newPos;

        mod2.translate(tans);

        checkOpenGLError();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
