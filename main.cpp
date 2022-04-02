#include "player.h"
#include "shader.h"
#include "block.h"
#include "chunk.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "texture.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

Player player;

Chunk chunk;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    player.processMovement(window, deltaTime, chunk);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    player.camera.ProcessMouseMovement(xoffset, yoffset);
}

int sgn(float a)
{
    if (a > 0)
        return 1;
    else if (a < 0)
        return -1;
    else return 0;
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "minecum", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    unsigned int VBOs[2], VAOs[2];
    
    Shader containerShader("vertexContainer.glsl", "fragmentContainer.glsl");
    Block container(VAOs[0], VBOs[0]);
    
    Shader playerShader("playerVertex.glsl", "playerFragment.glsl");
    player = Player(glm::vec3(0.0f, -0.2f, 0.0f), VAOs[1], VBOs[1]);

    Texture playerTexture = Texture("flAOQJ7reKc.jpg");
    playerShader.use();
    playerShader.setInt("playerTexture", 0);

    Texture containerTexture = Texture("container.jpg");
    containerShader.use();
    containerShader.setInt("texture1", 1);

    while (!glfwWindowShouldClose(window))
    {   
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, playerTexture.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerTexture.ID);
        
        {
            containerShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            containerShader.setMat4("projection", projection);
            glm::mat4 view = player.camera.GetViewMatrix();
            containerShader.setMat4("view", view);
            glBindVertexArray(VAOs[0]);
            for (int i = 0; i < chunk.coordinate.size(); i++) 
            {
                glm::vec3 coord = chunk.coordinate[i];
                
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, coord);
                containerShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

            }
        }

        //debug
        std::cout << player.position.x << " " << player.position.y << " " << player.position.z << std::endl;
        //std::cout << player.camera.Position.x << " " << player.camera.Position.y << " " << player.camera.Position.z << std::endl;
        
        //player draw
        /*{
            playerShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            playerShader.setMat4("projection", projection);
            glm::mat4 view = player.camera.GetViewMatrix();
            playerShader.setMat4("view", view);
            glBindVertexArray(VAOs[1]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, player.entity.position);
            playerShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/
 
        processInput(window);
        player.camera.update(glm::vec3(player.position.x, player.position.y + HEIGHT_Y / 2, player.position.z));
        
        std::cout << "-------------------------------" << std::endl;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glfwTerminate();

	return 0;
}