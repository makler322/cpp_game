#include "common.h"
#include "Image.h"
#include "Player.h"
#include "stdio.h"
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <experimental/random>
#define GLFW_DLL
#include <GLFW/glfw3.h>

Image loseImage("./resources/game_over.png");
Image winImage("./resources/win.png");

Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
Point startingFox_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

int processPlayerMovement(Player &player, Background &backgroundMain, Background &backgroundData, bool fox = false, int aim = 1, bool checkAttack = true)
{
  if (fox)
  {
    if (aim == 1)
    {
      int g = player.ProcessInput(MovementDir::LEFT, backgroundMain, backgroundData, fox);
      if (g == 2)
        return 11; //упёрлась в стену
      else
        if (g == 9)
          return 9;
    }
    else
    {
      int h = player.ProcessInput(MovementDir::RIGHT, backgroundMain, backgroundData, fox);
      if (h == 2)
        return 13; //упёрлась в стену
      else
        if (h == 9)
          return 9;
        
    }
    return 12;
  }
  int movementY = 0, movementX = 0;
  if (Input.keys[GLFW_KEY_W])
    movementY = player.ProcessInput(MovementDir::UP, backgroundMain, backgroundData);
  else if (Input.keys[GLFW_KEY_S])
    movementY =  player.ProcessInput(MovementDir::DOWN, backgroundMain, backgroundData);
  if (Input.keys[GLFW_KEY_A])
    movementX = player.ProcessInput(MovementDir::LEFT, backgroundMain, backgroundData);
  else if (Input.keys[GLFW_KEY_D])
    movementX = player.ProcessInput(MovementDir::RIGHT, backgroundMain, backgroundData);
  if (Input.keys[GLFW_KEY_Q] && checkAttack)
    return player.ProcessInput(MovementDir::ATTACK, backgroundMain, backgroundData);
  //printf("%d | %d\n", movementY, movementX);
  if ((movementY == 1) || (movementX == 1))
  {
    return 1; //огонь - смерть
  }
  if ((movementY == 4) || (movementX == 4)) //враг - минус жизнь
  {
    return 4;
  }
  if ((movementY == 5) || (movementX == 5)) //выход с уровня - переход
  {
    return 5;
  }
  return 3; // пол | освещение | 
}


void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}


int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();
///////////////////////////////////////////////////////////////////////////
	char const* pathLevel1 = "./resources/level1.txt";
  char const* perehod = "./resources/perehod.txt";
  char const* pathLevel2 = "./resources/level2.txt";

	Background backgroundMain(pathLevel1);
	Background backgroundData(pathLevel1);

  starting_pos.x = backgroundData.GetInit_x();
  starting_pos.y = backgroundData.GetInit_y();
  
  startingFox_pos.x = backgroundData.GetInitFox_x();
  startingFox_pos.y = backgroundData.GetInitFox_y();

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  
	Player player{starting_pos};
  Player fox{startingFox_pos};

  int stepProgramm = 0, stepProgrammFire = 0, aim = 1, lives = 3, stepLives = 0, stepAttack = 0;
  bool stepPlayer = true, checkProgramm = true, stepFire = true, checkFox = true, checkLives = true, checkAttack = false, goToParadise = false;
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    stepProgramm++;
    stepProgrammFire++;
    stepLives++;
    stepAttack++;
    glfwPollEvents();

    if (checkProgramm && checkFox)
    switch(processPlayerMovement(fox, backgroundMain, backgroundData, true, aim, 1))
    {
      case 11:
      {
        aim = 2;
        break;
      }
      case 13:
      {
        aim = 1;
        break;
      }
      case 9:
      {
        //printf("45\n");
        checkFox = false;
        break;
      }
      default:
      {
        break;
      }

    }
    if (checkProgramm)
    switch(processPlayerMovement(player, backgroundMain, backgroundData, false, 1, !checkAttack))
    {
      case 1:
      {
        backgroundMain.SetImage(loseImage.Data(), 0, 0, loseImage.Height(), loseImage.Width(), false);
        checkProgramm = false;
        break;
      }
      case 4:
      {
        if (stepLives > 70)
        {
          stepLives = 0;
          lives -= 1;
          if (lives == 0)
          {
            backgroundMain.SetImage(loseImage.Data(), 0, 0, loseImage.Height(), loseImage.Width(), false);
            checkProgramm = false;
          }
        }
        
        break;
      }
      case 5:
      {
        if (goToParadise)
        {
          backgroundMain.SetImage(winImage.Data(), 0, 0, winImage.Height(), winImage.Width(), false);
          checkProgramm = false;
        }
        else
        {
          goToParadise = true;
          
          Background backgroundPerehod(perehod, true, window);
          
          backgroundMain.Init("./resources/helpback.txt");
          backgroundData.Init("./resources/helpback.txt");
          backgroundMain.Init(pathLevel2);
	        backgroundData.Init(pathLevel2);

          starting_pos.x = backgroundData.GetInit_x();
          starting_pos.y = backgroundData.GetInit_y();
          
          startingFox_pos.x = backgroundData.GetInitFox_x();
          startingFox_pos.y = backgroundData.GetInitFox_y();
          
          player.Init(starting_pos);
          fox.Init(startingFox_pos);

          stepProgramm = 0, stepProgrammFire = 0, aim = 1, lives = 3, stepLives = 0, stepAttack = 0;
          stepPlayer = true, checkProgramm = true, stepFire = true, checkFox = true, checkLives = true, checkAttack = false;
        }
        
        break;
      }
      case 3:
      {
        //printf("47\n");
        break;
      }
      case 8:
      {
        stepAttack = 0;
        checkAttack = true;
        break;
      }
      default:
      {
        break;
      }
    }
    if (stepProgramm > 10)
    {
      stepPlayer = not stepPlayer;
      stepProgramm = 0;
    }
    if (stepProgrammFire > 100)
    {
      stepFire = not stepFire;
      stepProgrammFire = 0;
    }
    if (checkProgramm)
    {
      if (checkFox)
        fox.Draw(backgroundMain, backgroundData, stepPlayer, true);

      if (checkAttack && (stepAttack > 50))
      {
        //printf("55\n");
        stepAttack = 0;
        player.AttackHelp(backgroundMain, backgroundData);
        checkAttack = false;
      }
      if (goToParadise)
        player.DrawFire(backgroundMain, pathLevel2, stepFire);
      else
        player.DrawFire(backgroundMain, pathLevel1, stepFire);
      
      player.DrawHealth(backgroundMain, backgroundData, lives);
      player.Draw(backgroundMain, backgroundData, stepPlayer);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, backgroundMain.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
