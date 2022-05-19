#include <TFT.h>
#include <SPI.h>

#define cs 10
#define dc 9
#define rst 8

const int MAT_SIZE = 20;
const int screen_offset_x = 50;
const int screen_offset_y = 50;

TFT screen = TFT(cs, dc, rst);
boolean matrix[MAT_SIZE][MAT_SIZE];
int element_nr = 0;
int step_nr = 0;

void display_matrix(){
  for(int x=0; x<MAT_SIZE; x++){
    for(int y=0; y<MAT_SIZE; y++){
      //Serial.println(matrix[x][y]);
      if(matrix[x][y] == 1){
        screen.point(x*3 - 1 + screen_offset_x, y*3 - 1 + screen_offset_y);
        screen.point(x*3 - 1 + screen_offset_x, y*3 + screen_offset_y);
        screen.point(x*3 - 1 + screen_offset_x, y*3 + 1 + screen_offset_y);
        screen.point(x*3 + screen_offset_x, y*3 - 1 + screen_offset_y);
        screen.point(x*3 + screen_offset_x, y*3 + screen_offset_y);
        screen.point(x*3 + screen_offset_x, y*3 + 1 + screen_offset_y);
        screen.point(x*3 + 1 + screen_offset_x, y*3 - 1 + screen_offset_y);
        screen.point(x*3 + 1 + screen_offset_x, y*3 + screen_offset_y);
        screen.point(x*3 + 1 + screen_offset_x, y*3 + 1 + screen_offset_y);
      }
    }
  }
}

int neighbor_nr(int x, int y){
  int nr = 0;
    if( x - 1 >= 0 and y - 1 >= 0){  // NW
        if( matrix[x - 1][ y - 1] == 1)
            nr ++;
    }
    if( x - 1 >= 0){  // N
        if( matrix[x - 1][ y] == 1)
            nr ++;
    }
    if( x - 1 >= 0 and y + 1 < MAT_SIZE){  // NE
        if( matrix[x - 1][ y + 1] == 1)
            nr ++;
    }
    if( y - 1 >= 0){  // W
        if( matrix[x][ y - 1] == 1)
            nr ++;
    }
    if( y + 1 < MAT_SIZE){  // E
        if( matrix[x][ y + 1] == 1)
            nr ++;
    }
    if( x + 1 < MAT_SIZE and y - 1 >= 0){  // SW
        if( matrix[x + 1][ y - 1] == 1)
            nr ++;
    }
    if( x + 1 < MAT_SIZE){  // S
        if( matrix[x + 1][ y] == 1)
            nr ++;
    }
    if( x + 1 < MAT_SIZE and y + 1 < MAT_SIZE){ // SE
        if( matrix[x + 1][ y + 1] == 1)
            nr ++;
    }
    return nr;
}

void next_step(){
  for(int x=0; x<MAT_SIZE; x++){
    for(int y=0; y<MAT_SIZE; y++){
      if(matrix[x][y] == 0){
        if(neighbor_nr(x, y) == 3)
          matrix[x][y] = 1;
      }
      else{
        if(neighbor_nr(x, y) != 2 && neighbor_nr(x, y) != 3)
          matrix[x][y] = 0;
      }
    }
  }
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  screen.begin();
  screen.background(0,0,0);
  screen.stroke(255,255,255);
  screen.setTextSize(2);
}

void loop() {
  if(element_nr<(MAT_SIZE*MAT_SIZE)/2){
    int x = random(0, MAT_SIZE);
    int y = random(0, MAT_SIZE);
    if(matrix[x][y] == 0){
      element_nr++;
      matrix[x][y] = 1;
    }
    String element_nr_string = "";
    element_nr_string += element_nr;
    element_nr_string += "/";
    element_nr_string += (MAT_SIZE*MAT_SIZE)/2;
    Serial.println(element_nr_string);
    screen.text(element_nr_string.c_str(), 10, 10);
    display_matrix();
    screen.background(0,0,0);
  }
  else{
    step_nr++;
    Serial.println("Done");
    display_matrix();
    String step_string = "Step ";
    step_string += step_nr;
    screen.text(step_string.c_str(), 10, 10);
    screen.background(0,0,0);
    next_step();
  }
}
