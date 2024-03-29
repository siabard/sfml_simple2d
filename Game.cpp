#include "include/stdafx.h"
#include "include/Game.hpp"

// private functions
void Game::initVariables() {
  this->window = nullptr;

  // Game Logic
  this->endGame =  false;
  this->points = 0;
  this->health = 20;
  this->enemySpawnTimerMax = 20.f;
  this->enemySpawnTimer = this->enemySpawnTimerMax;
  this->maxEnemies = 5;
  this->mouseHeld = false;

}

void Game::initWindow() {
  this->videoMode.height = 600;
  this->videoMode.width = 800;

  this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

  this->window->setFramerateLimit(60);
}

void Game::initFonts() {
  if(!this->font.loadFromFile("fonts/DroidSans.ttf")) {
    std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << std::endl;
  }
}

void Game::initText() {
  this->uiText.setFont(this->font);
  this->uiText.setCharacterSize(12);
  this->uiText.setFillColor(sf::Color::White);
  this->uiText.setString("NONE");
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
  this->initFonts();
  this->initText();
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

  // Randomize enemy type
  int type = rand() % 5;

  switch(type) {
  case 0:
    this->enemy.setFillColor(sf::Color::Magenta);
    this->enemy.setSize(sf::Vector2f(10.f, 10.f));
    break;
  case 1:
    this->enemy.setFillColor(sf::Color::Blue);
    this->enemy.setSize(sf::Vector2f(30.f, 30.f));
    break;
  case 2:
    this->enemy.setFillColor(sf::Color::Cyan);
    this->enemy.setSize(sf::Vector2f(50.f, 50.f));
    break;
  case 3:
    this->enemy.setFillColor(sf::Color::Red);
    this->enemy.setSize(sf::Vector2f(70.f, 70.f));
    break;
  case 4:
    this->enemy.setFillColor(sf::Color::Green);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    break;
  default:
    this->enemy.setFillColor(sf::Color::Yellow);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));

    break;
  }

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

  for(size_t i = 0; i < this->enemies.size(); i++) {
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

      for(size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
        if(this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {


          // Gain points
          if(this->enemies[i].getFillColor() == sf::Color::Magenta) {
            this->points += 10.f;
          } else if(this->enemies[i].getFillColor() == sf::Color::Blue) {
            this->points += 7.f;
          } else if(this->enemies[i].getFillColor() == sf::Color::Cyan) {
            this->points += 5.f;
          } else if(this->enemies[i].getFillColor() == sf::Color::Red) {
            this->points += 3.f;
          } else if(this->enemies[i].getFillColor() == sf::Color::Green) {
            this->points += 1.f;
          } else {
            this->points += 1.f;
          }

          // Remove enemy
          deleted = true;
          this->enemies.erase(this->enemies.begin() + i);
        }
      }
    }
  } else {
    this->mouseHeld = false;
  }
}


void Game::updateText() {
  std::stringstream ss;

  ss << "Points: " << this->points << std::endl
     << "Health: " << this->health << std::endl;

  this->uiText.setString(ss.str());
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
    this->updateText();
  }

  // End Game Condition
  if(this->health <= 0) {
    this->endGame = true;
  }

}

void Game::renderEnemies(sf::RenderTarget& target) {

  // rendering all enemies
  for(auto &e: this->enemies) {
    target.draw(e);
   }
}

void Game::renderText(sf::RenderTarget& target) {
  target.draw(this->uiText);
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
  this->renderEnemies(*this->window);

  this->renderText(*this->window);

  this->window->display();

}
