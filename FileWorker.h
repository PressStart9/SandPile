#pragma once

#include <iostream>
#include <fstream>
#include <cstring>

#include "GrowingDequeMatrix.hpp"
#include "Cell.h"

class FileWorker {
 public:
  static void load_image(const std::string& inputFilepath, GrowingDequeMatrix<Cell>& field);
  static void save_image(const std::string& outputFilepath, GrowingDequeMatrix<Cell>& field);

  constexpr static uint8_t kColors[20] = {0xFF, 0xFF, 0xFF, 0x00,  // white
                                          0x00, 0xFF, 0x00, 0x00,  // green
                                          0x00, 0xFF, 0xFF, 0x00,  // yellow
                                          0x80, 0x00, 0x80, 0x00,  // purple
                                          0x00, 0x00, 0x00, 0x00, };  // black
 private:
  static uint64_t get_tab_separated_number(char* str, size_t& i);

  static const uint16_t kMaxLineLength = 50;
};


#pragma pack(push, 1)
struct BITMAPINFOHEADER {  // 56
  uint32_t biSize = sizeof(BITMAPINFOHEADER);  // size of this struct
  uint32_t biWidth;
  uint32_t biHeight;
  uint16_t biPlanes = 1;  // count of planes always set to 1
  uint16_t biBitCount = 4;  // bit per pixel
  uint32_t biCompression = 0;  // no compression
  uint32_t biSizeImage;
  uint32_t biXPelsPerMeter = 0;  // not defined
  uint32_t biYPelsPerMeter = 0;  // not defined
  uint32_t biClrUsed = 5;  // we use 5 colors
  uint32_t biClrImportant = 0;  // all colors are important
};

struct BITMAPFILEHEADER  {  // 14
  uint16_t bfType = 0x4D42;  // BM
  uint32_t bfSize;
  uint16_t bfReserved1 = 0;  // reserved
  uint16_t bfReserved2 = 0;  // reserved
  uint32_t bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(FileWorker::kColors);  // offset to pixels (with palette)
};
#pragma pack(pop)
