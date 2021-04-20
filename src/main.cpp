#include "lib.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

// using ip_address = std::map<>

using file_text = std::vector<std::string>;

bool readFile(const std::string &filename, file_text &text)
{
    try {
        std::string line;
        std::ifstream myfile (filename.c_str());

        if (!myfile.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;

            return false;
        }

        while ( getline (myfile,line) ) {
            text.push_back(line);
        }

        myfile.close();
    }
    catch(const std::exception &e) {
        std::cout << "Error read file: " << filename << std::endl;

        return false;
    }

    return true;
}

void showFile(file_text &text)
{
  std::cout << "---" << std::endl;

  for (const auto &line : text) {
    std::cout << line << std::endl;
  }

  std::cout << "---" << std::endl;
}

int main (int, char **)
{
    std::cout << "ip_filter Version: " << version() << std::endl;

    if (!readFile(const std::string &filename, file_text &text)) {
      return 1;
    }

    return 0;
}
