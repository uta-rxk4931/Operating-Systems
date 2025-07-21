#pragma once

#include "egos.h"
#include "servers.h"
#include "libc/libc.h"

struct grass *grass = (void*)APPS_STACK_TOP;
struct earth *earth = (void*)GRASS_STACK_TOP;
