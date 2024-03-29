#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Renderer.h"
#include "SpheresGenerator.h"

#include <cstdio>
#include <vector>
#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static std::vector<float> generateTextureData(glm::uvec2 size)
{
    std::vector<float> data;

    for (int i = 0; i != size.y; ++i)
    {
        for (int j = 0; j != size.x; ++j)
        {
            data.push_back(0.0);
            data.push_back((float)j / (size.x - 1));
            data.push_back((float)i / (size.y - 1));
        }
    }
    return data;
}

static GLuint generateTexture(glm::uvec2 size, const float* data)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_FLOAT, data);
   
    return texture;
}

//static GLuint generateTexture(glm::uvec2 size, const std::vector<glm::vec3>& pixels)
//{
//    return generateTexture(size, (float*)pixels.data());
//}
//
//static GLuint generateTexture(glm::uvec2 size, const std::vector<float>& data)
//{
//    return generateTexture(size, data.data());
//}

static GLuint compileVertexShader()
{
    std::ifstream vertexShaderFile("vertex.shader");

    std::string vertexShaderSource((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());

    const char* ptr = vertexShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &ptr, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512]{};
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        exit(1);
    }

    return vertexShader;
}

static GLuint compileFragmentShader()
{
    std::ifstream fragmentShaderFile("fragment.shader");

    std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* ptr = fragmentShaderSource.c_str();

    glShaderSource(fragmentShader, 1, &ptr, NULL);
    glCompileShader(fragmentShader);

    int success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512]{};
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        exit(1);
    }

    return fragmentShader;
}

static GLuint compileShaderProgram()
{
    GLuint vertexShader = compileVertexShader();
    GLuint fragmentShader = compileFragmentShader();

    unsigned int shaderProgram = glCreateProgram();
   
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    int success;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[512]{};
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        exit(1);
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

static GLuint createVBO()
{
    float vertices[]
    {
        -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return VBO;
}

static GLuint createEBO()
{
    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return EBO;
}

static GLuint createVAO()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    createVBO();
    createEBO();
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

static void updateCamera(GLFWwindow* window, Camera& camera, glm::uvec2 size, double deltaTime, bool& needsRerender)
{
    float offset = (float)deltaTime * 1.0f;

    if (ImGui::IsKeyDown(ImGuiKey_W))
    {
        camera.Move(glm::vec3(0, 0, -offset));
        needsRerender = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_S))
    {
        camera.Move(glm::vec3(0, 0, offset));
        needsRerender = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_A))
    {
        camera.Move(glm::vec3(-offset, 0, 0));
        needsRerender = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_D))
    {
        camera.Move(glm::vec3(offset, 0, 0));
        needsRerender = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_Space))
    {
        camera.Move(glm::vec3(0, offset, 0));
        needsRerender = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_C))
    {
        camera.Move(glm::vec3(0, -offset, 0));
        needsRerender = true;
    }

    static ImVec2 lastMousePos(-1, -1);

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        auto mousePos = ImGui::GetMousePos();

        if (lastMousePos.x != -1 && lastMousePos.y != -1)
        {
            float speed = 3000.0f;
            glm::vec3 rotation(float(mousePos.x - lastMousePos.x) / size.x, float(mousePos.y - lastMousePos.y) / size.y, 0);
            camera.Rotate(rotation * speed * (float)deltaTime);
            needsRerender = true;
        }

        lastMousePos = mousePos;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        lastMousePos = ImVec2(-1, -1);
    }
}

int main()
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        return 1;
    }

    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glm::uvec2 size(1280, 720);

    GLFWwindow* window = glfwCreateWindow(size.x, size.y, "Ray Tracing", nullptr, nullptr);

    if (window == nullptr)
    {
        return 1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    Camera camera(90, (float)size.x / size.y, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

    /*LambertianMaterial material1(glm::vec3(1.0, 0.0, 0.0), 0.5);
    LambertianMaterial material2(Random::Instance.Color(), 0.5);
    MetalMaterial material3(Random::Instance.Color(), Random::Instance.Float(std::uniform_real_distribution<float>(0, 1)));
    DielectricMaterial material4(1.5);

    Sphere sphere1(glm::vec3(0, 0, -2), 0.5, material1);
    Sphere sphere2(glm::vec3(0, -1000.5, 0), 1000, material2);
    Sphere sphere3(glm::vec3(-1, 0, -2), 0.5, material3);
    Sphere sphere4(glm::vec3(1, 0, -2), 0.5, material4);

    SphereCollection scene(
        {
            sphere1,
            sphere2,
            sphere3,
            sphere4
        });*/

    SpheresGenerator spheresGenerator;

    SpheresGenerator::Parameters parameters;
    parameters.maxCount = 10;
    parameters.radius = 2;

    SphereCollection scene = spheresGenerator.Generate(parameters);
    
    /*SphereCollection scene;

    int d = 1;

    for (int m = -d; m <= d; ++m)
    {
        for (int n = -d; n <= d; ++n)
        {
            for (int o = -d; o <= d; ++o)
            {
                if (m == 0 && n == 0 && o == 0)
                {
                    continue;
                }

                Sphere sphere;
                sphere.center = glm::vec3(m, n, o);
                sphere.material = MetalMaterial(Random::Instance.Color(), 0.01f);
                sphere.radius = 0.5;

                scene.Add(std::move(sphere));
            }
            
        }
    }

    LambertianMaterial ground(Random::Instance.Color(), 1.0);

    Sphere sphere;
    sphere.center = glm::vec3(0, -1001.5, 0);
    sphere.radius = 1000;
    sphere.material = ground;

    scene.Add(std::move(sphere));*/

    size_t currentSamples = 0;

    Image image(size);

    Renderer renderer(16, 8, std::chrono::duration<long long>::max());
    auto renderFunction = renderer.GetRenderFunction(size, camera, scene);

    bool needsRerender = true;

    double lastTime{};

    compileShaderProgram();
    createVAO();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        ImGui::Begin("Settings");

        ImGui::Text("dt: %.2f ms", io.DeltaTime * 1000);
        ImGui::Text("FPS: %.2f", io.Framerate);
        ImGui::Text("Samples: %d/%d", currentSamples, renderer.samplesPerPixel);

        constexpr size_t minSamples = 1;
        constexpr size_t maxSamples = 1024;

        if (ImGui::DragScalar("Samples", ImGuiDataType_U64, &renderer.samplesPerPixel, 0.5f, &minSamples, &maxSamples))
        {
            needsRerender = true;
        }

        constexpr size_t minBounces = 1;
        constexpr size_t maxBounces = 64;

        if (ImGui::DragScalar("Bounces", ImGuiDataType_U64, &renderer.bounceLimit, 0.5f, &minBounces, &maxBounces))
        {
            needsRerender = true;
        }

        ImGui::End();

        ImGui::Render();

        updateCamera(window, camera, size, deltaTime, needsRerender);

        int display_w, display_h;

        glfwGetFramebufferSize(window, &display_w, &display_h);

        if (display_w != size.x || display_h != size.y)
        {
            size = glm::uvec2(display_w, display_h);
            image = Image(size);
            currentSamples = 0;
            camera.aspectRatio = (float)size.x / size.y;
            renderFunction = renderer.GetRenderFunction(size, camera, scene);
        }

        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (needsRerender)
        {
            currentSamples = 0;
            image.SetColor(glm::vec3(0, 0, 0));
            needsRerender = false;
        }

        if (currentSamples < renderer.samplesPerPixel)
        {
            renderFunction(image);
            ++currentSamples;

            float inv = 1.0f / currentSamples;
            std::vector<glm::vec3> colors(image.GetWidth() * image.GetHeight());
            std::transform(std::execution::par_unseq, image.GetPixels().cbegin(), image.GetPixels().cend(), colors.begin(), [inv](const glm::vec3& color)
                {
                    //return color * inv;
                    return sqrt(color * inv);
                });

            //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, GL_RGB, GL_FLOAT, colors.data());
            generateTexture(size, (float*)colors.data());
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
