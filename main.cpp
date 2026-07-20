#include "wmcli/wmcli.hpp"
#include <string>

using namespace wm;

int main() {
  double cookies = 0;
  int clickers = 0;
  int farms = 0;

  while (true) {
    clearScreen();

    char input = getKey();
    if (input == KEY_UP) {
      cookies++;
    } else if (input == 'a') {
      if (cookies >= 50) {
        cookies -= 50;
        clickers++;
      }
    } else if (input == 'b') {
      if (cookies >= 500) {
        cookies -= 500;
        farms++;
      }
    }

    for (int i = 0; i < clickers; i++) {
      cookies += 0.1;
    }

    for (int i = 0; i < farms; i++) {
      cookies += 2;
    }

    wm::printAt(1, 3, "cookies: %f", cookies);
    wm::printAt(2, 3, "press a to buy clickers for 50$, count: %i", clickers);
    wm::printAt(3, 4, "press b to buy farm for 500$, count: %i", farms);

    wait(100);
  }
}
