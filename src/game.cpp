#include "game.h"
#include <iostream>
#include <thread>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      snakebot(grid_width, grid_height), // instantiate snake robot
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  snakebot.PlanRoute(food);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  //bool running = true;
  std::shared_ptr<bool> running(new bool); // leon 0710
  *running = true; // leon 0710
  snakebot.simulate(running);

  while (*running) {
    
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, snakebot, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  return;
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  /*
  if (!snake.alive) return;
  snake.Update();

  int new_snake_x = static_cast<int>(snake.head_x);
  int new_snake_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_snake_x && food.y == new_snake_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;

    snakebot.PlanRoute(food);
  }
  */

  int new_snakebot_x = static_cast<int>(snakebot._head_x);
  int new_snakebot_y = static_cast<int>(snakebot._head_y);

  // Check if there's food over here
  if(food.x == new_snakebot_x && food.y == new_snakebot_y){
    PlaceFood();
    snakebot.GrowBody();
    snakebot.PlanRoute(std::move(food));
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }