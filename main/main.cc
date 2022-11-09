#include <iostream>
#include <vector>

#include "absl/strings/str_join.h"

int main(int argc, char* argv[]) {
  std::vector<std::string> parts = {"Hello,", "world."};
  std::string greeting = absl::StrJoin(parts, " ");
  std::cout << greeting << std::endl;

  return 0;
}
