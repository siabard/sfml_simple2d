#ifndef __GAME_HPP__
#define __GAME_HPP__

/*

  Game engine class

*/

#include "stdafx.h"

class Game {

private:
  // private functions
  void initVariables();
  void initWindow();
  void initEnemies();

  // private variables
  sf::RenderWindow* window;
  sf::Event ev;
  sf::VideoMode videoMode;

  // mouse positions
  sf::Vector2i mousePosWindow;
  sf::Vector2f mousePosView;

  // Gamelogic
  bool endGame;
  unsigned points;
  int health;
  float enemySpawnTimer;
  float enemySpawnTimerMax;
  int maxEnemies;
  bool mouseHeld;

  // Game objects
  std::vector<sf::RectangleShape> enemies;
  sf::RectangleShape enemy;

public:
  // Constructor & Destructor
  Game();
  virtual ~Game();

  // accessors
  const bool running() const;
  const bool getEndGame() const;

  // functions
  void spawnEnemy();

  void pollEvents();
  void updateMousePositions();
  void updateEnemies();
  void update();

  void renderEnemies();
  void render();
};


#endif
