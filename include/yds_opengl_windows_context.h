#ifndef YDS_OPENGL_WINDOWS_CONTEXT_H
#define YDS_OPENGL_WINDOWS_CONTEXT_H

#include "yds_opengl_context.h"

#if defined(__APPLE__) && defined(__MACH__) // Apple OSX & iOS (Darwin)
    #include <OpenGL/OpenGL.h>
#elif defined(_WIN64)
    #include "OpenGL.h"
#endif

#if defined(_WIN64)

class ysOpenGLDevice;

class ysOpenGLWindowsContext : public ysOpenGLVirtualContext {
    friend ysOpenGLDevice;

public:
    ysOpenGLWindowsContext();
    virtual ~ysOpenGLWindowsContext();

    ysError CreateRenderingContext(ysOpenGLDevice *device, ysWindow *window, int major, int minor);

    virtual ysError DestroyContext();
    virtual ysError TransferContext(ysOpenGLVirtualContext *context);
    virtual ysError SetContextMode(ContextMode mode);
    virtual ysError SetContext(ysRenderingContext *realContext);
    virtual ysError Present();

protected:
    HDC m_deviceHandle;
    HGLRC m_contextHandle;

    ysOpenGLDevice *m_device;

protected:
    void LoadAllExtensions();
    void LoadContextCreationExtension();
};

#endif /* Windows */

#endif /* YDS_OPENGL_WINDOWS_CONTEXT_H */
