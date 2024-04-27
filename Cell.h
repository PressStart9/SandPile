#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct Cell {
 public:
    int x;
    int y;
    uint16_t count = 0;

    Cell* Up();
    Cell* Right();
    Cell* Down();
    Cell* Left();

    Cell* up = nullptr;
    Cell* right = nullptr;
    Cell* down = nullptr;
    Cell* left = nullptr;
};
#pragma pack(pop)
