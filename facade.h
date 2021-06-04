#include "game.h"

class Facade {
public:
    void start();
    Facade() { black = true; game = new Game();}
    void changeColor(bool value);
    bool getColor();
    ~Facade() { delete game; }
private:
    Game* game;
    bool black;
    void initialize();
    void readIrrelevantLines(int noLines);
    int newGame();
    int pauseGame();
    void printInput(char *input, FILE *outputFile); // pt debug
};