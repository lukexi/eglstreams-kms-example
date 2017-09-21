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

#if !defined(EGL_H)
#define EGL_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

/* XXX khronos eglext.h does not yet have EGL_DRM_MASTER_FD_EXT */
#if !defined(EGL_DRM_MASTER_FD_EXT)
#define EGL_DRM_MASTER_FD_EXT                   0x333C
#endif

#if !defined(EGL_CONSUMER_AUTO_ACQUIRE_EXT)
#define EGL_CONSUMER_AUTO_ACQUIRE_EXT         0x332B
#endif

#if !defined(EGL_DRM_FLIP_EVENT_DATA_NV)
#define EGL_DRM_FLIP_EVENT_DATA_NV            0x333E
#endif

#if !defined(EGL_RESOURCE_BUSY_EXT)
#define EGL_RESOURCE_BUSY_EXT                        0x3353
#endif

#if !defined(EGL_BAD_STATE_KHR)
#define EGL_BAD_STATE_KHR                 0x321C
#endif



EGLDeviceEXT GetEglDevice(void);

int GetDrmFd(EGLDeviceEXT device);

EGLDisplay GetEglDisplay(EGLDeviceEXT device, int drmFd);

EGLSurface SetUpEgl(EGLDisplay eglDpy, uint32_t crtcId, uint32_t planeId, int width, int height, EGLStreamKHR *eglStream);

EGLBoolean EglFlip(EGLDisplay eglDpy, EGLStreamKHR eglStream);
EGLint EglCheckStreamState(EGLDisplay eglDpy, EGLStreamKHR eglStream);

void EGLCheck(const char* name);

#endif /* EGL_H */
