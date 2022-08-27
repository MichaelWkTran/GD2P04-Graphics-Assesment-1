#include "Main.h"
#include "Classes/GameManager.h"
#include "Classes/Camera.h"
#include <iostream> 

#pragma region Window

unsigned int e_uViewPortW = 800, e_uViewPortH = 800;
GLFWwindow* e_pMainWindow = nullptr;

#pragma endregion

#pragma region Time

float e_fPreviousTimestep = 0.0f;
float e_fDeltatime = 0.0f;
float e_fMaxDeltatime = 1.0f/60.0f;

#pragma endregion

#pragma region Input

std::set<void(*)(GLFWwindow*, int, int, int, int)> e_setKeyCallbackFunctions;
char e_charCodePoint = 0;
bool e_bCodePointFound = false;

std::set<void(*)(GLFWwindow*, int, int, int)> e_setMouseCallbackFunctions;
glm::vec2 e_v2MousePosition;
glm::vec2 e_v2MouseNDCPosition;
glm::vec3 e_v3MouseRayDirection;

void UpdateMousePosition()
{
    double XPos, YPos;
    glfwGetCursorPos(e_pMainWindow, &XPos, &YPos);

    e_v2MousePosition.x = (float)XPos;
    e_v2MousePosition.y = (float)YPos;

    //Update Mouse NDC Position
    e_v2MouseNDCPosition = glm::vec2
    (
        (2.0f * e_v2MousePosition.x) / ((float)e_uViewPortW - 1.0f),
        (1.0f - (2.0f * e_v2MousePosition.y)) / (float)e_uViewPortH
    );
    
    glm::vec4 v4ClipCoord = glm::vec4(e_v2MouseNDCPosition.x, e_v2MouseNDCPosition.y, -1.0f, 1.0f);

    //Homogeneous Clip Space to Eye space
    glm::mat4 mat4InvProj = glm::inverse(GetMainCamera().GetProjectionMatrix());
    glm::vec4 v4EyeCoords = mat4InvProj * v4ClipCoord;

    // Manually set z and w to mean forward direction and a vector and not a point.
    v4EyeCoords = glm::vec4(v4EyeCoords.x, v4EyeCoords.y, -1.0f, 0.0f);

    //Eyespace to World space
    glm::mat4 mat4InvView = glm::inverse(GetMainCamera().GetViewMatrix());
    glm::vec4 v4RayWorld = mat4InvView * v4EyeCoords;
    e_v3MouseRayDirection = glm::normalize(glm::vec3(v4RayWorld));
}

void UpdateInputPressed()
{
    e_bCodePointFound = false;
}

void TextInput(GLFWwindow* _pWindow, unsigned int _iCodePoint)
{
    e_bCodePointFound = true;
    e_charCodePoint = _iCodePoint;
}

#pragma endregion

#include "Classes/AssesmentGameManager.h"

int main()
{
    #pragma region Setup GLFW, GLEW, and Viewport

    //Initialize and Configure GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Set up Window
    e_pMainWindow = glfwCreateWindow(e_uViewPortW, e_uViewPortH, "LearnOpenGL", NULL, NULL);
    if (e_pMainWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(e_pMainWindow);

    //Initializing GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Setup Window Viewport
    glViewport(0, 0, e_uViewPortW, e_uViewPortH);
    glfwSetFramebufferSizeCallback(e_pMainWindow, [](GLFWwindow* _e_pMainWindow, int _iWidth, int _iHeight) {glViewport(0, 0, _iWidth, _iHeight); });

    //Set up Culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //Set up Anti-Aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    #pragma endregion

    //Set up Game Manager
    new CAssesmentGameManager;

    //-------------------------------------------------------------------------------------
    glfwSetKeyCallback
    (
        e_pMainWindow,
        [](GLFWwindow* _pWindow, int _iKey, int _iScanCode, int _iAction, int _iMods)
        {
            for (auto& i : e_setKeyCallbackFunctions) i(_pWindow, _iKey, _iScanCode, _iAction, _iMods);
        }
    );

    glfwSetMouseButtonCallback
    (
        e_pMainWindow,
        [](GLFWwindow* _pWindow, int _iButton, int _iAction, int _iMods)
        {
            for (auto& i : e_setMouseCallbackFunctions) i(_pWindow, _iButton, _iAction, _iMods);
        }
    );

    //Game Loop
    while (!glfwWindowShouldClose(e_pMainWindow))
    {
        //Update Deltatime
        float fCurrentTimestep = (float)glfwGetTime();
        e_fDeltatime = fCurrentTimestep - e_fPreviousTimestep;
        if (e_fDeltatime > e_fMaxDeltatime) e_fDeltatime = e_fMaxDeltatime;
        e_fPreviousTimestep = fCurrentTimestep;

        //Update Window Size
        {
            int x, y;
            glfwGetWindowSize(e_pMainWindow, &x, &y);
            e_uViewPortW = x;
            e_uViewPortH = y;
        }

        //Inputs
        UpdateMousePosition();

        //Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        //Update GameManager
        GetGameManager().Update();

        //Check and call events and swap the buffers
        glfwSwapBuffers(e_pMainWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(e_pMainWindow);
    glfwTerminate();
    return 0;
}