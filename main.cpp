#include <string>

#include "Cell.h"
#include "FileWorker.h"
#include "GrowingDeque.hpp"
#include "GrowingDequeMatrix.hpp"


bool ComputePending(GrowingDequeMatrix<Cell>& field) {
  bool has_changes = false;
  for (size_t i = 0; i < field.size_x(); ++i) {
    for (size_t j = 0; j < field.size_y(); ++j) {
      if (field[i][j].pending == 0) {
        continue;
      }
      has_changes = true;
      field[i][j].count = static_cast<int16_t>(field[i][j].count) + field[i][j].pending;
      field[i][j].pending = 0;
    }
  }
  return has_changes;
}

void Step(GrowingDequeMatrix<Cell>& field) {
  size_t offset_x = 0;
  size_t offset_y = 0;
  for (size_t i = 0; i < field.size_x(); ++i) {
    for (size_t j = 0; j < field.size_y(); ++j) {
      size_t x = offset_x + i;
      size_t y = offset_y + j;
      if (field[x][y].count < 4) {
        continue;
      }

      field[x][y].pending -= 4;
      if (i == field.size_x() - 1) {
        field.extend_x_back();
      }
      if (i == 0 && offset_x == 0) {
        ++offset_x;
        ++x;
        field.extend_x_front();
      }
      if (j == field.size_y() - 1) {
        field.extend_y_back();
      }
      if (j == 0 && offset_y == 0) {
        ++offset_y;
        ++y;
        field.extend_y_front();
      }

      ++field[x][y + 1].pending;
      ++field[x][y - 1].pending;
      ++field[x + 1][y].pending;
      ++field[x - 1][y].pending;
    }
  }
}

void Life(const std::string& outputFilepath, GrowingDequeMatrix<Cell>& field, uint32_t iterations, uint32_t frequency) {
  for (uint32_t i = 0; i < iterations; ++i) {
    Step(field);
    bool has_changes = ComputePending(field);

    if (i % frequency == 0 || !has_changes) {
      FileWorker::save_image(outputFilepath + std::to_string(i) + ".bmp", field);

      // for (size_t a = 0; a < field.size_x(); ++a) {
      //   for (size_t b = 0; b < field.size_y(); ++b) {
      //     std::cout << field[a][b].count << ' ';
      //   } std::cout << '\n';
      // } std::cout << "\n\n";
      if (!has_changes) {
        break;
      }
    }
  }
}

void ParseCommandArguments(const int argc, char* argv[], uint32_t& iterations, uint32_t& frequency, std::string& inputFilepath, std::string& outputFilepath) {
  for (int i = 1; i < argc; ++i) {
    std::string curr_arg = argv[i];
    if (curr_arg.size() > 1 && curr_arg[0] == '-') {
      if (curr_arg == "-m") {
        ++i;
        iterations = static_cast<uint32_t>(std::stoul(argv[i]));
      } else if (curr_arg == "-f") {
        ++i;
        frequency = static_cast<uint32_t>(std::stoul(argv[i]));
      } else if (curr_arg == "-i") {
        ++i;
        inputFilepath = argv[i];
      } else if (curr_arg == "-o") {
        ++i;
        outputFilepath = argv[i];
      } else if (curr_arg.starts_with("--max-iter=")) {
        iterations = std::stoul(curr_arg.substr(curr_arg.find('=') + 1));
      } else if (curr_arg.starts_with("--freq=")) {
        frequency = std::stoul(curr_arg.substr(curr_arg.find('=') + 1));
      } else if (curr_arg.starts_with("--input=")) {
        inputFilepath = curr_arg.substr(curr_arg.find('=') + 1);
      } else if (curr_arg.starts_with("--output=")) {
        outputFilepath = curr_arg.substr(curr_arg.find('=') + 1);
      } else {
        throw std::runtime_error("No such argument: " + curr_arg);
        return;
      }
    } else {
      throw std::runtime_error("Argument not specified: " + curr_arg);
      return;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    throw std::runtime_error("File not specified.");
    return 1;
  }

  uint32_t iterations = INT_MAX;  // Max value
  uint32_t frequency = 1;  // Max value
  std::string inputFilepath;
  std::string outputFilepath;

  ParseCommandArguments(argc, argv, iterations, frequency, inputFilepath, outputFilepath);

  GrowingDequeMatrix<Cell> field;

  FileWorker::load_image(inputFilepath, field);
  FileWorker::save_image(outputFilepath + "test.bmp", field);

  Life(outputFilepath, field, iterations, frequency);

  return 0;
}
