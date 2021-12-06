#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define SDL_MAIN_HANDLED

#define RUN_TESTS 0

#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

#include "opcodes.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef char s8;
typedef short s16;
typedef int s32;

const int APP_WIDTH = 1200;
const int APP_HEIGHT = 950;

#define APP_SCALE 3