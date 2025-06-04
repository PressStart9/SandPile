#pragma once

#include <cstdint>

struct Cell {
  uint16_t count = 0;
  int16_t pending = 0;
};
