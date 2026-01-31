#include <iostream>
#include <cmath>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/ioctl.h>

const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

const std::string BG_BLUE = "\033[44m";

void getTerminalSize(int &width, int &height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
}

void enterAltScreen() {
    std::cout << "\033[?1049h\033[2J";
}

void exitAltScreen() {
    std::cout << "\033[?1049l";
}

void moveCursorHome() {
    std::cout << "\033[H";
}

void hideCursor() {
    std::cout << "\033[?25l";
}

void showCursor() {
    std::cout << "\033[?25h";
}

void visualizeModulo(int frame, int width, int height) {
    int divisor = (frame % 8) + 3;
    std::string colors[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
    char chars[] = {'*', '+', 'o', '#', '@', '&', '%', '$', '!', '~'};
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int value = (col + frame + row) % divisor;
            std::cout << colors[value % 6] << chars[value % 10];
        }
        if (row < height - 1) std::cout << RESET << "\n";
    }
    std::cout << RESET;
}

void visualizeLogarithm(int frame, int width, int height) {
    std::string colors[] = {RED, RED, YELLOW, YELLOW, GREEN, GREEN, CYAN, CYAN, BLUE, MAGENTA};
    
    for (int row = 0; row < height; row++) {
        double logVal = (double)row / height * 5;
        int barLen = (int)(logVal / 5.0 * width);
        bool highlight = ((frame / 3) % height) == row;
        
        if (highlight) std::cout << BOLD << BG_BLUE;
        std::cout << colors[row * 10 / height];
        
        for (int col = 0; col < width; col++) {
            std::cout << (col < barLen ? "█" : "░");
        }
        
        if (highlight) std::cout << RESET;
        if (row < height - 1) std::cout << "\n";
    }
    std::cout << RESET;
}

void runVisualization(void (*vizFunc)(int, int, int)) {
    enterAltScreen();
    hideCursor();
    
    int frame = 0;
    int prevWidth = 0, prevHeight = 0;
    
    while (true) {
        int width, height;
        getTerminalSize(width, height);
        
        if (width != prevWidth || height != prevHeight) {
            std::cout << "\033[2J";
            prevWidth = width;
            prevHeight = height;
        }
        
        moveCursorHome();
        vizFunc(frame, width, height);
        std::cout.flush();
        usleep(80000);
        frame++;
    }
    
    showCursor();
    exitAltScreen();
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    
    std::string arg = argv[1];
    
    if (arg == "modulo") {
        runVisualization(visualizeModulo);
    } else if (arg == "logarithm") {
        runVisualization(visualizeLogarithm);
    } else {
        return 1;
    }
    
    return 0;
}

