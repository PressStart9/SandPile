#include <string>

#include "FileWorker.h"

uint64_t FileWorker::get_tab_separated_number(char* str, size_t& i) {
  uint16_t j = i;
  while (str[i] != '\t' && str[i] != '\0') {
    ++i;
  }
  str[i] = '\0';
  ++i;
  return std::stoull(&str[j]);
}

void FileWorker::load_image(const std::string& inputFilepath, GrowingDequeMatrix<Cell>& field) {
  std::ifstream file(inputFilepath, std::ifstream::binary);
  if (!file.is_open()) {
    throw std::runtime_error("File not found: " + inputFilepath);
  }

  char* line = new char[FileWorker::kMaxLineLength];  // 21 + 1 + 21 + 1 + 6 = 50
  file.getline(line, FileWorker::kMaxLineLength);
  while (file.getline(line, FileWorker::kMaxLineLength)) {
    size_t i = 0;
    size_t x = get_tab_separated_number(line, i);
    size_t y = get_tab_separated_number(line, i);
    size_t count = get_tab_separated_number(line, i);

    while (field.size_x() <= x) {
      field.extend_x_back();
    }
    while (field.size_y() <= y) {
      field.extend_y_back();
    }

    field[x][y].count = count;
  }

  delete[] line;
  file.close();
}

void FileWorker::save_image(const std::string& outputFilepath, GrowingDequeMatrix<Cell>& field) {
  std::ofstream file(outputFilepath, std::ios::binary | std::ios::out);

  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info_header;

  bmp_info_header.biWidth = field.size_x();
  bmp_info_header.biHeight = field.size_y();

  bmp_info_header.biSizeImage = ((bmp_info_header.biWidth + 1) / 2 + 3) / 4 * 4 * bmp_info_header.biHeight;
  bmp_header.bfSize = bmp_header.bfOffBits + bmp_info_header.biSizeImage;
  auto* pixels = new uint8_t[bmp_info_header.biSizeImage] { 0 };

  for (uint32_t i = 0; i < bmp_info_header.biHeight; ++i) {
    for (uint32_t j = 0; j < bmp_info_header.biWidth; ++j) {
      uint64_t ind = i * (((bmp_info_header.biWidth + 1) / 2 + 3) / 4 * 4) + j / 2;
      pixels[ind] |= std::min(field[j][i].count, (uint16_t)4) << ((j + 1) % 2) * 4;
    }
  }

  bmp_info_header.biHeight = -bmp_info_header.biHeight;

  file.write(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
  file.write(reinterpret_cast<char*>(&bmp_info_header), sizeof(bmp_info_header));
  file.write(const_cast<char*>(reinterpret_cast<const char*>(kColors)), sizeof(FileWorker::kColors));
  file.write(reinterpret_cast<char*>(pixels), bmp_info_header.biSizeImage);

  file.close();

  delete[] pixels;
}
