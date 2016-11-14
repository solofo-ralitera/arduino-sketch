#include <SPI.h>
#include <TFT.h>            // Arduino TFT library

#define cs   10
#define dc   9
#define rst  8

TFT screen = TFT(cs, dc, rst);

void setup() {
  // initialize the screen
  screen.begin();

  // make the background black
  screen.background(0,0,0);

  // set the stroke color to white
  screen.stroke(255,255,255);

  // set the fill color to grey
  screen.fill(127,127,127);

  // draw a circle in the center of screen
  screen.circle(screen.width()/2, screen.height()/2, 10);
}

void loop() {

}
