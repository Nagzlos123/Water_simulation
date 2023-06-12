#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <iostream>
#include <vector>
#include "OBJ_Loader.h"
#include<stb/std_image.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace std;
using namespace glm;

class Model
{
private:
	GLuint Shader;
	GLuint LightLoc;
	

    string LoadTextFile(string fileName)
    {
        ifstream file;
        file.open(fileName, ios::in);
        if (!file.is_open())
        {
            cout << "Open file error: " << fileName << endl;
            system("pause");
            exit(0);
        }
        // Odczyt calosci pliku:
        string line, buff;
        while (!file.eof())
        {
            getline(file, line);
            buff += line + "\n";
        }
        return(buff);
    }
public:
    GLuint VAO;
    GLuint TEX;
    objl::Loader model;
    vector<unsigned int> model_indices;
    mat4 MatV;
    mat4 MatP;
    mat4 MatM;

    void InitShader(string vertShader, string fragShader)
    {
        // Wczytanie kodu shaderow z plikow:
        string VertShaderCode = LoadTextFile(vertShader);
        string FragShaderCode = LoadTextFile(fragShader);
        const char* src[1];

        // Inicjalizacja shaderow:
        GLuint vertShr = glCreateShader(GL_VERTEX_SHADER);
        src[0] = VertShaderCode.c_str();
        glShaderSource(vertShr, 1, src, NULL);
        glCompileShader(vertShr);

        GLuint fragShr = glCreateShader(GL_FRAGMENT_SHADER);
        src[0] = FragShaderCode.c_str();
        glShaderSource(fragShr, 1, src, NULL);
        glCompileShader(fragShr);

        // Utworzenie programu i polaczenie shaderow:
        Shader = glCreateProgram();
        glAttachShader(Shader, fragShr);
        glAttachShader(Shader, vertShr);
        glLinkProgram(Shader);

        glUseProgram(Shader);
    }

    void InitModel(string obj, string tex)
    {

    }
};
#endif // MODEL_H_INCLUDED