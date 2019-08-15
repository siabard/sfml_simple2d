#include "include/stdafx.h"
#include "include/Game.hpp"

int main () {

  // Init game engine
  srand(static_cast<unsigned>(time(NULL)));

  Game game;

  // Game loop
  while(game.running()) {

    // update
    game.update();

    // render
    game.render();

  }

  // End of application
  return 0;
}
