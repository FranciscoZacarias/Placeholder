#ifndef MAIN_H
#define MAIN_H

///////////////////////
//~ fz_std
#define FZ_WINDOW_NAME "Placeholder"
#include "fz_include.h"

///////////////////////
//~ *.h
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