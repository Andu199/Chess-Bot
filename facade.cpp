#include <string.h>
#include "facade.h"

using namespace std;

void Facade::changeColor(bool value) { black = value; }

bool Facade::getColor() { return black; }

void Facade::start() {
    setbuf(stdin, NULL);
	setbuf(stdout, NULL);

    initialize();
    int result = 1;
    while(result) {
        result = newGame();
        delete game;
        game = new Game();
        changeColor(true);
    }
}

void Facade::initialize() {
    readIrrelevantLines(3);
    cout << "feature sigint=0 san=0 name=Hello\n";
	readIrrelevantLines(7);
}

int Facade::newGame() {
    readIrrelevantLines(7);
    char *input = (char *) calloc(1000, sizeof(char));
    cin >> input;

    while(strcmp(input, "quit") != 0) {
        if(!strcmp(input, "force")) {
            if(pauseGame()) {
                free(input);
                return 1;
            }
            else
                cout << game->getNextMove(!black) << endl;
        }
        if(!strcmp(input, "time")) {
            readIrrelevantLines(3);
            cin >> input;
            game->updateBoardForEnemy(input, black);
            cout << game->getNextMove(!black) << endl;
        }
        else if(!strcmp(input, "white") || !strcmp(input, "black")) {
            changeColor(!getColor());
            readIrrelevantLines(6);
            cout << game->getNextMove(!black) << endl;
        }
        cin >> input;
    }
    free(input);
    return 0;
}

int Facade::pauseGame() {
    char *input = (char *) calloc(1000, sizeof(char));
    cin >> input; 
    while(strcmp(input, "go") && strcmp(input, "new") && strcmp(input, "quit")) {
        if(!strcmp(input, "white")) {
            changeColor(true);
            readIrrelevantLines(5);
        }
        else if (!strcmp(input, "black")) {
            changeColor(false);
            readIrrelevantLines(5);
        }
        if(game->updateBoardForEnemy(input, black) == 1)
            black = !black;
        cin >> input;
    }
    if(!strcmp(input, "quit")) {
        free(input);
        exit(0);
    }
    if(!strcmp(input, "go")) {
        free(input);
        return 0;
    }
    free(input);
    return 1;
}

void Facade::readIrrelevantLines(int noLines) {
    char *input = (char *) calloc(1000, sizeof(char));
    for(int i = 0; i < noLines; ++i)
        cin >> input;
    free(input);
}