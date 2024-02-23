#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderUtils {
public:
    static GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path) {
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if (VertexShaderStream.is_open()) {
            std::string Line = "";
            while (getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        } else {
            std::cerr << "Impossible to open " << vertex_file_path << ". Are you in the right directory ? Don't forget to read the FAQ !" << std::endl;
            return 0;
        }
        
        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open()) {
            std::string Line = "";
            while (getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        } else {
            std::cerr << "Impossible to open " << fragment_file_path << ". Are you in the right directory ? Don't forget to read the FAQ !" << std::endl;
            return 0;
        }

        // Compile Vertex Shader
        std::cout << "Compiling shader : " << vertex_file_path << std::endl;
        const char *VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);
        
        // Compile Fragment Shader
        std::cout << "Compiling shader : " << fragment_file_path << std::endl;
        const char *FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Vertex Shader
        CheckShader(VertexShaderID);

        // Check Fragment Shader
        CheckShader(FragmentShaderID);

        // Link the program
        std::cout << "Linking program\n";
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            std::cerr << &ProgramErrorMessage[0] << std::endl;
        }

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

private:
    static void CheckShader(GLuint shaderID) {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
            std::cerr << &ShaderErrorMessage[0] << std::endl;
        }
    }
};

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Load shaders
    GLuint program = ShaderUtils::LoadShaders("../shader/default.vert.glsl", "../shader/default.frag.glsl");
    if (!program) {
        std::cerr << "Failed to load shader program" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
