#include <iostream>
#include <cmath>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include <ctime>

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

const char* DIGITS[10][9] = {
    {"#########", "###   ###", "###   ###", "###   ###", "###   ###", "###   ###", "###   ###", "###   ###", "#########"},
    {"    ###  ", "   ####  ", "    ###  ", "    ###  ", "    ###  ", "    ###  ", "    ###  ", "    ###  ", "#########"},
    {"#########", "      ###", "      ###", "#########", "###      ", "###      ", "###      ", "###      ", "#########"},
    {"#########", "      ###", "      ###", "#########", "      ###", "      ###", "      ###", "      ###", "#########"},
    {"###   ###", "###   ###", "###   ###", "#########", "      ###", "      ###", "      ###", "      ###", "      ###"},
    {"#########", "###      ", "###      ", "#########", "      ###", "      ###", "      ###", "      ###", "#########"},
    {"#########", "###      ", "###      ", "#########", "###   ###", "###   ###", "###   ###", "###   ###", "#########"},
    {"#########", "      ###", "     ### ", "    ###  ", "   ###   ", "  ###    ", " ###     ", "###      ", "###      "},
    {"#########", "###   ###", "###   ###", "#########", "###   ###", "###   ###", "###   ###", "###   ###", "#########"},
    {"#########", "###   ###", "###   ###", "#########", "      ###", "      ###", "      ###", "      ###", "#########"}
};

const char* COLON[9] = {"         ", "   ###   ", "   ###   ", "   ###   ", "         ", "   ###   ", "   ###   ", "   ###   ", "         "};

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

void visualizeSpiral(int frame, int width, int height) {
    std::string colors[] = {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA};
    char symbols[] = {'*', '+', '@', '#', '%', '&', '=', '~', '^', 'o'};
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dx = x - width / 2.0;
            double dy = (y - height / 2.0) * 2;
            double dist = sqrt(dx * dx + dy * dy);
            if (dist < 1) dist = 1;
            
            double logDist = log(dist + 1);
            int pattern = (int)(logDist * 5 + frame * 0.5) % 10;
            int colorIdx = (int)(logDist * 2 + frame * 0.3) % 6;
            double angle = atan2(dy, dx);
            int spiral = (int)((angle + M_PI) * 5 + logDist * 3 + frame * 0.2) % 10;
            
            std::cout << colors[colorIdx] << symbols[(pattern + spiral) % 10];
        }
        if (y < height - 1) std::cout << RESET << "\n";
    }
    std::cout << RESET;
}

void visualizeFibonacciModulo(int frame, int width, int height) {
    long long* fib = new long long[width];
    fib[0] = 0;
    fib[1] = 1;
    
    for (int i = 2; i < width; i++) {
        fib[i] = (fib[i-1] + fib[i-2]) % height;
    }
    
    std::string colors[] = {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE};
    
    for (int row = height - 1; row >= 0; row--) {
        for (int i = 0; i < width; i++) {
            int val = fib[i] % height;
            if (val == row) {
                bool highlight = (i == (frame % width));
                std::cout << (highlight ? BOLD + YELLOW + "◆" : colors[val % 7] + "●") << RESET;
            } else {
                std::cout << DIM << "·" << RESET;
            }
        }
        if (row > 0) std::cout << "\n";
    }
    std::cout << RESET;
    delete[] fib;
}

void visualizeWaveInterference(int frame, int width, int height) {
    char intensityChars[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    std::string colors[] = {DIM + WHITE, WHITE, CYAN, BLUE, MAGENTA, RED};
    
    double source1x = width * 0.2;
    double source2x = width * 0.8;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dx1 = x - source1x;
            double dy1 = y - height / 2.0;
            double dist1 = sqrt(dx1 * dx1 + dy1 * dy1) + 0.1;
            
            double dx2 = x - source2x;
            double dy2 = y - height / 2.0;
            double dist2 = sqrt(dx2 * dx2 + dy2 * dy2) + 0.1;
            
            double wave1 = sin(log(dist1) * 5 + frame * 0.3);
            double wave2 = sin(log(dist2) * 5 + frame * 0.3);
            double combined = (wave1 + wave2) / 2.0;
            
            int intensity = (int)((combined + 1) * 4.5);
            if (intensity < 0) intensity = 0;
            if (intensity > 9) intensity = 9;
            
            std::cout << colors[intensity * 5 / 10] << intensityChars[intensity];
        }
        if (y < height - 1) std::cout << RESET << "\n";
    }
    std::cout << RESET;
}

void visualizeMatrix(int frame, int width, int height) {
    std::string greens[] = {DIM + GREEN, GREEN, BOLD + GREEN, BOLD + WHITE};
    char matrixChars[] = {'0', '1', '@', '#', '$', '%', '&', '*', '=', '+'};
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double speed = log(((x * 7 + 13) % 17) + 2) * 2;
            int dropPos = (int)(frame * speed + x * 31) % (height * 3);
            int distFromHead = dropPos - y;
            
            if (distFromHead >= 0 && distFromHead < 15) {
                int colorIdx = distFromHead % 4;
                int charIdx = (x + y + frame) % 10;
                std::cout << greens[3 - colorIdx] << matrixChars[charIdx];
            } else {
                std::cout << DIM << GREEN << ".";
            }
        }
        if (y < height - 1) std::cout << RESET << "\n";
    }
    std::cout << RESET;
}

void visualizePlasma(int frame, int width, int height) {
    std::string colors[] = {RED, MAGENTA, BLUE, CYAN, GREEN, YELLOW};
    char plasmaChars[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double v1 = sin(x * 0.1 + frame * 0.05);
            double v2 = sin(y * 0.1 + frame * 0.03);
            
            double dx = x - width / 2.0;
            double dy = y - height / 2.0;
            double v3 = sin(log(sqrt(dx * dx + dy * dy) + 1) * 2 + frame * 0.04);
            
            double v4 = sin(log(((x * y) % 100) + 2) + frame * 0.06);
            
            double combined = (v1 + v2 + v3 + v4) / 4.0;
            int colorIdx = (int)((combined + 1) * 3) % 6;
            int charIdx = (int)((combined + 1) * 4.5) % 10;
            
            std::cout << colors[colorIdx] << plasmaChars[charIdx];
        }
        if (y < height - 1) std::cout << RESET << "\n";
    }
    std::cout << RESET;
}

void visualizeClock(int frame, int width, int height) {
    std::string colors[] = {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA};
    
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    int h = t->tm_hour;
    int m = t->tm_min;
    int s = t->tm_sec;
    
    int digitWidth = 9;
    int digitHeight = 9;
    int spacing = 1;
    int colonWidth = 9;
    int clockWidth = 6 * (digitWidth + spacing) + 2 * colonWidth;
    int startX = (width - clockWidth) / 2;
    int startY = (height - digitHeight) / 2;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dist = sqrt((x - width/2.0) * (x - width/2.0) + 
                              (y - height/2.0) * (y - height/2.0));
            double logDist = log(dist + 1);
            int pattern = (int)(logDist * 2 + frame * 0.1 + s * 0.5) % 6;
            
            int relX = x - startX;
            int relY = y - startY;
            
            bool inClockArea = (relY >= 0 && relY < digitHeight && relX >= 0 && relX < clockWidth);
            
            if (inClockArea) {
                int pos = 0;
                int digit = -1;
                bool isColon = false;
                int charInDigit = -1;
                
                if (relX < digitWidth) { digit = h / 10; charInDigit = relX; }
                pos = digitWidth + spacing;
                if (relX >= pos && relX < pos + digitWidth) { digit = h % 10; charInDigit = relX - pos; }
                pos += digitWidth + spacing;
                if (relX >= pos && relX < pos + colonWidth) { isColon = true; charInDigit = relX - pos; }
                pos += colonWidth;
                if (relX >= pos && relX < pos + digitWidth) { digit = m / 10; charInDigit = relX - pos; }
                pos += digitWidth + spacing;
                if (relX >= pos && relX < pos + digitWidth) { digit = m % 10; charInDigit = relX - pos; }
                pos += digitWidth + spacing;
                if (relX >= pos && relX < pos + colonWidth) { isColon = true; charInDigit = relX - pos; }
                pos += colonWidth;
                if (relX >= pos && relX < pos + digitWidth) { digit = s / 10; charInDigit = relX - pos; }
                pos += digitWidth + spacing;
                if (relX >= pos && relX < pos + digitWidth) { digit = s % 10; charInDigit = relX - pos; }
                
                char ch = ' ';
                if (isColon && charInDigit >= 0 && charInDigit < colonWidth) {
                    ch = COLON[relY][charInDigit];
                } else if (digit >= 0 && charInDigit >= 0 && charInDigit < digitWidth) {
                    ch = DIGITS[digit][relY][charInDigit];
                }
                
                if (ch == '#') {
                    std::cout << BOLD << WHITE << "#";
                } else {
                    std::cout << DIM << colors[pattern] << ".";
                }
            } else {
                char bgChars[] = {' ', '.', ':', '+', '*'};
                int charIdx = (int)(logDist + frame * 0.05) % 5;
                std::cout << DIM << colors[pattern] << bgChars[charIdx];
            }
        }
        if (y < height - 1) std::cout << RESET << "\n";
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
        std::this_thread::sleep_for(std::chrono::microseconds(16666));
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
    } else if (arg == "spiral") {
        runVisualization(visualizeSpiral);
    } else if (arg == "fibonacci") {
        runVisualization(visualizeFibonacciModulo);
    } else if (arg == "wave") {
        runVisualization(visualizeWaveInterference);
    } else if (arg == "matrix") {
        runVisualization(visualizeMatrix);
    } else if (arg == "plasma") {
        runVisualization(visualizePlasma);
    } else if (arg == "clock") {
        runVisualization(visualizeClock);
    } else {
        return 1;
    }
    
    return 0;
}

