#include "include/stdafx.h"
#include "include/Game.hpp"

// private functions
void Game::initVariables() {
  this->window = nullptr;

  // Game Logic
  this->endGame =  false;
  this->points = 0;
  this->health = 10;
  this->enemySpawnTimerMax = 10.f;
  this->enemySpawnTimer = this->enemySpawnTimerMax;
  this->maxEnemies = 10;
  this->mouseHeld = false;

}

void Game::initWindow() {
  this->videoMode.height = 600;
  this->videoMode.width = 800;

  this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

  this->window->setFramerateLimit(60);
}

void Game::initEnemies() {

  this->enemy.setPosition(10.f, 10.f);
  this->enemy.setSize(sf::Vector2f(100.f, 100.f));
  this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
  this->enemy.setFillColor(sf::Color::Cyan);
  //this->enemy.setOutlineColor(sf::Color::Green);
  //this->enemy.setOutlineThickness(1.);


}

// Constructor & Destructor
Game::Game() {
  this->initVariables();
  this->initWindow();
  this->initEnemies();
}

Game::~Game() {
  if(!this->window)
    delete this->window;
}

// accessors
const bool Game::running() const {
  return this->window->isOpen();
}

const bool Game::getEndGame() const {
  return this->endGame;
}
// functions

/**
 * Spawns enemies and sets their color and positions.
 * - set a random position
 * - set a random color
 * - add enemy to the vector
 *
 * @return
 */
void Game::spawnEnemy() {
  this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
                          0.f
                          );

  this->enemy.setFillColor(sf::Color::Green);

  // Spawn the enemy
  this->enemies.push_back(this->enemy);
}

void Game::pollEvents() {
  while(this->window->pollEvent(this->ev)) {
    switch(this->ev.type) {
    case sf::Event::Closed:
      this->window->close();
      break;
    case sf::Event::KeyPressed:
      if (this->ev.key.code == sf::Keyboard::Escape)
        this->window->close();
      break;
    default:
      break;
    }
  }
}

/**
 * Updates enemy spawn timer and spawns enemies
 * when the total amount of enemis is smaller than the limit
 * Move enemies
 * Remove the enemies at the edge of the screen.
 *
 * @return
 */
void Game::updateEnemies() {
  // updating the timer for enemy spawning.
  if(this->enemies.size() < this->maxEnemies) {

    if(this->enemySpawnTimer >= this->enemySpawnTimerMax) {
      this->spawnEnemy();
      this->enemySpawnTimer = 0.f;
    }
    else
      this->enemySpawnTimer += 5.f;
  }

  for(int i = 0; i < this->enemies.size(); i++) {
    this->enemies[i].move(0.f, 1.f);

    // check enemy move pass the bottom.
    if(this->enemies[i].getPosition().y > this->window->getSize().y) {
      this->enemies.erase(this->enemies.begin() + i);
      this->health -= 1;
    }

  }


  // Check if clicked upon
  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if(this->mouseHeld == false) {
      this->mouseHeld = true;
      bool deleted = false;

      for(int i = 0; i < this->enemies.size() && deleted == false; i++) {
        if(this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
          deleted = true;
          this->enemies.erase(this->enemies.begin() + i);

          this->points += 1.f;
        }
      }
    }
  } else {
    this->mouseHeld = false;
  }
}

/**
 *
 * Update the mouse positions:
 * Mouse position relative to window( sf::Vector2i )
 *
 * @ return
 */

void Game::updateMousePositions() {
  this->mousePosWindow = sf::Mouse::getPosition(*this->window);
  this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::update() {

  this->pollEvents();

  if(this->endGame == false) {
    this->updateMousePositions();
    this->updateEnemies();
  }

  // End Game Condition
  if(this->health <= 0) {
    this->endGame = true;
  }

}

void Game::renderEnemies() {

  // rendering all enemies
  for(auto &e: this->enemies) {
    this->window->draw(e);
  }
}

void Game::render() {

  /*
    Render game objects

    - clear old frame
    - render objects
    - display frame window
  */

  this->window->clear();

  // Draw objects
  this->renderEnemies();

  this->window->display();

}
