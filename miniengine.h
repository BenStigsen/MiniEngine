// TODO:
//   - Add line width (use triangles)
//   - Remove deprecated GL_QUADS (use triangles)

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef MINI_H
#define MINI_H

// --- Constants --- //
#define MB_LEFT   1 
#define MB_MIDDLE 2 
#define MB_RIGHT  4 

// --- Structures --- // 
typedef struct {
  int a;
  int b;
} Tuple;

typedef struct {
  int x;
  int y;
} Vec2;

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} Color;

typedef struct {
  int x;
  int y;
} Mouse;

// --- Functions --- // 
void windowInit(int w, int h, const char *title);
void windowUpdate();
void windowClose();
void windowClear(Color color);

Color colorRGB(int r, int g, int b);
Color colorRGBA(int r, int g, int b, int a);

// Input
Vec2 mousePosition();
int keyboardDown(unsigned char key);

// Shapes
void drawLine(int x0, int y0, int x1, int y1, Color color);
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color);
void drawTriangleFilled(int x0, int y0, int x1, int y1, int x2, int y2, Color color);
void drawRectangle(int x, int y, int w, int h, Color color);
void drawRectangleFilled(int x, int y, int w, int h, Color color);
void drawCircle(int x, int y, int r, Color color);
void drawCircleFilled(int x, int y, int r, Color color);
void drawPolygon(int *points, int count, Color color);
void drawPolygonFilled(int *points, int count, Color color);

#endif // MINI_H

// ------------------------------------ //
// ---------- IMPLEMENTATION ---------- //
// ------------------------------------ //
#ifdef MINI_IMPLEMENTATION
  #include <math.h>
  #include <GLFW/glfw3.h>
  
  GLFWwindow *_window = NULL;

  // --- WINDOW --- //
  void _resize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0.0f, 1.0f);
  };
  
  void windowInit(int w, int h, const char *title) {
    if (!glfwInit()) {
      exit(-1);
    }

    _window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!_window) {
        glfwTerminate();
        exit(-1);
    }
    
    glEnable(GL_TEXTURE_2D);
    glfwMakeContextCurrent(_window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0.0f, 1.0f);
    glClearColor(0.3, 0.3, 0.9 ,0);
  }
  
  int windowShouldClose() { return glfwWindowShouldClose(_window); }
  
  void windowUpdate() { glfwSwapBuffers(_window); glfwPollEvents(); }
  
  void windowClose() { glfwTerminate(); }
  
  void windowClear(Color color) {
    float r = (float)color.r / 0xFF;
    float g = (float)color.g / 0xFF;
    float b = (float)color.b / 0xFF;
    float a = (float)color.a / 0xFF;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  }
  
  Color colorHex(unsigned int c) {
    unsigned int r = (c >> 24) & 0xFF;
    unsigned int g = (c >> 16) & 0xFF;
    unsigned int b = (c >>  8) & 0xFF;
    unsigned int a = c & 0xFF;
  
    return (Color){r, g, b, a};
  }
  
  Color colorRGB(int r, int g, int b) {
    return (Color){r, g, b, 255};
  }
  
  Color colorRGBA(int r, int g, int b, int a) {
    return (Color){r, g, b, a};
  }
  
  
  // --- DRAWING --- //
  void drawLine(int x0, int y0, int x1, int y1, Color color) {
    glBegin(GL_LINES);
      glColor4ub(color.r, color.g, color.b, color.a); 
      glVertex2i(x0, y0);
      glVertex2i(x1, y1);
    glEnd();
  }
  
  // Triangles
  void _drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color, int mode) {
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a); 
      glVertex2i(x0, y0);
      glVertex2i(x1, y1);
      glVertex2i(x2, y2);
    glEnd();
  }
  
  inline void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
    _drawTriangle(x0, y0, x1, y1, x2, y2, color, GL_LINE_LOOP);
  }
  
  inline void drawTriangleFilled(int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
    _drawTriangle(x0, y0, x1, y1, x2, y2, color, GL_TRIANGLES);
  }
  
  // Rectangles
  void _drawRectangle(int x, int y, int w, int h, Color color, int mode) {
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a); 
      glVertex2i(x, y);
      glVertex2i(x + w, y);
      glVertex2i(x + w, y + h);
      glVertex2i(x, y + h);
    glEnd();
  }
  
  inline void drawRectangle(int x, int y, int w, int h, Color color) {
    _drawRectangle(x, y, w, h, color, GL_LINE_LOOP);
  }
  
  inline void drawRectangleFilled(int x, int y, int w, int h, Color color) {
    _drawRectangle(x, y, w, h, color, GL_QUADS);
  }
  
  // Polygons
  void _drawPolygon(int *points, int count, Color color, int mode) {
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a); 
      for (int i = 0; i < count; i += 2) {
        glVertex2i(points[i], points[i+1]);
      }
      
      if (mode == GL_TRIANGLE_STRIP) {
        glVertex2i(points[0], points[1]);
      }
      
    glEnd();
  }
  
  inline void drawPolygon(int *points, int count, Color color) {
    _drawPolygon(points, count, color, GL_LINE_LOOP);
  }
  
  inline void drawPolygonFilled(int *points, int count, Color color) {
    _drawPolygon(points, count, color, GL_TRIANGLE_STRIP);
  }
  
  // Circles
  void _drawCircle(int x, int y, int r, Color color, int mode) {
      float rr = (float)r;
      float xx = (float)x;
      float yy = (float)y;
      int segments = 36;
      
    	glBegin(mode);
        glColor4ub(color.r, color.g, color.b, color.a);
        if (mode == GL_TRIANGLE_STRIP) {
          glVertex2i(x, y);
        }
        
        for (int i = 0; i <= segments; ++i) {
          glVertex2f(x + (r * cos(i * (M_PI * 2) / segments)), y + (r * sin(i * (M_PI * 2) / segments)));
        }
      glEnd();
  }
  
  inline void drawCircle(int x, int y, int r, Color color) {
    _drawCircle(x, y, r, color, GL_LINE_LOOP);
  }
  
  inline void drawCircleFilled(int x, int y, int r, Color color) {
    _drawCircle(x, y, r, color, GL_TRIANGLE_STRIP);
  }

#endif // MINI_IMPLEMENTATION
