#include <MPU6050_tockn.h>
#include <Wire.h>

// Define a set of parameters that manipulate the entire working of the game itself (level means starting level)
int lim = 6;
int updaterate = 5;
float scalea = 1;
float scalev = 2;
int level = 1;
bool cheatMode = false;


// Define the struct that are used to create the Game Elements
typedef struct Wall{
    int startx;
    int starty;
    int endx;
    int endy;
      bool vert;
}Wall;
 
typedef struct Ball{
    float cenx;
    float ceny;
    int r;
    float vx;
    float vy; 
}Ball;

typedef struct Hole{
    int x;
    int y;
    int r;
}Hole;

//Update the ball position depending on the acceleration(x & y)
void Ball_update(Ball &b, float x, float y)
{
    b.vx+=x/scalea;
    b.vy+=y/scalea;
    if(b.vx<-lim)
        b.vx=-lim;
    if(b.vy<-lim)
        b.vy=-lim;
    if(b.vx>lim)
        b.vx=lim;
    if(b.vy>lim)
        b.vy=lim;

    b.cenx+=b.vx/scalev;
    b.ceny+=b.vy/scalev;
}  
 

 //Check if the ball reaches the Goal (Hard condition is the 2 circles intersecting)
bool checkGoal(Ball ball, Hole goal){

    if(((long)ball.cenx-goal.x)*((long)ball.cenx-goal.x)+((long)ball.ceny-goal.y)*((long)ball.ceny-goal.y)< ((long)goal.r+ball.r)*((long)goal.r+ball.r) ){
       return true;
    }
    else
       return false;
}

//Check if the ball touches the hole  (Hard condition is the 2 circles intersecting)
bool checkHole(Ball ball, Hole hole){
    if(((long)ball.cenx-hole.x)*((long)ball.cenx-hole.x) + ((long)ball.ceny-hole.y)*((long)ball.ceny-hole.y) < ((long)hole.r+ball.r)*((long)hole.r+ball.r)){
        return true;
    }
    else
        return false;
}
 
//Check if the ball crosses the wall. If it does then move the ball to just touching wall
void checkWall(Ball &ball, Wall w){
    float sc = 0;
    if(w.vert)
    {
        if(ball.ceny+ball.r>w.starty && ball.ceny-ball.r<w.endy && (ball.cenx + ball.r>=w.startx && ball.cenx - ball.r <= w.startx))
        {
            if(ball.cenx>=w.startx)
            {
                ball.cenx = w.startx+ball.r;//+2;
                ball.vx=-ball.vx*sc;///30;
            }
            else 
            {
                ball.cenx = w.startx-ball.r;//-2;
                ball.vx=-ball.vx*sc;///30;
            }
        }
    }
    else
    {
        if(ball.cenx+ball.r>w.startx && ball.cenx-ball.r<w.endx && (ball.ceny + ball.r >= w.starty && ball.ceny - ball.r <= w.starty))
        {
            if(ball.ceny>=w.starty)
            {
                ball.ceny = w.starty+ball.r;//+2;
                ball.vy=-ball.vy*sc;///30;
            }
            else 
            {
                ball.ceny = w.starty-ball.r;//-2;
                ball.vy=-ball.vy*sc;///30;
            }
        }
    }
 
}

//Send all display data as a long list of integers to the Serial Port
void display2(Ball b, Hole g, Wall w[], unsigned int wlen, Hole h[], unsigned int hlen){
    Serial.print("0 ");
    Serial.print(wlen); Serial.print(" ");
    Serial.print(hlen); Serial.print(" ");
    
    Serial.print((int)b.cenx); Serial.print(" ");
    Serial.print((int)b.ceny); Serial.print(" ");
    Serial.print(b.r); Serial.print(" "); 

    Serial.print(g.x); Serial.print(" ");
    Serial.print(g.y); Serial.print(" ");
    Serial.print(g.r); Serial.print(" "); 

    for(unsigned int i=0; i<hlen; i++){
        Serial.print(h[i].x); Serial.print(" ");
        Serial.print(h[i].y); Serial.print(" ");
        Serial.print(h[i].r); Serial.print(" ");
    }

    for(unsigned int i=0; i<wlen; i++){
        Serial.print(w[i].startx); Serial.print(" ");
        Serial.print(w[i].starty); Serial.print(" ");
        Serial.print(w[i].endx); Serial.print(" ");
        Serial.print(w[i].endy); Serial.print(" ");
    }

    Serial.println("");
}

//Initialize the game elements
Hole g;
Ball b;
int hlen = 0;
int wlen = 0;
Hole h[27];
Wall w[20];

//Update the game elements depending on the level
void levelchange(int l){
    if(l==1){
        // Level 1
        b = (Ball){25, 50, 10, 0, 3};
        g = (Hole){140, 225, 15};
        h[0] = (Hole){275, 150, 20};
        h[1] = (Hole){25, 250, 20};
        h[2] = (Hole){160, 100, 20};
        h[3] = (Hole){85, 260, 15};

        w[0] = (Wall){0, 0, 0, 300, 1};
        w[1] = (Wall){300, 0, 300, 300, 1};
        w[2] = (Wall){0, 0, 300, 0, 0};
        w[3] = (Wall){0, 300, 300, 300, 0};
        w[4] = (Wall){0, 75, 200, 75, 0};
        w[5] = (Wall){50, 150, 50, 300, 1};
        w[6] = (Wall){200, 75, 200, 150, 1};
        w[7] = (Wall){100, 100, 100, 200, 1};
        w[8] = (Wall){100, 200, 300, 200, 0};
        w[9] = (Wall){250, 75, 250, 200, 1};
        w[10] = (Wall){100, 200, 100, 250, 1};
        w[11] = (Wall){100, 250, 275, 250, 0};
        w[12] = (Wall){50, 25, 50, 75, 1};
        hlen = 4;
        wlen = 13;
        updaterate = 5;
    }

    else if(l==2){
        // Level 2
        b = (Ball){35, 225, 10};
        g = (Hole){275, 130, 15};
        h[0] = (Hole){125, 280, 15};
        h[1] = (Hole){175, 220, 15};
        h[2] = (Hole){275, 275, 15};
        h[3] = (Hole){150, 185, 12};
        h[4] = (Hole){20, 125, 15};
        h[5] = (Hole){75, 60, 15};
        h[6] = (Hole){150, 75, 15};
        h[7] = (Hole){210, 135, 10};
        h[8] = (Hole){280, 70, 15};

        w[0] = (Wall){0, 0, 0, 300, 1};
        w[1] = (Wall){300, 0, 300, 300, 1};
        w[2] = (Wall){0, 0, 300, 0, 0};
        w[3] = (Wall){0, 300, 300, 300, 0};
        w[4] = (Wall){0, 200, 225, 200, 0};
        w[5] = (Wall){50, 200, 50, 275, 1};
        w[6] = (Wall){75, 150, 300, 150, 0};
        w[7] = (Wall){75, 80, 75, 150, 1};
        w[8] = (Wall){150, 0, 150, 60, 1};
        w[9] = (Wall){225, 60, 225, 150, 1};
        hlen = 9;
        wlen = 10;
        updaterate = 5;
    }

    else if(l==3){
        // Level 3

        b = (Ball){215, 160, 7};
        g = (Hole){212, 210, 6};

        h[0] = (Hole){150, 140, 7};
        h[1] = (Hole){213, 115, 5};
        h[2] = (Hole){210, 90, 10};
        h[3] = (Hole){175, 85, 5};
        h[4] = (Hole){150, 82, 5};
        h[5] = (Hole){130, 85, 5};
        h[6] = (Hole){90, 90, 12};
        h[7] = (Hole){85, 130, 7};
        h[8] = (Hole){100, 200, 15};
        h[9] = (Hole){260, 115, 10};
        h[10] = (Hole){235, 60, 10};
        h[11] = (Hole){165, 10, 10};
        h[12] = (Hole){165, 65, 10};
        h[13] = (Hole){145, 10, 10};
        h[14] = (Hole){145, 65, 10};
        h[15] = (Hole){35, 10, 10};
        h[16] = (Hole){35, 80, 10};
        h[17] = (Hole){10, 137, 10};
        h[18] = (Hole){65, 210, 10};
        h[19] = (Hole){70, 240, 10};
        h[20] = (Hole){100, 290, 10};
        h[21] = (Hole){130, 240, 10};
        h[22] = (Hole){160, 290, 10};
        h[23] = (Hole){190, 240, 10};
        h[24] = (Hole){220, 290, 10};
        h[25] = (Hole){250, 240, 10};
        h[26]= (Hole){280, 290, 10};
        w[0] = (Wall){0, 0, 0, 300, 1};
        w[1] = (Wall){300, 0, 300, 300, 1};
        w[2] = (Wall){0, 0, 300, 0, 0};
        w[3] = (Wall){0, 300, 300, 300, 0};
        w[4] = (Wall){200, 150, 222, 150, 0};
        w[5] = (Wall){150, 150, 150, 172, 1};
        w[6] = (Wall){150, 175, 272, 175, 0};
        w[7] = (Wall){225, 75, 225, 175, 1};
        w[8] = (Wall){75, 75, 225, 75, 0};
        w[9] = (Wall){75, 75, 75, 95, 1};
        w[10] = (Wall){30, 225, 200, 225, 0};
        w[11] = (Wall){230, 225, 300, 225, 0};
        w[12] = (Wall){275, 150, 275, 175, 1};
        w[13] = (Wall){250, 100, 300, 100, 0};
        w[14] = (Wall){85, 30, 85, 75, 1};
        w[15] = (Wall){0, 150, 50, 150, 0};
        w[16] = (Wall){193, 200, 232, 200, 0};
        w[17] = (Wall){190, 200, 190, 225, 1};
        w[18] = (Wall){235, 200, 235, 225, 1};
        w[19] = (Wall){75, 130, 75, 225, 1};

        hlen = 27;
        wlen = 20;
        updaterate = 3;
    }
}

//Create animation that after level ends ball moves to centre of respective goal or hole
void ending(Ball &b, Hole hol)
{
    int dx = hol.x-b.cenx, dy = hol.y-b.ceny;
    int sx = b.cenx, sy=b.ceny;
    for(int j=0; j<8; j++)
    {
        b.cenx = sx+(double) dx*j/8;
        b.ceny = sy+(double) dy*j/8;
        display2(b, g, w, wlen, h, hlen);
        delay(5);
    }
}

//Initialize the MPU6050 communincation and define global variables
MPU6050 mpu6050(Wire);
double accx = 0;
double accy = 0;
unsigned int n = 0;
char val;

//We initialize the Serial(comm. with laptop) and Wire(comm. with MPU6050)
//Also start the gyroscope and calibrate it. At end we change the level 
//and send coded message to signal begining of gaming loop
void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
    Serial.println();
    Serial.println("TheWorldEatingPythonIsBack");
    levelchange(level);
}

//Gaming loop begins
void loop() {
    //Increment the n->no. of loops before display update
    //Also calculate the acceleration values that the ball follows
    n+=1;
    mpu6050.update();
    accx = 10*sin(2*3.141*mpu6050.getAngleX()/360.0)+0.2;
    accy = 10*sin(2*3.141*mpu6050.getAngleY()/360.0)+1.76;

    //Conduct check if Display device sends any information. If it does do the corresponding action
    if (Serial.available()) 
    { // If data is available to read,
        val = Serial.read(); // read it and store it in val
        //For 1, 2 & 3 change the level to that number
        if(val=='1')
        {
            level=1;
            levelchange(level);
        }
        if(val=='2')
        {
            level=2;
            levelchange(level);
        }
        if(val=='3')
        {
            level=3;
            levelchange(level);
        }
        //Toggle cheat mode -> Ball passes through all walls and is impervious to holes
        if(val=='c')
        {
            cheatMode = !cheatMode;
        }
        //Clear the Serial buffer
        while(Serial.available())
            Serial.read();
    }

    //Update the game parameters after every updaterate no of loops take place
    if(n%updaterate==0)
    {
        //Update the ball position
        Ball_update(b, accy, accx);

        //Check if goal reached
        if(checkGoal(b,g)){
            ending(b, g);

            //Send output denoting win
            if(level==3)
                Serial.println("1 2");
            else
                Serial.println("1 1");
            
            //Wait for response from Display device. Depending on it restart or change level
            while(!Serial.available()){}

            if (Serial.available()) 
            { // If data is available to read,
                val = Serial.read(); // read it and store it in val
            }
            if (val == 'n')
                level = (level)%3+1;
            levelchange(level);
        }

        //Check if any hole is reached
        for (int i=0; i<hlen && !cheatMode; i++){
            if(checkHole(b, h[i])){
                ending(b, h[i]);   
                //Send loss result to display device
                Serial.println("1 0");
                //Wait for response
                while(!Serial.available()){}
                //If response comes then restart level
                if (Serial.available()) 
                { // If data is available to read,
                    val = Serial.read(); // read it and store it in val
                }
                levelchange(level);
            }
        }

        //Check and update ball for all walls
        for(int i=0; i<wlen && !cheatMode; i++){
            checkWall(b, w[i]);
        }
            
        display2(b, g, w, wlen, h, hlen);

        //For debugging purposes:
        //Serial.print(accx); Serial.print(" ");Serial.print(accy); Serial.print(" ");Serial.print(b.vx); Serial.print(" ");Serial.print(b.vy); Serial.print(" ");Serial.print(b.cenx); Serial.print(" ");Serial.println(b.ceny);
        n=0; //Reset the count of no of loops after last game update
    }
}
