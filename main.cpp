#include <fstream>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <sstream>

// Globals.
int gScreenHeight = 480;
int gScreenWidth = 640;
bool gIsRunning = false;
SDL_Window* gWindow = nullptr;
SDL_GLContext gContext = nullptr;
SDL_DisplayMode gDisplayMode;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint gPipelineProgram = 0;

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitializeProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL2 could not initialize video subsystem." << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION , 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION , 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER , 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE , 24);

    SDL_GetCurrentDisplayMode(0, &gDisplayMode);

    const int halfWidth = gDisplayMode.w / 2;
    const int halfHeight = gDisplayMode.h / 2;

    gWindow = SDL_CreateWindow(
        "Main window",
        halfWidth - gScreenWidth / 2,
        halfHeight - gScreenHeight / 2,
        gScreenWidth,
        gScreenHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (gWindow == nullptr) {
        std::cerr << "Could not create window." << std::endl;
        exit(1);
    }

    gContext = SDL_GL_CreateContext(gWindow);

    if (gContext == nullptr) {
        std::cerr << "Could not create OpenGL context." << std::endl;
        exit(1);
    }

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "GLAD could not be initialized." << std::endl;
    }

    GetOpenGLVersionInfo();
    gIsRunning = true;
}

void VertexSpecification() {
    const std::vector<GLfloat> vertexPositions {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };

    // Create VAO.
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    // Generate VBO.
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GLfloat), vertexPositions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

    // Cleanup.
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

GLuint CompileShader(const GLuint type, const std::string& source) {
    GLuint gShaderObject;

    switch (type) {
        case GL_VERTEX_SHADER:
        case GL_FRAGMENT_SHADER:
            gShaderObject = glCreateShader(type);
            break;
        default:
            std::cerr << "Invalid shader type: " << type << std::endl;
            exit(1);
    }

    const char* cSource = source.c_str();
    glShaderSource(gShaderObject, 1, &cSource, nullptr);
    glCompileShader(gShaderObject);

    return gShaderObject;
}

std::string ReadShaderSource(const std::string &path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint CreateShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    GLuint gProgram = glCreateProgram();
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, ReadShaderSource(vertexShaderPath));
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, ReadShaderSource(fragmentShaderPath));

    glAttachShader(gProgram, vertexShader);
    glAttachShader(gProgram, fragmentShader);
    glLinkProgram(gProgram);
    glValidateProgram(gProgram);

    return gProgram;
}

void CreateGraphicsPipeline() {
    gPipelineProgram = CreateShaderProgram("shaders/shader.vert", "shaders/shader.frag");
}

void Input() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            std::cout << "Quitting..." << std::endl;
            gIsRunning = false;
        }
    }
}

void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.0f, 0.8f, 0.5f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(gPipelineProgram);
}

void Draw() {
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainLoop() {
    while (gIsRunning) {
        Input();
        PreDraw();
        Draw();
        SDL_GL_SwapWindow(gWindow);
    }
}

void CleanUp() {
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

int main() {
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    CleanUp();
    return 0;
}
