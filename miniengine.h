#ifndef MINI_H
#define MINI_H
  #include <stdlib.h>
  #include <stdbool.h>
  #include <stdio.h>
  
  // --- Constants --- //
  #define PI 3.14159265358979323846
  #define DEG2RAD (PI / 180.0f)
  #define TAU (PI * 2.0)
  
  #define MB_LEFT   1 
  #define MB_MIDDLE 2 
  #define MB_RIGHT  4 
  
  typedef enum { RELEASE, PRESS, CLICK, NONE } MouseState;
  
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
    MouseState state;
    int button;
  } Mouse;
  
  typedef struct {
    int state;
    int key;
  } Keyboard;
  
  typedef struct {
    int x;
    int y;
    int w;
    int h;
    Mouse mouse;
    Mouse mouseprev;
    Keyboard keyboard;
    double prev;
    double dt;
  } Window;
  
  // --- Functions --- // 
  void windowInit(int w, int h, const char *title);
  void windowUpdate();
  void windowClose();
  void windowClear(Color color);
  
  Color colorHex(unsigned int c);
  Color colorRGB(int r, int g, int b);
  Color colorRGBA(int r, int g, int b, int a);
  
  // Time
  double delta();
  
  // Input
  Vec2 mousePosition();
  int mouseClicked(int button);
  int mousePressed(int button);
  int mouseReleased(int button);
  
  int keyPressed(int key);
  int keyReleased(int key);
  
  // Shapes
  void drawLine(float x0, float y0, float x1, float y1, Color color);
  void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, Color color);
  void drawTriangleFilled(float x0, float y0, float x1, float y1, float x2, float y2, Color color);
  void drawRectangle(float x, float y, float w, float h, Color color);
  void drawRectangleFilled(float x, float y, float w, float h, Color color);
  void drawCircle(float x, float y, float r, Color color);
  void drawCircleFilled(float x, float y, float r, Color color);
  void drawPolygon(float *pofloats, int count, Color color);
  void drawPolygonFilled(float *points, int count, Color color);
  void drawSlice(float x, float y, float r, float start, float end, Color color);
  void drawSliceFilled(float x, float y, float r, float start, float end, Color color);
  void drawArc(float x, float y, float r1, float thickness, float start, float end, Color color);
  void drawArcFilled(float x, float y, float r1, float thickness, float start, float end, Color color);
#endif // MINI_H

// ------------------------------------ //
// ---------- IMPLEMENTATION ---------- //
// ------------------------------------ //
#ifdef MINI_IMPLEMENTATION
  #include <math.h>
  #include <GLFW/glfw3.h>

  GLFWwindow *glfw_window;

  // --- INTERNAL --- //
  void _resize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0.0f, 1.0f);
  };
  
  void _mouseButton(GLFWwindow *window, int button, int action, int mods) {
    Window *win = glfwGetWindowUserPointer(window);
    win->mouse.button = button;
    win->mouse.state = action;
  }
  
  // --- WINDOW --- //
  
  void windowInit(int w, int h, const char *title) {
    if (!glfwInit()) {
      exit(-1);
    }

    glfw_window = glfwCreateWindow(w, h, "Hello World", NULL, NULL);
    if (!glfw_window) {
        glfwTerminate();
        exit(-1);
    }
    
    Window window = {0};
    window.w = w;
    window.h = h;
    window.dt = 0.0;
    window.prev = 0.0;
    
    glfwSetWindowUserPointer(glfw_window, &window);
    glfwSetMouseButtonCallback(glfw_window, _mouseButton);
    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1.0f);
    glClearColor(0.3, 0.3, 0.9 ,0);
  }
  
  int windowShouldClose() { return glfwWindowShouldClose(glfw_window); }
  
  void windowUpdate() { 
    glfwSwapBuffers(glfw_window);
    
    Window *win = glfwGetWindowUserPointer(glfw_window);
    Mouse prev = win->mouse;
    
    #if defined(MINI_GUI_MODE)
      glfwWaitEvents();
    #else
      glfwPollEvents();
    #endif
    
    if (prev.state == CLICK && win->mouse.state == CLICK) {
      win->mouse.state == PRESS;
    } else if (prev.state == RELEASE && win->mouse.state == RELEASE) {
      win->mouse.state == NONE;
    }
    
    double current = glfwGetTime();
    win->dt = current - win->prev;
    win->prev = current;
  }
  
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
    
    if (c <= 0xFF)     { return (Color){c, c, c, 0xFF}; }
    if (c <= 0xFFFFFF) { return (Color){r, g, b, 0xFF}; }
  
    return (Color){r, g, b, a};
  }
  
  Color colorRGB(int r, int g, int b) {
    return (Color){r, g, b, 255};
  }
  
  Color colorRGBA(int r, int g, int b, int a) {
    return (Color){r, g, b, a};
  }
  
  // --- TIME --- //
  double delta() {
    Window *win = glfwGetWindowUserPointer(glfw_window);
    return win->dt;
  }
  
  // --- INPUT --- //
  Vec2 mousePosition() {
    double x;
    double y;
    glfwGetCursorPos(glfw_window, &x, &y);
    return (Vec2){(int)x, (int)y};
  }
  
  int mouseClicked(int button) {
    Window *win = glfwGetWindowUserPointer(glfw_window);
    return win->mouse.button == button && win->mouse.state == CLICK;
  }
  
  int mousePressed(int button) {
    Window *win = glfwGetWindowUserPointer(glfw_window);
    return win->mouse.button == button && win->mouse.state == PRESS;
  }
  
  int mouseReleased(int button) {
    Window *win = glfwGetWindowUserPointer(glfw_window);
    return win->mouse.button == button && win->mouse.state == RELEASE;
  }
  
  int keyPressed(int key) {
    return glfwGetKey(glfw_window, key) == GLFW_PRESS;
  }
  
  // Not working as intended
  // TODO: Add keyHeld();
  int keyReleased(int key) {
    return glfwGetKey(glfw_window, key) == GLFW_RELEASE;
  }
  
  // --- DRAWING --- //
  void drawLine(float x0, float y0, float x1, float y1, Color color) {
    glBegin(GL_LINES);
      glColor4ub(color.r, color.g, color.b, color.a); 
      glVertex2f(x0, y0);
      glVertex2f(x1, y1);
    glEnd();
  }
  
  // Triangles
  void _drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, Color color, int mode) {
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a); 
      glVertex2f(x0, y0);
      glVertex2f(x1, y1);
      glVertex2f(x2, y2);
    glEnd();
  }
  
  inline void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, Color color) {
    _drawTriangle(x0, y0, x1, y1, x2, y2, color, GL_LINE_LOOP);
  }
  
  inline void drawTriangleFilled(float x0, float y0, float x1, float y1, float x2, float y2, Color color) {
    _drawTriangle(x0, y0, x1, y1, x2, y2, color, GL_TRIANGLES);
  }
  
  // Rectangles
  void _drawRectangle(float x, float y, float w, float h, Color color, int mode) {
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a); 
      glVertex2f(x, y);
      glVertex2f(x + w, y);
      glVertex2f(x + w, y + h);
      glVertex2f(x, y + h);
    glEnd();
  }
  
  inline void drawRectangle(float x, float y, float w, float h, Color color) {
    _drawRectangle(x, y, w, h, color, GL_LINE_LOOP);
  }
  
  inline void drawRectangleFilled(float x, float y, float w, float h, Color color) {
    _drawRectangle(x, y, w, h, color, GL_TRIANGLE_FAN);
  }
  
  // Polygons
  void _drawPolygon(float *points, int count, Color color, int mode) {
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a); 
      for (int i = 0; i < count; i += 2) {
        glVertex2f(points[i], points[i+1]);
      }
      
      if (mode == GL_TRIANGLE_STRIP) {
        glVertex2f(points[0], points[1]);
      }
      
    glEnd();
  }
  
  inline void drawPolygon(float *points, int count, Color color) {
    _drawPolygon(points, count, color, GL_LINE_LOOP);
  }
  
  inline void drawPolygonFilled(float *points, int count, Color color) {
    _drawPolygon(points, count, color, GL_TRIANGLE_STRIP);
  }
  
  // Circles
  void _drawCircle(float x, float y, float r, Color color, int mode) {
    int segments = 36;
    
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a);
      if (mode == GL_TRIANGLE_STRIP) {
        glVertex2f(x, y);
      }
      
      for (int i = 0; i <= segments; ++i) {
        glVertex2f(x + (r * cos(i * (M_PI * 2) / segments)), y + (r * sin(i * (M_PI * 2) / segments)));
      }
    glEnd();
  }
  
  inline void drawCircle(float x, float y, float r, Color color) {
    _drawCircle(x, y, r, color, GL_LINE_LOOP);
  }
  
  inline void drawCircleFilled(float x, float y, float r, Color color) {
    _drawCircle(x, y, r, color, GL_TRIANGLE_FAN);
  }
  
  // Slices
  void _drawSlice(float x, float y, float r, float start, float end, Color color, int mode) {
    int segments = 36;
    
    end = end - start > 360 ? start + 360 : end;
    float stride = (end - start) / (float)segments;
    float angle = start;
    
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a);
      
      if (mode == GL_TRIANGLE_FAN) {
        for (int i = 0; i < segments; ++i) {
          float a = DEG2RAD * angle;
          glVertex2f(x, y);
          glVertex2f(x + sin(a) * r, y + cos(a) * r);
          glVertex2f(x + sin(DEG2RAD * (angle + stride)) * r, y + cos(DEG2RAD * (angle + stride)) * r);
          glVertex2f(x + sin(DEG2RAD * (angle + stride * 2)) * r, y + cos(DEG2RAD * (angle + stride * 2)) * r);
          angle += stride;
        }
      } else {
        if (end < 360) { glVertex2f(x, y); }
        
        for (int i = 0; i < segments; ++i) {
          glVertex2f(x + sin(DEG2RAD * (angle + stride)) * r, y + cos(DEG2RAD * (angle + stride)) * r);
          glVertex2f(x + sin(DEG2RAD * (angle + stride * 2)) * r, y + cos(DEG2RAD * (angle + stride * 2)) * r);
          angle += stride;
        }
      }
    glEnd();
  }
  
  inline void drawSlice(float x, float y, float r, float start, float end, Color color) {
    _drawSlice(x, y, r, start, end, color, GL_LINE_LOOP);
  }
  
  inline void drawSliceFilled(float x, float y, float r, float start, float end, Color color) {
    _drawSlice(x, y, r, start, end, color, GL_TRIANGLE_FAN);
  }
  
  // Rings
  void _drawArc(float x, float y, float r1, float thickness, float start, float end, Color color, int mode) {
    int segments = 36;
    
    float a1 = start;
    float a2 = end;
    
    float r2 = r1 + thickness;
    
    end = end - start > 360 ? start + 360 : end;
    
    float stride = (a2 - a1) / (float)segments;
    float angle = a1;
    
    glBegin(mode);
      glColor4ub(color.r, color.g, color.b, color.a);
      
      if (mode == GL_TRIANGLE_STRIP) {
        for (int i = 0; i < segments; ++i) {
          float a = DEG2RAD*angle;
        
          glVertex2f(x + sin(a) * r1, y + cos(a) * r1);
          glVertex2f(x + sin(a) * r2, y + cos(a) * r2);
          
          glVertex2f(x + sin(DEG2RAD * (angle + stride)) * r1, y + cos(DEG2RAD * (angle + stride)) * r1);
          glVertex2f(x + sin(DEG2RAD * (angle + stride)) * r1, y + cos(DEG2RAD * (angle + stride)) * r1);
          
          glVertex2f(x + sin(a) * r2, y + cos(a) * r2);
          glVertex2f(x + sin(DEG2RAD * (angle + stride)) * r2, y + cos(DEG2RAD * (angle + stride)) * r2);
          
          angle += stride;
        }
      } else {
        for (int i = 0; i < segments; ++i) {
          float a = DEG2RAD*angle;
          glVertex2f(x + sin(a) * r2, y + cos(a) * r2);
          glVertex2f(x + sin(DEG2RAD * (angle + stride)) * r2, y + cos(DEG2RAD * (angle + stride)) * r2);
          angle += stride;
        }
        
        if ((int)start == ((int)end % 360)) {
          glEnd();
          glBegin(mode);
        }
        
        for (int i = 0; i < segments; ++i) {
          float a = DEG2RAD*angle;
          glVertex2f(x + sin(a) * r1, y + cos(a) * r1);
          glVertex2f(x + sin(DEG2RAD * (angle - stride)) * r1, y + cos(DEG2RAD * (angle - stride)) * r1);
          angle -= stride;
        }
      }
    glEnd();
  }
  
  inline void drawArc(float x, float y, float r1, float thickness, float start, float end, Color color) {
    _drawArc(x, y, r1, thickness, start, end, color, GL_LINE_LOOP);
  }
  
  inline void drawArcFilled(float x, float y, float r1, float thickness, float start, float end, Color color) {
    _drawArc(x, y, r1, thickness, start, end, color, GL_TRIANGLE_STRIP);
  }

#endif // MINI_IMPLEMENTATION
