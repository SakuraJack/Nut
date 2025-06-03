#pragma once

#ifdef NT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <string>
#include <memory>
#include <functional>
#include <filesystem>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include <string_view>
#include <iostream>

#include "Nut/Core/Base.h"
#include "Nut/Core/Log.h"
#include "Nut/Core/Memory.h"
#include "Nut/Events/Event.h"