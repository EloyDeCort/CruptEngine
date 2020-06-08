#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#pragma warning(push)
#pragma warning(disable: 4505 26812)
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma warning(pop)
