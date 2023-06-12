
#include "IncludeHeaders.h";
#include <GLFW/glfw3.h>


const unsigned WINDOW_WIDTH = 1280;
const unsigned WINDOW_HEIGHT = 720;
const char* WINDOW_NAME = "Symulacja wody OpenGL Arekadiusz Wieclaw 1ID23B";
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 500.0f;
const float CAM_FOV = 45.0f;
double deltaTime = 0.0;

GLFWwindow* window = nullptr;

bool rotateScene = false;
bool running = true;
glm::mat4 projection;

void initGL();
void resizeCallback(GLFWwindow* window, int width, int height);
void setWindowSize(int width, int height);
void processInput(GLFWwindow* window);
void updateScene();
void updateTerrain();
void drawScene();
void drawWindow();
void displayModel(Model model);
void setupCameraView(int w, int h);
void initShaderAndModels();
void displayWather();


//punkt umiejscowienia kamery
vec3 cameraPosition = vec3(10, 10, 15);
//miejsce, ktore "widzi" kamera
vec3 cameraView = vec3(0, 0, 0);
//zdefiniowanie gornego kierunku dla sceny (os y)
vec3 upDirektionY = vec3(0, 1, 0);

//obiekty dla shadera i modeli znajdujacych sie na scenie
Model shader;
Model terrain;







int main() {

    initGL();

    
    

    // Vertices coordinates
    /*
        GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
    };
    
    */

    GLfloat vertices[64000];
    GLint first_index[54];
    GLint length_i[54];


    for (int k = 0; k < 54; k++)
    {
        first_index[k] = (k * 192);
        length_i[k] = 192;
        for (int i = 0; i < 96; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                vertices[(j * 6) + (i * 12) + (k * 1152) + 0] = (GLfloat)(i * 0.2085f) - 0.998f;
                vertices[(j * 6) + (i * 12) + (k * 1152) + 1] = 0.998f - (GLfloat)(j * 0.037f) - (GLfloat)(k * 0.037f);
                vertices[(j * 6) + (i * 12) + (k * 1152) + 2] = 0.0f;
                vertices[(j * 6) + (i * 12) + (k * 1152) + 3] = (GLfloat)(rand() % 256) / 256.0f;
                vertices[(j * 6) + (i * 12) + (k * 1152) + 4] = (GLfloat)(rand() % 256) / 256.0f;
                vertices[(j * 6) + (i * 12) + (k * 1152) + 5] = (GLfloat)(rand() % 256) / 256.0f;
            }
        }
    }


    // Create reference containers for the Vartex Array Object and the Vertex Buffer Object
    GLuint VAO, VBO;

    // Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO);

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    //initShaderAndModels();
    while (running)
    {
        updateScene();
        processInput(window);
        //inicjalizacja shadera oraz modeli
        initShaderAndModels();

        drawScene();

        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        //glUseProgram(shaderProgram);
        // Bind the VAO so OpenGL knows to use it
        glBindVertexArray(VAO);
        // Draw the triangle using the GL_TRIANGLES primitive
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glMultiDrawArrays(GL_TRIANGLE_STRIP, (const GLint *) first_index, (const GLint*)length_i, 54);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}



void initGL()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (window == NULL)
        throw std::runtime_error("Can't create window.\n");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeCallback);


    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void setWindowSize(int width, int height)
{
    projection = glm::perspective(glm::radians(CAM_FOV), (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        running = false;
    glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotateScene = true;
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        rotateScene = false;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cameraView[2] -= 0.7;
        cameraPosition[2] -= 5.0;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cameraView[2] += 0.7;
        cameraPosition[2] += 5.0;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cameraView[0] -= 0.7;
        cameraPosition[0] -= 5.0;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cameraView[0] += 0.7;
        cameraPosition[0] += 5.0;
    }
    setupCameraView(WINDOW_WIDTH, WINDOW_HEIGHT);

}

void updateScene()
{
    //updateTerrain();
    //displayModel(terrain);
    void displayWather();
}

void drawScene()
{
    drawWindow();
}

void drawWindow()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void setupCameraView(int w, int h)
{
    //przygotowanie widoku z kamery
    glViewport(0, 0, w, h);
    shader.MatV = lookAt(cameraPosition, cameraView, upDirektionY);
    shader.MatP = perspective<float>(radians(60.0f), (float)w / (float)h, 0.1f, 1000.0f);
}

void displayModel(Model model)
{
    glBindTexture(GL_TEXTURE_2D, model.TEX);
    glBindVertexArray(model.VAO);
    glDrawElements(GL_TRIANGLES, model.model_indices.size(), GL_UNSIGNED_INT, model.model_indices.data());
}

void updateTerrain()
{
    terrain.MatM = mat4(1.0f);
    //ground.MatM = scale(ground.MatM, vec3(1.0f, 1.0f, 1.0f));
    terrain.MatM = translate(terrain.MatM, vec3(0.0f, 0.0f, 0.0f));
    //shader.update(terrain.MatM);
}

void initShaderAndModels()
{
    shader.InitShader("triangleShader.vert", "triangleShader.frag");
    //shader.shaderInit("perpixelTextureShader.vert", "perpixelTextureShader.frag");
    //terrain.initModel("Materials\ObjFiles\ground.obj", "Materials\Textures\ground.tga");
}

void displayWather()
{
    glColor3f(1.0, 1.0, 1.0);

    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT);

   
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}


