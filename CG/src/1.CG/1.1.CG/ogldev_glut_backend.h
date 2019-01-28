
#ifndef GLUT_BACKEND_H
#define	GLUT_BACKEND_H

#include "ogldev_types.h"
#include "ogldev_callbacks.h"


void GLUTBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil);


bool GLUTBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

void GLUTBackendRun(ICallbacks* pCallbacks);

void GLUTBackendSwapBuffers();

void GLUTBackendLeaveMainLoop();

OGLDEV_KEY GLUTKeyToOGLDEVKey(uint Key);


#endif	/* GLUT_BACKEND_H */
