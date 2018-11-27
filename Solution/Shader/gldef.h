#pragma once

#if defined(_WIN32)

#define GLEW_STATIC
#define FREEGLUT_STATIC
#include<GL\glew.h>
#include<GL\GL.h>
	
#elif defined(__IPHONE__)

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif




#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"

#include<stdio.h>



#define DEG2RAD (3.141593f / 180)

