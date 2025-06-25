#ifndef MAIN_H
#define MAIN_H

///////////////////////
//~ fz_std
#define FZ_WINDOW_NAME "Placeholder"
#include "fz_include.h"

#define FZ_ENABLE_ASSERT  1
#define FZ_ENABLE_WINDOW  1
#define FZ_ENABLE_CONSOLE 1
#define FZ_ENABLE_OPENGL  1

#define DeltaTime   _Timer_DeltaTime.elapsed_seconds
#define FrameTime   _Timer_FrameTime.elapsed_seconds
#define Fps         (DeltaTime > 0.0f ? 1.0f / DeltaTime : 0.0f)
#define ElapsedTime _get_elapsed_time()

///////////////////////
//~ *.h

#define FZ_CAMERA_SENSITIVITY 1.0f
#define FZ_CAMERA_SPEED       8.0f

#include "fz_camera.h"
#include "renderer.h"

///////////////////////
//~ *.c
#include "fz_camera.c"
#include "renderer.c"

global Camera GlobalCamera;

internal void entry_point();
internal void input_update();

#endif // MAIN_H