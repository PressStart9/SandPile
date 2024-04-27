#include <string>

#include "FileWorker.h"

struct Node {
  Node* prev = nullptr;
  Cell* cell = nullptr;
};

void Life(const std::string& outputFilepath, Cell* start, uint32_t iterations, uint32_t frequency) {
  Node* update = new Node();
  Cell* row = start;
  Cell* el;
  while (row != nullptr) {
    el = row;
    while (el != nullptr) {
      if (el->count > 3) {
        update = new Node { update, el };
      }
      el = el->right;
    }
    row = row->down;
  }

  Node* temp;
  for (uint32_t i = 0; i < iterations; ++i) {
    if (update->prev == nullptr) {
      break;
    }

    Node* new_update = new Node();
    while (update->prev != nullptr) {
      uint16_t n = update->cell->count / 4;

      if (update->cell->Down()->count < 4 && 3 < update->cell->Down()->count + n) {
        new_update = new Node { new_update, update->cell->Down() };
      }
      if (update->cell->Left()->count < 4 && 3 < update->cell->Left()->count + n) {
        new_update = new Node { new_update, update->cell->Left() };
      }
      if (update->cell->Right()->count < 4 && 3 < update->cell->Right()->count + n) {
        new_update = new Node { new_update, update->cell->Right() };
      }
      if (update->cell->Up()->count < 4 && 3 < update->cell->Up()->count + n) {
        new_update = new Node { new_update, update->cell->Up() };
      }

      update->cell->Down()->count += n;
      update->cell->Left()->count += n;
      update->cell->Right()->count += n;
      update->cell->Up()->count += n;

      update->cell->count %= 4;

      temp = update;
      update = update->prev;
      delete temp;
    }
    temp = update;
    update = new_update;
    delete temp;

    if (i % frequency == 0) {
      while (start->up != nullptr) {
        start = start->up;
      }
      while (start->left != nullptr) {
        start = start->left;
      }

      FileWorker::Writing(outputFilepath + std::to_string(i) + ".bmp", start);
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
        RaiseError(ErrorCodes::NO_SUCH_ARGUMENT, curr_arg.c_str());
        return;
      }
    } else {
      RaiseError(ErrorCodes::ARGUMENT_NOT_SPECIFIED, curr_arg.c_str());
      return;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    RaiseError(ErrorCodes::FILE_NOT_SPECIFIED);
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

  FileWorker::Reading(inputFilepath, start);
  FileWorker::Writing(outputFilepath + "test.bmp", start);

  Life(outputFilepath, start, iterations, frequency);

  return 0;
}
