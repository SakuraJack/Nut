#pragma once

#define NUT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

using byte = uint8_t;
using RenderID = uint32_t;
#define BIT(x) (1u << x)
