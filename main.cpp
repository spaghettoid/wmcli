#include "wmcli/wmcli.hpp"

using namespace wm;

int main() {

  while (true) {

    clearScreen();
    
    printAt(1, 0, "Hello World");
    printAt(3, 5, "%i", 6);

    wait(32);
    
  }
}
