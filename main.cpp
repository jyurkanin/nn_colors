#include <ncurses.h>
#include "engine.hpp"

int main(){
    int input[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int output[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    Engine engine;
    engine.start(input, output, sizeof(input) / sizeof(input[0]));
    
    
}
