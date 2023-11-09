import processing.serial.*;
import uibooster.*;

Serial myPort;
String val;
boolean started = false;
int scale = 2;

void setup()
{
  size(600, 600);
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 115200);
  noCursor();
  background(255, 255, 255);
}

int[] state = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int[] state = {0, 13, 4, 25, 50, 10, 140, 225, 15, 275, 150, 20, 25, 250, 20, 160, 100, 20, 85, 260, 15, 0, 0, 0, 300, 300, 0, 300, 300, 0, 0, 300, 0, 0, 300, 300, 300, 0, 75, 200, 75, 250, 75, 250, 200, 50, 150, 50, 300, 200, 75, 200, 150, 100, 100, 100, 200, 100, 200, 300, 200, 100, 200, 100, 250, 100, 250, 275, 250, 50, 25, 50, 75};
//int[] state = {0, 10, 9, 35, 225, 10, 275, 130, 15, 125, 280, 15, 175, 220, 15, 275, 275, 15, 150, 185, 12, 20, 125, 15, 75, 60, 15, 150, 75, 15, 210, 135, 10, 280, 70, 15, 0, 0, 0, 300, 300, 0, 300, 300, 0, 0, 300, 0, 0, 300, 300, 300, 0, 200, 225, 200, 50, 200, 50, 275, 75, 150, 300, 150, 75, 80, 75, 150, 150, 0, 150, 60, 225, 60, 225, 150};
//int[] state = {0, 20, 27, 215, 160, 7, 212, 210, 6, 150, 140, 7, 213, 115, 5, 210, 90, 10, 175, 85, 5, 150, 82, 5, 130, 85, 5, 90, 90, 12, 85, 130, 7, 100, 200, 15, 260, 115, 10, 235, 60, 10, 165, 10, 10, 165, 65, 10, 145, 10, 10, 145, 65, 10, 35, 10, 10, 35, 80, 10, 10, 137, 10, 65, 210, 10, 70, 240, 10, 100, 290, 10, 130, 240, 10, 160, 290, 10, 190, 240, 10, 220, 290, 10, 250, 240, 10, 280, 290, 10, 0, 0, 0, 300, 300, 0, 300, 300, 0, 0, 300, 0, 0, 300, 300, 300, 200, 150, 222, 150, 150, 150, 150, 172, 150, 175, 272, 175, 225, 75, 225, 175, 75, 75, 225, 75, 75, 75, 75, 95, 30, 225, 200, 225, 230, 225, 300, 225, 275, 150, 275, 175, 250, 100, 300, 100, 85, 30, 85, 75, 0, 150, 50, 150, 193, 200, 232, 200, 190, 200, 190, 225, 235, 200, 235, 225, 75, 130, 75, 225};
void disp()
{
    int wlen = state[1];
    int hlen = state[2];
    
    background(255, 255, 255);
    
    //Goal
    fill(0, 150, 0);
    circle(state[6]*scale, state[7]*scale, 2*state[8]*scale);
    
    //Holes
    fill(255, 51, 51); //<>//
    for(int i=9; i<9+3*hlen; i+=3){
      circle(state[i]*scale, state[i+1]*scale, 2*state[i+2]*scale);
    }
    
    //Walls
    stroke(0);
    strokeWeight(3*scale);
    for(int i=9+3*hlen; i<9+3*hlen+4*wlen; i+=4){
      line(state[i]*scale, state[i+1]*scale, state[i+2]*scale, state[i+3]*scale);
    }
    
    //Ball
    strokeWeight(1*scale);
    fill(161, 10, 216);
    circle(state[3]*scale, state[4]*scale, 2*state[5]*scale);
}

void draw()
{
  if ( myPort.available() > 0) 
  {   //If data is available,
    val = myPort.readStringUntil('\n');         // read it and store it in val
    println(val);
    if(val!=null && val.contains("TheWorldEatingPythonIsBack"))
      started=true;
    else if(val!=null && started==true)
    {
      int[] data= int(trim(split(trim(val), ' ')));
      if(data[0]==1)
      {
        print(data);
        if(data[1]==1)
          new UiBooster().showConfirmDialog(
                  "Do you want to go to the next level(else retry)?",
                  "Congratulations. You have passed the level.",
                  () -> myPort.write('n'),
                  () -> myPort.write('r'));
        if(data[1]==0)
          new UiBooster().showConfirmDialog(
                  "Do you want to retry(else quit)?",
                  "Bad Luck. You have Lost",
                  () -> myPort.write('r'),
                  () -> exit());
        if(data[1]==2)        
          new UiBooster().showConfirmDialog(
                  "Do you want to start again(else quit)?",
                  "Congratulations. You have Won",
                  () ->  myPort.write('n'),
                  () -> exit());
      }
      else if(data[0]==0)
      {
        for(int i=0; i<data.length; i++)
          state[i]=data[i];
      }
    }
    disp();
  }
  
  if(keyPressed)
  {
    //myPort.write(key);
    if(key=='1')
      myPort.write('1');
     if(key=='2')
      myPort.write('2');
     if(key=='3')
      myPort.write('3');
     if(key=='c')
      myPort.write('c'); 
  }
}
