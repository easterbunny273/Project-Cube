// small header to help including lua
#pragma once

#ifndef __PROJECT_CUBE_LUA_INCLUDE
#define __PROJECT_CUBE_LUA_INCLUDE

extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}
#include <luabind/luabind.hpp>

#endif
