#ifndef __VOLUME_RENDERER_H__
#define __VOLUME_RENDERER_H__

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glext.h>
//#include <GL/glut.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtx/transform2.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

#define GL_ERROR() checkForOpenGLError(__FILE__, __LINE__)

using namespace std;
using glm::mat4;
using glm::vec3;

#if 0
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutCreateWindow("GLUT Test");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
	/* Problem: glewInit failed, something is seriously wrong. */
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
  
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&display);
    glutReshapeFunc(&reshape);
    glutIdleFunc(&rotateDisplay);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}
#endif

int checkForOpenGLError(const char* file, int line);
GLint checkShaderLinkStatus(GLuint pgmHandle);
void checkFramebufferStatus();
GLboolean compileCheck(GLuint shader);
GLuint createShaderPgm();
void drawBox(GLenum glFaces);
GLuint initShaderObj(const GLchar* srcfile, GLenum shaderType);
void linkShader(GLuint shaderPgm, GLuint newVertHandle, GLuint newFragHandle);

class VolumeRenderer
{
  GLuint g_vao;
  GLuint g_programHandle;
  GLuint g_winWidth;
  GLuint g_winHeight;
  GLint g_angle;
  GLuint g_frameBuffer;
  // transfer function
  GLuint g_tffTexObj;
  GLuint g_bfTexObj;
  GLuint g_texWidth;
  GLuint g_texHeight;
  GLuint g_volTexObj;
  GLuint g_rcVertHandle;
  GLuint g_rcFragHandle;
  GLuint g_bfVertHandle;
  GLuint g_bfFragHandle;
  float g_stepSize;

  void keyboard(unsigned char key, int x, int y);
  void display(void);
  void reshape(int w, int h);

  void initVBO();
  void initShader();
  void initFrameBuffer(GLuint, GLuint, GLuint);
  GLuint initTFF1DTex(const char* filename);
  GLuint initFace2DTex(GLuint texWidth, GLuint texHeight);
  GLuint initVol3DTex(const char* filename, GLuint width, GLuint height, GLuint depth);
  void render(GLenum cullFace);
  void rcSetUinforms();
  void rotateDisplay();
  void drawBox(GLenum glFaces);

  void init()
  {
    g_texWidth = g_winWidth;
    g_texHeight = g_winHeight;
    initVBO();
    initShader();
    g_tffTexObj = initTFF1DTex("tff.dat");
    g_bfTexObj = initFace2DTex(g_texWidth, g_texHeight);
    g_volTexObj = initVol3DTex("head256.raw", 256, 256, 225);
    GL_ERROR();
    initFrameBuffer(g_bfTexObj, g_texWidth, g_texHeight);
    GL_ERROR();
  }

public:
  VolumeRenderer():g_winWidth(400),g_winHeight(400),g_angle(0),g_stepSize(0.001f)
  {
    init();
  }
  ~VolumeRenderer()
  {
  }
};

#endif // __VOLUME_RENDERER_H__