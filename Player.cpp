#include "Player.h"


Image playerInitImage("./resources/front1.png");
Image floorImage1("./resources/floor.png"); // .
Image lightingAroundImage1("./resources/floorlamp.png"); //  Вокруг

Image playerLeftOneImage("./resources/left1.png");
Image playerLeftTwoImage("./resources/left2.png");

Image playerRightOneImage("./resources/right1.png");
Image playerRightTwoImage("./resources/right2.png");

Image playerUpOneImage("./resources/back1.png");
Image playerUpTwoImage("./resources/back2.png");

Image playerDownOneImage("./resources/front1.png");
Image playerDownTwoImage("./resources/front2.png");

Image foxLeftImage("./resources/foxLeft.png");
Image foxRightImage("./resources/foxRight.png");


Image healthImage("./resources/health.png");
Image healthBackImage("./resources/backLives.png");
Image healthMinusImage("./resources/hearthminus.png");
Image attackImage("./resources/break.png");

Image floors("./resources/floor.png"); // .
Image fireOne("./resources/fire1.png"); // probel
Image fireTwo("./resources/fire2.png"); // probel

Player::Player(Point pos)
{
  coords = pos;
  old_coords = coords;
}

void Player::Init(Point pos)
{
  coords = pos;
  old_coords = coords;
}

MovementDir Player::Moved()
{
  //printf("%d %d\n", coords.y, old_coords.y);
  if(coords.x > old_coords.x) return MovementDir::RIGHT;
  if(coords.x < old_coords.x) return MovementDir::LEFT;
  if(coords.y > old_coords.y) return MovementDir::UP;
  if(coords.y < old_coords.y) return MovementDir::DOWN;
  return MovementDir::STOP;
}

int Player::HelpProcessInput(MovementDir dir, Background &backgroundData)
{
  switch(dir)
  {
    
    case MovementDir::UP:
    {
      //printf("%d == %d | \n", backgroundData.TypeOfPixel(coords.x, coords.y + tileSize), backgroundData.TypeOfPixel(coords.x + tileSize - 1, coords.y + tileSize));
      if (backgroundData.TypeOfPixel(coords.x, coords.y + tileSize) == backgroundData.TypeOfPixel(coords.x + tileSize - 1, coords.y + tileSize))
        return backgroundData.TypeOfPixel(coords.x, coords.y + tileSize);
      else
        return 0;
      break;
    }
    case MovementDir::DOWN:
    {
      if (backgroundData.TypeOfPixel(coords.x, coords.y - 1) == backgroundData.TypeOfPixel(coords.x+ tileSize - 1, coords.y - 1))
        return backgroundData.TypeOfPixel(coords.x, coords.y - 1);
      else
        return 0;
      break;
    }
    case MovementDir::RIGHT:
    {
      if (backgroundData.TypeOfPixel(coords.x + tileSize, coords.y) == backgroundData.TypeOfPixel(coords.x + tileSize, coords.y + tileSize - 1))
        return backgroundData.TypeOfPixel(coords.x + tileSize, coords.y);
      else
        return 0;
      break;
    }
    case MovementDir::LEFT:
    {
      if (backgroundData.TypeOfPixel(coords.x - 1, coords.y) == backgroundData.TypeOfPixel(coords.x - 1, coords.y + tileSize - 1))
        return backgroundData.TypeOfPixel(coords.x - 1, coords.y);
      else
        return 0;
      break;
    }
    case MovementDir::ATTACK:
    {
      return 8;
      break;
    }
    case MovementDir::STOP:
    {
      return 0;
      break;
    }
    default:
    {
      return 0;
      break;
    }
  }
}

void Player::Attack(Background &backgroundMain, Background &backgroundData, int i, int j, bool isClear)
{
  //printf("%d\n", backgroundData.TypeOfPixel(i, j));
  
  if(isClear)
  {
    for (int k = i; k < i + 16; k++)
    {
      for (int l = j; l < j + 16; l++)
      {
        if (backgroundData.TypeOfPixel(k, l) == 7)
        {
          backgroundMain.SetPixel(k, l, floors.GetPixel(k-i, l-j), 3);
          backgroundData.SetPixel(k, l, floors.GetPixel(k-i, l-j), 3);
        }
          
        else
          backgroundMain.SetPixel(k, l, backgroundData.WhatPixel(k, l), backgroundData.TypeOfPixel(k, l));
      }
    }
  }
  else
  {
    int a = 0;
    for (int k = i; k < i + 16; k++)
    {
      for (int l = j; l < j + 16; l++)
      {
        if (backgroundData.TypeOfPixel(k, l) == 7 || backgroundData.TypeOfPixel(k, l) == 3)
          a++;
      }
    }
    if (a == 256)
    {
      backgroundMain.SetSquare(attackImage.Data(), i, j, 8);
    }
  }
  
}

void Player::AttackHelp(Background &backgroundMain, Background &backgroundData)
{
  int i = attack_x;
  int j = attack_y;
  //printf("jj\n");
  Attack(backgroundMain, backgroundData, i, j+16, true);
  Attack(backgroundMain, backgroundData, i, j+32, true);
  Attack(backgroundMain, backgroundData, i, j-16, true);
  Attack(backgroundMain, backgroundData, i, j-32, true);
  Attack(backgroundMain, backgroundData, i+16, j, true);
  Attack(backgroundMain, backgroundData, i+16, j+16, true);
  Attack(backgroundMain, backgroundData, i+16, j-16, true);
  Attack(backgroundMain, backgroundData, i+32, j, true);
  Attack(backgroundMain, backgroundData, i-16, j+16, true);
  Attack(backgroundMain, backgroundData, i-16, j, true);
  Attack(backgroundMain, backgroundData, i-16, j-16, true);
  Attack(backgroundMain, backgroundData, i-32, j, true);
}

int Player::ProcessInput(MovementDir dir, Background &backgroundMain, Background &backgroundData, bool fox)
{
  switch (HelpProcessInput(dir, backgroundData))
  {
  case 1: //огонь
  {
    return 1;
    break;
  }
  case 2: //стена
  {
    return 2;
    break;
  }
  case 3: //пол
  {
    if (!fox)
    {
      //printf("|||||||||||||||||\n");
      for (int j = old_coords.y - 1; j < old_coords.y + 1; j++)
      {
        for (int i = old_coords.x - 1; i < old_coords.x + 1; i++)
        {
          //printf("i = %d | j = %d | type = %d\n", i, j, backgroundMain.TypeOfPixel(i, j));
          if (backgroundMain.TypeOfPixel(i, j) == 4)
            return 4;
        }
      }
    }
    else
    {
      for (int j = old_coords.y - 1; j < old_coords.y + 1; j++)
      {
        for (int i = old_coords.x - 1; i < old_coords.x + 1; i++)
        {
          //printf("i = %d | j = %d | type = %d\n", i, j, backgroundMain.TypeOfPixel(i, j));
          if (backgroundMain.TypeOfPixel(i, j) == 8)
            return 9;
        }
      }
    }
    
    switch(dir)
    {
      case MovementDir::UP:
      {
        old_coords.y = coords.y;
        coords.y += 1;
        break;
      }
      case MovementDir::DOWN:
      {
        old_coords.y = coords.y;
        coords.y -= 1;
        break;
      }
      case MovementDir::RIGHT:
      {
        old_coords.x = coords.x;
        coords.x += 1;
        break;
      }
      case MovementDir::LEFT:
      {
        old_coords.x = coords.x;
        coords.x -= 1;
        break;
      }
    }
    return 3; 
    break;
  }
  case 4: //враг
  {
    return 4;
    break;
  }
  case 5: //выход с уровня
  {
    return 5;
    break;
  }
  case 6: //освещение
  {
    switch(dir)
    {
      case MovementDir::UP:
      {
        old_coords.y = coords.y;
        coords.y += 1;
        break;
      }
      case MovementDir::DOWN:
      {
        old_coords.y = coords.y;
        coords.y -= 1;
        break;
      }
      case MovementDir::RIGHT:
      {
        old_coords.x = coords.x;
        coords.x += 1;
        break;
      }
      case MovementDir::LEFT:
      {
        old_coords.x = coords.x;
        coords.x -= 1;
        break;
      }
    }
    return 6; 
    break;
  }
  case 7:
  {
    return 7;
    break;
  }
  case 8: //атака
  {
    //printf("45\n");
    int i = old_coords.x, j = old_coords.y; 
    attack_x = i;
    attack_y = j;
    Attack(backgroundMain, backgroundData, i, j+16);
    Attack(backgroundMain, backgroundData, i, j+32);
    Attack(backgroundMain, backgroundData, i, j-16);
    Attack(backgroundMain, backgroundData, i, j-32);
    Attack(backgroundMain, backgroundData, i+16, j);
    Attack(backgroundMain, backgroundData, i+16, j+16);
    Attack(backgroundMain, backgroundData, i+16, j-16);
    Attack(backgroundMain, backgroundData, i+32, j);
    Attack(backgroundMain, backgroundData, i-16, j+16);
    Attack(backgroundMain, backgroundData, i-16, j);
    Attack(backgroundMain, backgroundData, i-16, j-16);
    Attack(backgroundMain, backgroundData, i-32, j);
    return 8;
    break;
  }
  default:
  {
    return 0;
    break;
  }
    
  }
}

void Player::DrawFire(Background &backgroundMain, char const* path, bool typeOfFire)
{
  FILE *file;
  file = fopen(path, "r");
  int symbol, i = 0, j = 0;
  if (file)
  {
    while((symbol = getc(file)) != EOF)
    {
      if (symbol == '\n')
      {
        i = -tileSize;
        j += tileSize;
      }
      else
      {
        switch (symbol)
        {
          case ' ': //огонь
          {
            backgroundMain.SetSquare(floors.Data(), i, abs(WINDOW_HEIGHT - j) - tileSize, 1);
            if (typeOfFire)
              backgroundMain.SetSquare(fireOne.Data(), i, abs(WINDOW_HEIGHT - j) - tileSize, 1);
            else
              backgroundMain.SetSquare(fireTwo.Data(), i, abs(WINDOW_HEIGHT - j) - tileSize, 1);
            break;
          }
          default:
            break;
        }
      }
      i += tileSize;
    }
  }
  else
  {
    printf("You have some trouble with file %s!\n", path);
    exit(-1);
  }
  fclose(file);
}

void Player::DrawHealth(Background &backgroundMain, Background &backgroundData, int lives)
{

  backgroundMain.SetImage(healthBackImage.Data(), 880, 877, healthBackImage.Height(), healthBackImage.Width());
  backgroundMain.SetImage(healthBackImage.Data(), 800, 877, healthBackImage.Height(), healthBackImage.Width());
  backgroundMain.SetImage(healthBackImage.Data(), 720, 877, healthBackImage.Height(), healthBackImage.Width());

  switch (lives)
  {
  case 0:
    {
      backgroundMain.SetImage(healthMinusImage.Data(), 880, 877, healthMinusImage.Height(), healthMinusImage.Width());
      backgroundMain.SetImage(healthMinusImage.Data(), 800, 877, healthMinusImage.Height(), healthMinusImage.Width());
      backgroundMain.SetImage(healthMinusImage.Data(), 720, 877, healthMinusImage.Height(), healthMinusImage.Width());
    }
    break;
  case 1:
    {
      backgroundMain.SetImage(healthMinusImage.Data(), 880, 877, healthMinusImage.Height(), healthMinusImage.Width());
      backgroundMain.SetImage(healthMinusImage.Data(), 800, 877, healthMinusImage.Height(), healthMinusImage.Width());
      backgroundMain.SetImage(healthImage.Data(), 720, 877, healthImage.Height(), healthImage.Width());
    }
    break;
  case 2:
    {
      backgroundMain.SetImage(healthMinusImage.Data(), 880, 877, healthMinusImage.Height(), healthMinusImage.Width());
      backgroundMain.SetImage(healthImage.Data(), 800, 877, healthImage.Height(), healthImage.Width());
      backgroundMain.SetImage(healthImage.Data(), 720, 877, healthImage.Height(), healthImage.Width());
    }
    break;
  case 3:
    {
      backgroundMain.SetImage(healthImage.Data(), 880, 877, healthImage.Height(), healthImage.Width());
      backgroundMain.SetImage(healthImage.Data(), 800, 877, healthImage.Height(), healthImage.Width());
      backgroundMain.SetImage(healthImage.Data(), 720, 877, healthImage.Height(), healthImage.Width());
    }
    break;
  default:
    break;
  }
}



void Player::Draw(Background &backgroundMain, Background &backgroundData, bool step, bool fox)
{
  
  switch (Moved())
  {
    case MovementDir::UP:
    {
      backgroundMain.SetSquare(backgroundData.Data(), old_coords.x, old_coords.y, 3, true);
      if (step) backgroundMain.SetSquare(playerUpOneImage.Data(), coords.x, coords.y, 3);
      else backgroundMain.SetSquare(playerUpTwoImage.Data(), coords.x, coords.y, 3);
      old_coords.x = coords.x;
      old_coords.y = coords.y;
      break;
    }
    case MovementDir::DOWN:
    {
      backgroundMain.SetSquare(backgroundData.Data(), old_coords.x, old_coords.y, 3, true);
      if (step) backgroundMain.SetSquare(playerDownOneImage.Data(), coords.x, coords.y, 3);
      else backgroundMain.SetSquare(playerDownTwoImage.Data(), coords.x, coords.y, 3);
      old_coords.x = coords.x;
      old_coords.y = coords.y;
      break;
    }
    case MovementDir::RIGHT:
    {
      backgroundMain.SetSquare(backgroundData.Data(), old_coords.x, old_coords.y, 3, true);
      if (fox)
      {
        backgroundMain.SetSquare(foxRightImage.Data(), coords.x, coords.y, 4);
      }
      else
      {
        if (step) backgroundMain.SetSquare(playerRightOneImage.Data(), coords.x, coords.y, 3);
        else backgroundMain.SetSquare(playerRightTwoImage.Data(), coords.x, coords.y, 3);
      }
      old_coords.x = coords.x;
      old_coords.y = coords.y;
      break;
    }
    case MovementDir::LEFT:
    {

      backgroundMain.SetSquare(backgroundData.Data(), old_coords.x, old_coords.y, 3, true);
      if (fox)
      {
        backgroundMain.SetSquare(foxLeftImage.Data(), coords.x, coords.y, 4);
        
      }
      else
      {
        if (step) backgroundMain.SetSquare(playerLeftOneImage.Data(), coords.x, coords.y, 3);
        else backgroundMain.SetSquare(playerLeftTwoImage.Data(), coords.x, coords.y, 3);
      }
      old_coords.x = coords.x;
      old_coords.y = coords.y;
      break;
    }
    case MovementDir::STOP:
    {
      backgroundMain.SetSquare(backgroundData.Data(), old_coords.x, old_coords.y, 3, true);
      backgroundMain.SetSquare(playerInitImage.Data(), coords.x, coords.y, 3);
      old_coords.x = coords.x;
      old_coords.y = coords.y;
      break;
    }
    default:
      break;
  }
}