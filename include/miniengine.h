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
  #include "miniglut.h"
  
  inline static void _empty() {};
  
  Tuple _mouseButtons(int update, int mbutton, int mstate) {
    static int button;
    static int state;
    
    if (update) { 
      if (mbutton == GLUT_LEFT_BUTTON) { 
        button = MB_LEFT; 
      } else if (mbutton == GLUT_MIDDLE_BUTTON) {
        button = MB_MIDDLE; 
      } else if (mbutton == GLUT_RIGHT_BUTTON) {
        button = MB_RIGHT; 
      } else {
        button = 0; 
      }
      state = mstate; 
    }
    return (Tuple){button, state};
  }
  
  Vec2 _mousePosition(int update, int mx, int my) {
    static int x;
    static int y;
    
    if (update) { x = mx; y = my; }
    return (Vec2){x, y};
  }
  
  void _mouseMotion(int x, int y) {
    _mousePosition(1, x, y);
  }
  
  void _mouse(int button, int state, int x, int y) {
    _mousePosition(1, x, y);
    _mouseButtons(1, button, state);
  }
  
  Vec2 mousePosition() { return _mousePosition(0, 0, 0); }
  int mouseDown(int button) { 
    Tuple mouse = _mouseButtons(0, 0, 0);
    return (mouse.a & button) && mouse.b == GLUT_DOWN; 
  }
  
  // --- WINDOW --- //
  void _resize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0.0f, 1.0f);
  };
  
  void windowInit(int w, int h, const char *title) {
    glutInit(0, NULL);
    glutInitWindowSize(w, h);
    glutInitDisplayMode(GLUT_ALPHA);
    glutCreateWindow(title);
    
    glutDisplayFunc(_empty);
    glutMouseFunc(_mouse);
    glutMotionFunc(_mouseMotion);
    glutPassiveMotionFunc(_mouseMotion);
    glutReshapeFunc(_resize);
    /*
    glutKeyboardFunc(keypress);
    glutMouseFunc(mouse);
    */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0.0f, 1.0f);
    glClearColor(0.3, 0.3, 0.9 ,0);
  }
  
  void windowUpdate() { glutSwapBuffers(); glutMainLoopEvent(); }
  
  void windowClose() { glutExit(); }
  
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
  
  void _drawCircle(int x, int y, int r, Color color, int mode) {
      float rr = (float)r;
      float xx = (float)x;
      float yy = (float)y;
      int segments = 36;
      
    	glBegin(GL_TRIANGLE_FAN);
        glColor4ub(color.r, color.g, color.b, color.a);
        glVertex2i(x, y);
        for (int i = 0; i <= segments; ++i) {
          glVertex2f(x + (r * cos(i * (M_PI * 2) / segments)), y + (r * sin(i * (M_PI * 2) / segments)));
        }
      glEnd();
  }
  
  inline void drawCircle(int x, int y, int r, Color color) {
    _drawCircle(x, y, r, color, GL_LINES);
  }
  
  inline void drawCircleFilled(int x, int y, int r, Color color) {
    _drawCircle(x, y, r, color, GL_TRIANGLE_STRIP);
  }
  // TODO: Circle

#endif // MINI_IMPLEMENTATION
