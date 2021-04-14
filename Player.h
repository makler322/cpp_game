#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"



struct Point
{
  int x;
  int y;
};


enum class MovementDir
{
  STOP,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  ATTACK
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10});
  void Init(Point pos = {.x = 10, .y = 10});
  MovementDir Moved();
  int HelpProcessInput(MovementDir dir, Background &backgroundData);
  int ProcessInput(MovementDir dir, Background &backgroundMain, Background &backgroundData, bool fox = false);
  void DrawFire(Background &backgroundMain, char const* path, bool typeOfFire);
  void DrawHealth(Background &backgroundMain, Background &backgroundData, int lives);
  void Draw(Background &backgroundMain, Background &backgroundData, bool step, bool fox = false);
  void Attack(Background &backgroundMain, Background &backgroundData, int i, int j, bool isClear = false);
  int GetAttack_x() { return attack_x; };
  int GetAttack_y() { return attack_y; };
  void AttackHelp(Background &backgroundMain, Background &backgroundData);

private:
  int attack_x = 0, attack_y = 0;
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  int move_speed = 1;

};


#endif //MAIN_PLAYER_H
