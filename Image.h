#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <glad/glad.h>
#include "stb_image.h"
#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr int tileSize = 16;
constexpr GLsizei WINDOW_WIDTH = 60*tileSize, WINDOW_HEIGHT = 60*tileSize;



struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};


struct Image
{
  explicit Image(const std::string &a_path);

  int Save(const std::string &a_path);
  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }
  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  
  //void SetImage(Pixel* pixels, int x, int y, int heightOfImage, int wightOfImage);

  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  
  bool self_allocated = false;
};


struct Background
{
  explicit Background(char const* path, bool perehod = false, GLFWwindow* window = nullptr);
  void Init(char const* path, bool perehod = false, GLFWwindow* window = nullptr);
  void SetPixel(int x, int y, Pixel p, int a);
  Pixel WhatPixel(int x, int y) { return data[width * y + x]; }
  int Save(const std::string &a_path);
  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }
  int TypeOfPixel(int x, int y)   { return typeOfPixel[width * y + x];}
  void SetSquare(Pixel *pixels, int x, int y, int typeOfPixels, bool maybeNextTime = false, bool transparentBackground = true);
  void SetImage(Pixel* pixels, int x, int y, int heightOfImage, int wightOfImage, bool transparentBackground = true);
  int GetInit_x() { return  init_x; }
  int GetInit_y() { return  init_y; }
  int GetInitFox_x() { return  initFox_x; }
  int GetInitFox_y() { return  initFox_y; }
  void BackgroundLight(int i, int j);

  private:
    int init_x = 150, init_y = 150;
    int initFox_x = 150, initFox_y = 150;
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    int channels = 4;
    size_t size = width * height * channels;
    bool self_allocated = false;
    bool animation = false;
    GLFWwindow* window = nullptr;
    Pixel *data = new Pixel[width * height];
    int *typeOfPixel = new int[width * height];
};
#endif //MAIN_IMAGE_H
