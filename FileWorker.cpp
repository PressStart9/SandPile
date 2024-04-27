#include <string>

#include "FileWorker.h"

uint64_t FileWorker::GetNumber(char* str, uint16_t* i) {
  uint16_t j = *i;
  while (str[*i] != '\t' && str[*i] != '\0') {
    ++(*i);
  }
  str[*i] = '\0';
  ++(*i);
  return std::stoull(&str[j]);
}

// Read each symbol in file
void FileWorker::Reading(const std::string& inputFilepath, Cell* start) {
  // Open file in "in" and "binary" mode
  std::ifstream file(inputFilepath, std::ifstream::binary);
  if (!file) {
    RaiseError(ErrorCodes::NO_SUCH_FILE, inputFilepath.c_str());
  }
  uint64_t px = 0;
  uint64_t py = 0;
  Cell* current_cell = start;

  uint64_t x, y;
  uint16_t count, i;
  char* line = new char[FileWorker::kMaxLineLength];  // 21 + 1 + 21 + 1 + 6 = 50
  file.getline(line, FileWorker::kMaxLineLength);
  while (file.getline(line, FileWorker::kMaxLineLength)) {
    i = 0;
    x = GetNumber(line, &i);
    y = GetNumber(line, &i);
    count = GetNumber(line, &i);

    for (uint64_t j = x; j < px; ++j) {
      current_cell = current_cell->Left();
    }
    for (uint64_t j = px; j < x; ++j) {
      current_cell = current_cell->Right();
    }
    for (uint64_t j = y; j < py; ++j) {
      current_cell = current_cell->Up();
    }
    for (uint64_t j = py; j < y; ++j) {
      current_cell = current_cell->Down();
    }

    px = x;
    py = y;

    current_cell->count = count;
  }

  delete[] line;
  file.close();
}

void FileWorker::Writing(const std::string& outputFilepath, Cell *start) {
  std::ofstream file(outputFilepath, std::ios::binary | std::ios::out);

  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info_header;

  Cell* next = start;
  bmp_info_header.biWidth = 1;
  while (next->right != nullptr) {
    next = next->right;
    ++bmp_info_header.biWidth;
  }
  bmp_info_header.biHeight = 1;
  while (next->down != nullptr) {
    next = next->down;
    ++bmp_info_header.biHeight;
  }

  bmp_info_header.biSizeImage = ((bmp_info_header.biWidth + 1) / 2 + 3) / 4 * 4 * bmp_info_header.biHeight;
  bmp_header.bfSize = bmp_header.bfOffBits + bmp_info_header.biSizeImage;
  auto* pixels = new uint8_t[bmp_info_header.biSizeImage] { 0 };

  next = start;
  for (uint32_t i = 0; i < bmp_info_header.biHeight; ++i) {
    for (uint32_t j = 0; j < bmp_info_header.biWidth; ++j) {
      uint64_t ind = i * (((bmp_info_header.biWidth + 1) / 2 + 3) / 4 * 4) + j / 2;
      pixels[ind] |= std::min(next->count, (uint16_t)4) << ((j + 1) % 2) * 4;
      next = next->right;
    }
    start = start->down;
    next = start;
  }

  bmp_info_header.biHeight = -bmp_info_header.biHeight;

  file.write(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
  file.write(reinterpret_cast<char*>(&bmp_info_header), sizeof(bmp_info_header));
  file.write(const_cast<char*>(reinterpret_cast<const char*>(kColors)), sizeof(FileWorker::kColors));
  file.write(reinterpret_cast<char*>(pixels), bmp_info_header.biSizeImage);

  file.close();

  delete[] pixels;
}
