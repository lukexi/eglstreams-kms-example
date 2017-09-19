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
#include "eglgears.h"
#include <stdio.h>
#include <unistd.h>
/*
 * Example code demonstrating how to connect EGL to DRM KMS using
 * EGLStreams.
 */

void PrintStreamState(EGLint streamState) {
    switch (streamState) {
        case EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR:
            printf("EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR\n");
            break;
        case EGL_STREAM_STATE_OLD_FRAME_AVAILABLE_KHR:
            printf("EGL_STREAM_STATE_OLD_FRAME_AVAILABLE_KHR\n");
            break;
        case EGL_STREAM_STATE_CREATED_KHR:
            printf("EGL_STREAM_STATE_CREATED_KHR\n");
            break;
        case EGL_STREAM_STATE_CONNECTING_KHR:
            printf("EGL_STREAM_STATE_CONNECTING_KHR\n");
            break;

        case EGL_STREAM_STATE_EMPTY_KHR:
            printf("EGL_STREAM_STATE_EMPTY_KHR\n");
            break;

        case EGL_STREAM_STATE_DISCONNECTED_KHR:
            printf("EGL_STREAM_STATE_DISCONNECTED_KHR\n");
            break;
    }

}

int main(void)
{
    EGLDisplay eglDpy;
    EGLDeviceEXT eglDevice;
    int drmFd, width, height;
    uint32_t crtcID = 0;
    uint32_t planeID = 0;
    EGLSurface eglSurface;

    GetEglExtensionFunctionPointers();

    eglDevice = GetEglDevice();

    drmFd = GetDrmFd(eglDevice);

    SetMode(drmFd, &crtcID, &planeID, &width, &height);
    printf("Set mode to %i %i\n", width, height);

    eglDpy = GetEglDisplay(eglDevice, drmFd);

    EGLStreamKHR eglStream;
    eglSurface = SetUpEgl(eglDpy, crtcID, planeID, width, height, &eglStream);
    (void)eglSurface;

    InitGears(width, height);

    int FrameCount = 0;
    while(1) {

        printf("Frame: %i\n", FrameCount++);
        DrawGears();
        eglSwapBuffers(eglDpy, eglSurface);

// This seems to not be strictly necessary...
#if 0
        EGLint streamState = 0;
        while (streamState != EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR) {
            streamState = EglCheckStreamState(eglDpy, eglStream);
            PrintStreamState(streamState);
            printf("Waiting for frame available...\n");
        }
#endif

        EGLBoolean r = pEglStreamConsumerAcquireAttribNV(eglDpy, eglStream, NULL);
        if (r == EGL_FALSE) {
            EGLCheck("EGL Flip");
        }

        // while (!EglFlip(eglDpy, eglStream)) {
        //     printf("Waiting...\n");
        // }
        // usleep(16000);
        PrintFps();
    }

    return 0;
}
