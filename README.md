# MiniEngine

MiniEngine is a lightweight engine, using GLUT (miniglut) and OpenGL. 
The goal is to create something tiny, performant and easy-to-use.

## Using GLUT / freeglut instead of MiniGLUT
MiniGLUT is a simple library for Windows and Linux support. If one wishes
to use regular GLUT or freeglut, then this can be done by defining one of the following **before including `miniengine`**:  
- `#define MINI_USE_GLUT`
- `#define MINI_USE_FREEGLUT`

The benefit to doing the above, is more supported platforms as well as making
`miniengine` header-only, not having to include `miniglut.c` and `miniglut.h`.

