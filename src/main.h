#ifndef MAIN_H
#define MAIN_H


///////////////////////
//~ fz_std
#include "fz_include.h"

#define ERROR_FILE Str8("D:/work/Placeholder/error.log")

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