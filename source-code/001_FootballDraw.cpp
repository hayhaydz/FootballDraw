#include <iostream>

#include "GameManager.h"

int main() {
    GameManager GameManager;

    // setting seed
    srand(time(NULL));
    // calling game manager setup
    GameManager.setup();

    return 0;
}