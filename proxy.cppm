module;

#include <assert.h>
#include <limits.h>
#include <stddef.h>

export module proxy;
import std;

#define MICROSOFT_PROXY_USE_MODULE

export extern "C++" {
    #include "proxy.h"
}