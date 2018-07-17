#pragma once

#ifdef  _WIN32
#include<GL\glew.h>
#include<GL\GL.h>
#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"

#else
#include<GLES2\gl2.h>
#include<GLES2\gl2ext.h>
#endif 

#define DEG2RAD 3.141593f / 180

