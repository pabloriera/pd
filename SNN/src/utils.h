#pragma once

#include "float.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))

float scale(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp);

bool file_exists (const std::string& name);