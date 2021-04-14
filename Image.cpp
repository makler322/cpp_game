#include "Image.h"
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <chrono>
#include <thread>

#include <iostream>





Image floorImage("./resources/floor.png"); // .
Image wallImage("./resources/wall.png"); // #
//Image playerImage("./resources/stena_lomayuschayasya.png"); // @
Image exitImage("./resources/exit.png"); // X
Image lightingImage("./resources/lamp.png"); // 
Image lightingAroundImage("./resources/floorlamp.png"); //  Вокруг
Image wallBreakImage("./resources/wall2.png"); // %
Image breakImage("./resources/break.png");




Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 4)) != nullptr)
  {
    size = width * height * channels;
  }
}



int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}

void Background::SetSquare(Pixel *pixels, int x, int y, int typeOfPixels, bool maybeNextTime, bool transparentBackground)
{
  for (int i = 0; i < tileSize; i ++)
  {
    for (int j = 0; j < tileSize; j ++)
    {
      if (maybeNextTime)
      {
        if (transparentBackground)
        {
          if (pixels[width * (i+y) + (j+x)].r + pixels[width * (i+y) + (j+x)].g + pixels[width * (i+y) + (j+x)].b > 0)
          {
            data[width * (i+y) + (j+x)] = pixels[width * (i+y) + (j+x)];
          }
        }
        else 
        {
          data[width * (i+y) + (j+x)] = pixels[width * (i+y) + (j+x)];
        }
      }
      else
      {
        if (transparentBackground)
        {
          if (pixels[(tileSize-1-i)*tileSize+j].r + pixels[(tileSize-1-i)*tileSize+j].g + pixels[(tileSize-1-i)*tileSize+j].b > 0)
          {
            data[width * (i+y) + (j+x)] = pixels[(tileSize-1-i)*tileSize+j];
          }
        }
        else
        {
          data[width * (i+y) + (j+x)] = pixels[(tileSize-1-i)*tileSize+j];
        }
      }
      typeOfPixel[width * (i+y) + (j+x)] = typeOfPixels;
    }
  }
}



void Background::SetImage(Pixel* pixels, int x, int y, int heightOfImage, int wightOfImage, bool transparentBackground)
{
  for (int i = 0; i < heightOfImage; i++)
  {
    for (int j = 0; j < wightOfImage; j++)
    {
      if (transparentBackground)
      {
        if (pixels[(heightOfImage-1-i)*wightOfImage+j].r + pixels[(heightOfImage-1-i)*wightOfImage+j].g + pixels[(heightOfImage-1-i)*wightOfImage+j].b > 0)
        {
          data[width * (i+y) + (j+x)] = pixels[(heightOfImage-1-i)*wightOfImage+j];
        }
      }
      else
      {
        data[width * (i+y) + (j+x)] = pixels[(heightOfImage-1-i)*wightOfImage+j];
      }
    }
  }
}

void Background::BackgroundLight(int i, int j)
{
  if (TypeOfPixel(i, abs(width - j) - tileSize) == 3 || TypeOfPixel(i, abs(width - j) - tileSize) == 0)
    SetSquare(lightingAroundImage.Data(), i, abs(width - j) - tileSize, 3);
}

Background::Background(char const* path, bool perehod, GLFWwindow* window)
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
        if (perehod)
        {
          glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, data);
          glfwSwapBuffers(window);
          std::this_thread::sleep_for(std::chrono::milliseconds(180));
        }
      }
      else
      {
        switch (symbol)
        {
          case ' ': //огонь
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 1);
            break;
          }
          case '#': //стена
          {
            
            SetSquare(wallImage.Data(), i, abs(width - j) - tileSize, 2);
            break;
          }
          case '.': //пол
          {
            //printf("floor\n");
            if (TypeOfPixel(i, abs(width - j) - tileSize) != 3)
              SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 3);
            break;
          }
          case '@': //игрок
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 3);
            init_x = i;
            init_y = abs(width - j) - tileSize;
            break;
          }
          case '*': //лиса
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 3);
            initFox_x = i;
            initFox_y = abs(width - j) - tileSize;
            break;
          }
          case 'x': //выход с уровня
          {
            //printf("exit\n");
            SetSquare(exitImage.Data(), i, abs(width - j) - tileSize, 5);
            break;
          }
          case '!': //освещение
          {
            //printf("light\n");
            SetSquare(lightingAroundImage.Data(), i, abs(width - j) - tileSize, 3);
            SetSquare(lightingImage.Data(), i, abs(width - j) - tileSize, 3);
            BackgroundLight(i, j+16);
             BackgroundLight(i, j+32);
            BackgroundLight(i, j-16);
            BackgroundLight(i, j-32);
             BackgroundLight(i+16, j);
             BackgroundLight(i+16, j+16);
             BackgroundLight(i+16, j-16);
             BackgroundLight(i+32, j);
             BackgroundLight(i-16, j+16);
            BackgroundLight(i-16, j);
            BackgroundLight(i-16, j-16);
            BackgroundLight(i-32, j);
            break;
          }
          case '%': //ломающаяся стена
          {
            SetSquare(wallImage.Data(), i, abs(width - j) - tileSize, 7);
            SetSquare(breakImage.Data(), i, abs(width - j) - tileSize, 7);
            break;
          }
          case ')': //ломающаяся стена
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 0);
            SetSquare(breakImage.Data(), i, abs(width - j) - tileSize, 0);
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

void Background::Init(char const* path, bool perehod, GLFWwindow* window)
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
        if (perehod)
        {
          glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, data);
          glfwSwapBuffers(window);
          std::this_thread::sleep_for(std::chrono::milliseconds(180));
        }
      }
      else
      {
        switch (symbol)
        {
          case ' ': //огонь
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 1);
            break;
          }
          case '#': //стена
          {
            
            SetSquare(wallImage.Data(), i, abs(width - j) - tileSize, 2);
            break;
          }
          case '.': //пол
          {
            //printf("floor\n");
            if (TypeOfPixel(i, abs(width - j) - tileSize) != 3)
              SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 3);
            break;
          }
          case '@': //игрок
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 3);
            init_x = i;
            init_y = abs(width - j) - tileSize;
            break;
          }
          case '*': //лиса
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 3);
            initFox_x = i;
            initFox_y = abs(width - j) - tileSize;
            break;
          }
          case 'x': //выход с уровня
          {
            //printf("exit\n");
            SetSquare(exitImage.Data(), i, abs(width - j) - tileSize, 5);
            break;
          }
          case '!': //освещение
          {
            //printf("light\n");
            SetSquare(lightingAroundImage.Data(), i, abs(width - j) - tileSize, 3);
            SetSquare(lightingImage.Data(), i, abs(width - j) - tileSize, 3);
            BackgroundLight(i, j+16);
             BackgroundLight(i, j+32);
            BackgroundLight(i, j-16);
            BackgroundLight(i, j-32);
             BackgroundLight(i+16, j);
             BackgroundLight(i+16, j+16);
             BackgroundLight(i+16, j-16);
             BackgroundLight(i+32, j);
             BackgroundLight(i-16, j+16);
            BackgroundLight(i-16, j);
            BackgroundLight(i-16, j-16);
            BackgroundLight(i-32, j);
            break;
          }
          case '%': //ломающаяся стена
          {
            SetSquare(wallImage.Data(), i, abs(width - j) - tileSize, 7);
            SetSquare(breakImage.Data(), i, abs(width - j) - tileSize, 7);
            break;
          }
          case ')': //ломающаяся стена
          {
            SetSquare(floorImage.Data(), i, abs(width - j) - tileSize, 0);
            SetSquare(breakImage.Data(), i, abs(width - j) - tileSize, 0);
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

void Background::SetPixel(int x, int y, Pixel p, int a)
{
  typeOfPixel[width * y + x] = a;
  data[width * y + x] = p;
}
