#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cmath> // Only if you want actual floor() math
#include <chrono>
#include <thread>
//
// struct Timer {
//     std::chrono::steady_clock::time_point start;
//
//     Timer() {
//         start = std::chrono::steady_clock::now();
//     }
//
//     double getSeconds() {
//         auto now = std::chrono::steady_clock::now();
//         std::chrono::duration<double> elapsed = now - start;
//         return elapsed.count();
//     }
// };
//
// struct Point {
//     double x;
//     double y;
//
//     bool dead = false;
//     // Helper methods to get grid coordinates
//     int get_x() const { return std::floor(x); }
//     int get_y() const { return std::floor(y); }
// };
//
// bool isColliding(const Point& a, const Point& b) {
//     // Compare their "floored" integer coordinates
//     return (a.get_x() == b.get_x()) && (a.get_y() == b.get_y());
// }
//
// const char KEY_UP    = 'U';
// const char KEY_DOWN  = 'D';
// const char KEY_LEFT  = 'L';
// const char KEY_RIGHT = 'R';
//
// class Game {
// private:
//     int width;
//     int height;
//     std::vector<std::vector<char>> grid;
//     struct termios original_terminal;
//     char last_active_key; // Remembers the direction even when they release the key!
//
//     // Internal player state managed entirely by the engine
//     int playerX;
//     int playerY;
//
//     void enableRawMode() {
//         struct termios raw;
//         tcgetattr(STDIN_FILENO, &original_terminal);
//         raw = original_terminal;
//         raw.c_lflag &= ~(ICANON | ECHO); 
//         tcsetattr(STDIN_FILENO, TCSANOW, &raw);
//
//         int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
//         fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
//     }
//
//     void disableRawMode() {
//         tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal);
//     }
//
// public:
//     Game(int w, int h) {
//         std::ios_base::sync_with_stdio(false);
//         std::cin.tie(NULL);
//
//         width = w;
//         height = h;
//
//         // Start player in the center
//         playerX = width / 2;
//         playerY = height / 2;
//
//         last_active_key = 0; // Stationary at start
//         grid.resize(height, std::vector<char>(width, '.'));
//         enableRawMode();
//     }
//
//     ~Game() {
//         disableRawMode();
//     }
//
//     // Handles terminal input and saves the direction immediately
//     void pollInput() {
//         char ch = 0;
//         last_active_key = 0;
//         if (read(STDIN_FILENO, &ch, 1) > 0) {
//             if (ch == '\033') { 
//                 char seq[2];
//                 if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
//                     if (seq[0] == '[') {
//                         switch (seq[1]) {
//                             case 'A': last_active_key = KEY_UP;    break;
//                             case 'B': last_active_key = KEY_DOWN;  break;
//                             case 'C': last_active_key = KEY_RIGHT; break;
//                             case 'D': last_active_key = KEY_LEFT;  break;
//                         }
//                     }
//                 }
//             } else {
//                 last_active_key = ch; // Store normal keys (like 's' for stop)
//             }
//         }
//     }
//
//
//     // Tells the child's code if the robot is currently headed in this direction
//     bool if_pressing(char c) {
//         return (last_active_key == c);
//     }
//
//     // Easy accessors for the kids to get the player's position
//     int getPlayerX() { return playerX; }
//     int getPlayerY() { return playerY; }
//
//     void clearGrid() {
//         grid.assign(height, std::vector<char>(width, '.'));
//     }
//
//     void drawChar(int x, int y, char c) {
//         if (x >= 0 && x < width && y >= 0 && y < height) {
//             grid[y][x] = c;
//         }
//     }
//
//     template <size_t N>
//     void drawSprite(int x, int y, const char* (&sprite)[N]) {
//         for (int row = 0; row < N; ++row) {
//             for (int col = 0; sprite[row][col] != '\0'; ++col) {
//                 char c = sprite[row][col];
//                 if (c != ' ') {
//                     drawChar(x + col, y + row, c);
//                 }
//             }
//         }
//     }
//
//     // Moves the player internally and flushes the screen smoothly
//     void render() {
//         // 1. Move player automatically based on the last registered direction
//         if (last_active_key == KEY_UP && playerY > 0) playerY--;
//         if (last_active_key == KEY_DOWN && playerY < height - 1) playerY++;
//         if (last_active_key == KEY_LEFT && playerX > 0) playerX--;
//         if (last_active_key == KEY_RIGHT && playerX < width - 1) playerX++;
//
//         // 2. Clear terminal screen
//         std::string frameBuffer = "\x1B[2J\x1B[H"; 
//
//         // 3. Assemble the grid frame
//         for (int y = 0; y < height; ++y) {
//             for (int x = 0; x < width; ++x) {
//                 frameBuffer += grid[y][x];
//                 frameBuffer += ' ';
//             }
//             frameBuffer += '\n';
//         }
//         std::cout << frameBuffer << std::flush;
//     }
//
//     void wait_ms(int miliseconds) {
//       usleep(miliseconds / 1000); 
//     }
// };
namespace Color {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* CYAN    = "\033[36m";
}

namespace wm
{
    // Clears the entire terminal
    inline void clearScreen()
    {
        std::cout << "\033[2J";
        std::cout.flush();
    }

    // Moves the cursor to (row, col)
    // Top-left is (1, 1)
    inline void moveCursor(int row, int col)
    {
        std::cout << "\033[" << row << ";" << col << "H";
        std::cout.flush();
    }

    // Hides the blinking cursor
    inline void hideCursor()
    {
        std::cout << "\033[?25l";
        std::cout.flush();
    }

    // Shows the cursor again
    inline void showCursor()
    {
        std::cout << "\033[?25h";
        std::cout.flush();
    }

    // Clears from the cursor to the end of the line
    inline void clearLine()
    {
        std::cout << "\033[K";
        std::cout.flush();
    }

    // inline void printAt(int row, int col, const std::string& text)
    // {
    //     moveCursor(row, col);
    //     std::cout << text;
    //     std::cout.flush();
    // }

    template<typename... Args>
    inline void printAt(int row, int col, const std::string& format, Args... args)
    {
        row += 1;
        // 1. Calculate the exact buffer size needed for the formatted string
        // We add 1 for the null-terminator
        int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; 
        if (size <= 0) return;

        // 2. Allocate a buffer and format the string into it
        std::vector<char> buf(size);
        std::snprintf(buf.data(), size, format.c_str(), args...);

        // 3. Move the cursor and print the formatted string
        moveCursor(row, col);
        std::cout << buf.data();
        std::cout.flush();
    }


    inline void wait(int milliseconds)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(milliseconds)
        );
    }

  // inline char getKey()
  //     {
  //         // Save terminal settings
  //         termios oldSettings;
  //         tcgetattr(STDIN_FILENO, &oldSettings);
  //
  //         // Create raw mode
  //         termios newSettings = oldSettings;
  //         newSettings.c_lflag &= ~(ICANON | ECHO);
  //
  //         tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
  //
  //         // Read one character
  //         char c;
  //         read(STDIN_FILENO, &c, 1);
  //
  //         // Restore terminal settings
  //         tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
  //
  //         return c;
  //     }

const char KEY_UP    = 'A';
    const char KEY_DOWN  = 'B';
    const char KEY_RIGHT = 'C';
    const char KEY_LEFT  = 'D';

    inline char getKey()
    {
        static bool initialized = false;
        if (!initialized) {
            static termios oldSettings;
            static int oldFcntlFlags;

            tcgetattr(STDIN_FILENO, &oldSettings);
            oldFcntlFlags = fcntl(STDIN_FILENO, F_GETFL, 0);

            termios newSettings = oldSettings;
            newSettings.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
            fcntl(STDIN_FILENO, F_SETFL, oldFcntlFlags | O_NONBLOCK);

            std::atexit([]() {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
                fcntl(STDIN_FILENO, F_SETFL, oldFcntlFlags);
            });

            initialized = true;
        }

        char lastKey = 0;
        char c;

        // Consume ALL available characters currently waiting in the buffer
        while (read(STDIN_FILENO, &c, 1) > 0) {
            if (c == '\033') {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
                    if (seq[0] == '[') {
                        lastKey = seq[1]; // Store the latest arrow key ('A', 'B', 'C', 'D')
                        continue;
                    }
                }
                lastKey = '\033'; 
            } else {
                lastKey = c; // Store the latest normal key
            }
        }

        // Returns ONLY the very last key pressed during this frame cycle
        return lastKey; 
    }
}
