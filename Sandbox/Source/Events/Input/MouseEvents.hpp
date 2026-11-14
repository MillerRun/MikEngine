#pragma once

#include "MouseTypes.hpp"
#include <glm/ext/vector_float2.hpp>

struct EventMousePressed
{
   MK::EMouseButton eCode{};
};

struct EventMouseReleased
{
   MK::EMouseButton eCode{};
};

struct EventMouseMoved
{
   glm::vec2 v2Position{};
   glm::vec2 v2Delta{};
};

struct EventMouseScrolled
{
   glm::vec2 v2Offset{}; // y is the regular wheel, x is horizontal scroll (touchpads, tilt wheels)
};
