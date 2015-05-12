/*
 * main.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: itay
 */

using namespace std;

#include "lodepng.h"
#include <iostream>

#define PIXEL_SIZE 0.036
#define ROBOT_RADIUS 0.45
#define PAINT_RADIUS 13


std::vector<unsigned char> image; //the raw pixels
unsigned width, height;

void loadImage(const char* filename)
{
  //decode
  unsigned error = lodepng::decode(image, width, height, filename);

  //if there's an error, display it
  if(error) cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
}

int main()
{
  const char* filename = "hospital_seciton.png";
  loadImage(filename);


  cout << "width: " << width << ", height: " << height << endl;


  for (unsigned i = 0; i < width * height * 4; i += 4)
  {
      if (image[i] == 255)
      {
          unsigned p = i - width;
          for (unsigned j = p - PAINT_RADIUS; j < i + PAINT_RADIUS; j++)
          {
              image[j] = 255;
          }

      }
  }
}
