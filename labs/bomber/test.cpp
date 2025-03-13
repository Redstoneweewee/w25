#include <fstream>
#include <iostream>

#include "Errors.h"
#include "Map.h"

int main(int argc, char** argv) {
  const char* filename;

  if(argc == 3 && std::string("-i") == argv[1]) {
    filename = argv[2];
  }
  else if(argc == 2) {
    filename = argv[1];
  }
  else {
    std::cerr << "USAGE: " << argv[0] << "[-i] map-file.txt\n";
    return 1;
  }

  std::ifstream stream(filename);
  if(stream.fail()) {
    std::cerr << "ERROR: Could not open file: " << filename << '\n';
    return 1;
  }

  Map map(stream);

  map.printMap();
  map.printPerimeter();

  return 0;
}
