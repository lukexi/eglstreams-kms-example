/*
 * Copyright (c) 2016, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "utils.h"
#include "egl.h"
#include "kms.h"
#include "GL/gl.h"
#include "math.h"
#include <stdio.h>

/*
 * Example code demonstrating how to connect EGL to DRM KMS using
 * EGLStreams.
 */

int main(void)
{
    // Setup global EGL device state
    GetEglExtensionFunctionPointers();

    EGLDeviceEXT eglDevice = GetEglDevice();

    int drmFd = GetDrmFd(eglDevice);

    EGLDisplay eglDpy     = GetEglDisplay(eglDevice, drmFd);
    EGLConfig  eglConfig  = GetEglConfig(eglDpy);
    EGLContext eglContext = GetEglContext(eglDpy, eglConfig);


    // Set up EGL state for each connected display
    int NumPlanes;
    kms_plane* Planes     = SetDisplayModes(drmFd, &NumPlanes);
    int NumDisplays = NumPlanes;
    egl_display* Displays = GetEglDisplays(eglDpy, eglConfig, eglContext, Planes, NumPlanes);

    while(1) {
        glClearColor(
                (sin(GetTime()*3)/2+0.5) * 0.8,
                (sin(GetTime()*5)/2+0.5) * 0.8,
                (sin(GetTime()*7)/2+0.5) * 0.8,
                1);

        for (int DisplayIndex = 0; DisplayIndex < NumDisplays; DisplayIndex++) {
            egl_display* Display = &Displays[DisplayIndex];
            eglMakeCurrent(eglDpy, Display->Surface, Display->Surface, Display->Context);
            glClear(GL_COLOR_BUFFER_BIT);
            eglSwapBuffers(eglDpy, Display->Surface);
        }
        PrintFps();
    }

    return 0;
}
