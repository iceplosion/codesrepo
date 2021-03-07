#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.h"
#include <filesystem>
#include <stdio.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

Camera camera = Camera();

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void glfw_mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
    static double x, y;
    static bool lbutton_down, rbutton_down;
    auto lbutton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    auto rbutton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (!rbutton_down && !lbutton_down && lbutton == GLFW_PRESS) {
        glfwGetCursorPos(window, &x, &y);
        lbutton_down = true;
    }
    else if (lbutton_down && GLFW_RELEASE == lbutton) {
        lbutton_down = false;
        camera.update();
    }
    else if (!lbutton_down && !rbutton_down && rbutton == GLFW_PRESS) {
        glfwGetCursorPos(window, &x, &y);
        rbutton_down = true;
    }
    else if (rbutton_down && GLFW_RELEASE == rbutton) {
        rbutton_down = false;
        camera.update();
    }
    if (lbutton_down) {
        double u, v;
        glfwGetCursorPos(window, &u, &v);
        double dx = u - x, dy = v - y;
        camera.rotate(-dx, -dy);
    }
    else if (rbutton_down) {
        double u, v;
        glfwGetCursorPos(window, &u, &v);
        double dx = u - x, dy = v - y;
        camera.push(dy);
    }
}

void shader_compile_error_check(int shader) {
    static int succ;
    const static int buf_sz = 1 << 10;
    static char info[buf_sz];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(shader, buf_sz, NULL, info);
        std::printf("ERROR: shader compilation failed\n%s\n", info);
        exit(1);
    }
}
void shader_link_error_check(int program) {
    static int succ;
    const static int buf_sz = 1 << 10;
    static char info[buf_sz];
    glGetProgramiv(program, GL_LINK_STATUS, &succ);
    if (!succ) {
        glGetProgramInfoLog(program, buf_sz, NULL, info);
        printf("ERROR: shader link failed\n%s\n", info);
        exit(1);
    }
}

void setMat4(const unsigned int id, const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

struct ModelFiles {
    std::string filename;
    std::string filepath;
    ModelFiles(std::string name, std::string path) : filename(name), filepath(path) {}
};
std::vector<ModelFiles> filenames;

const ImVec4 BG_COLOR = ImVec4(0.204f, 0.20f, 0.498f, 1.00f);
const int WINDOW_W = 1280;
const int WINDOW_H = 720;

const char* pointCloudVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* pointCloudFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.f, 1.f, 1.f, 1.f);
}
)";

int main(int, char**)
{
    static int opened_file = -1;
    static FILE* opened_file_ptr = nullptr;
    static GLfloat* point_cloud_vertices = nullptr;
    static size_t point_cloud_vertices_num = (size_t)0;
    // load the path of all assets
    for (const auto& ite : std::filesystem::recursive_directory_iterator(
        std::filesystem::current_path().string() + "/../assets")) {
        if (ite.path().extension() == ".obj") {
            filenames.push_back(ModelFiles(ite.path().stem().string(), ite.path().string()));
        }    
    }

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "HW10", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gladLoadGL() == 0;

    // Setup mouse button callback
    glfwSetCursorPosCallback(window, glfw_mouse_position_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // build, compile and link shader for point cloud
    int pointCloudVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(pointCloudVertexShader, 1, &pointCloudVertexShaderSource, NULL);
    glCompileShader(pointCloudVertexShader);
    shader_compile_error_check(pointCloudVertexShader);
    int pointCloudFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(pointCloudFragmentShader, 1, &pointCloudFragmentShaderSource, NULL);
    glCompileShader(pointCloudFragmentShader);
    shader_compile_error_check(pointCloudFragmentShader);
    int pointCloudShaderProgram = glCreateProgram();
    glAttachShader(pointCloudShaderProgram, pointCloudVertexShader);
    glAttachShader(pointCloudShaderProgram, pointCloudFragmentShader);
    glLinkProgram(pointCloudShaderProgram);
    shader_link_error_check(pointCloudShaderProgram);
    glDeleteShader(pointCloudVertexShader);
    glDeleteShader(pointCloudFragmentShader);

    unsigned int pointCloudVBO, pointCloudVAO;

    ImVec4 clear_color = BG_COLOR;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show toolkit window
        {
            ImGui::Begin("Toolkit");
            static int selected = -1;
            if (ImGui::Button("Open selected file")) {
                if (opened_file != -1) {
                    glDeleteVertexArrays(1, &pointCloudVAO);
                    glDeleteBuffers(1, &pointCloudVBO);
                }
                opened_file = selected;
                // load point cloud data from file
                opened_file_ptr = fopen(filenames[opened_file].filepath.c_str(), "r");
                std::vector<float> points;
                float xp, yp, zp;
                while (~fscanf(opened_file_ptr, "%*s %f %f %f", &xp, &yp, &zp)) {
                    points.push_back(xp);
                    points.push_back(yp);
                    points.push_back(zp);
                }
                fclose(opened_file_ptr);
                opened_file_ptr = nullptr;
                if (point_cloud_vertices != nullptr) free(point_cloud_vertices);
                point_cloud_vertices = (GLfloat *)malloc(points.size() * sizeof(GLfloat));
                for (size_t i = 0; i < points.size(); ++i)
                    point_cloud_vertices[i] = points[i];
                point_cloud_vertices_num = points.size() / 3;
                points.clear();
                // set up data for shader
                unsigned int &vbo = pointCloudVBO, &vao = pointCloudVAO;
                glGenVertexArrays(1, &vao);
                glUseProgram(pointCloudShaderProgram);
                // projection matrix
                glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)WINDOW_W / (float)WINDOW_H, 0.1f, 100.f);
                setMat4(pointCloudShaderProgram, "projection", projection);
                // model matrix
                glm::mat4 model = glm::mat4(1.0f);
                setMat4(pointCloudShaderProgram, "model", model);
                glGenBuffers(1, &vbo);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * point_cloud_vertices_num * 3, point_cloud_vertices, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
            
            ImGui::Text(("Current opened: " + (opened_file == -1 ? "" : filenames[opened_file].filename)).c_str());
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * 0.05f);
            ImGui::BeginChild("File Panel", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.9f, 100), false, ImGuiWindowFlags_HorizontalScrollbar);
            for (int i = 0; i < filenames.size(); ++i) {
                if (ImGui::Selectable(filenames[i].filename.c_str(), selected == i)) {
                    selected = i;
                }
            }
            ImGui::EndChild();
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        if (opened_file != -1) {
            glUseProgram(pointCloudShaderProgram);
            glBindVertexArray(pointCloudVAO);
            // view matrix
            auto view = camera.getView();
            setMat4(pointCloudShaderProgram, "view", view);
            glDrawArrays(GL_POINTS, 0, point_cloud_vertices_num);
            glBindVertexArray(0);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (opened_file != -1) {
        glDeleteVertexArrays(1, &pointCloudVAO);
        glDeleteBuffers(1, &pointCloudVBO);
    }
    glDeleteProgram(pointCloudShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}