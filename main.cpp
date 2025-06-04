#include <string>

#include "FileWorker.h"

void Life(const std::string& outputFilepath, Cell* start, uint32_t iterations, uint32_t frequency) {
  Cell* bg_buffer = new Cell(*start);
  for (uint32_t i = 0; i < iterations; ++i) {
    Cell* current_row = start;
    while (current_row != nullptr) {
      Cell* current = current_row;
      while (current != nullptr) {
        current->count += current->pending;
        current->pending = 0;
        current = current->right;
      }
      current_row = current_row->down;
    }

    current_row = start;
    while (current_row != nullptr) {
      Cell* current = current_row;
      while (current != nullptr) {
        if (current->count < 4) {
          current = current->right;
          continue;
        }

        ++current->Down()->pending;
        ++current->Left()->pending;
        ++current->Right()->pending;
        ++current->Up()->pending;

        current->pending -= 4;
        current = current->right;
      }
      current_row = current_row->down;
    }

    if (start->up != nullptr) {
      start = start->up;
    }
    if (start->left != nullptr) {
      start = start->left;
    }

    if (i % frequency == 0) {
      FileWorker::save_image(outputFilepath + std::to_string(i) + ".bmp", start);
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

  Cell* start = new Cell();
  start->x = 0;
  start->y = 0;

  FileWorker::load_image(inputFilepath, start);
  FileWorker::save_image(outputFilepath + "test.bmp", start);

  Life(outputFilepath, start, iterations, frequency);

  return 0;
}
