#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

class ShaderUtils {
public:
    static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
};