#include "wmcli/wmcli.hpp"

using namespace wm;

int main() {

  while (true) {

    clearScreen();
    
    printAt(1, 0, "Hello World");

    wait(32);
    
  }
}
