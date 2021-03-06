//
//  Shader.h
//  Shaders
//
//  Created by Crunchy on 5/26/20.
//  Copyright © 2020 AnOrganization. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    Shader(const char * vertexPath, const char * fragmentPath);
    
    // Variables
    unsigned int ID;
    
    // Methods
    void use();

    // Set Primitive
    void setBool (const std::string& name, bool value)  const;
    void setInt  (const std::string& name, int value)   const;
    void setFloat(const std::string& name, float value) const;

    // Set Vector

    // Set Matrix
    
private:
    bool FileExists(const std::string& path);
    void CheckCompileErrors(GLuint shader, const std::string& type);
};

#endif
