#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

int y1=90, x1=90;

void setup() {
    Serial.begin(9600);
    Braccio.begin();
        Braccio.ServoMovement(20, 47,  90, 180, 90, 90,  73);  

}

void loop() {
  
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        int commaIndex = data.indexOf(',');
        int x = data.substring(0, commaIndex).toInt();
        int y = data.substring(commaIndex + 1).toInt();
        Serial.print("Received X: ");
        Serial.print(x);
        Serial.print(" Y: ");
        Serial.println(y);
        
        if(y>150)
          y1 +=1;
          
        if(y<70)
          y1 -=1;
        
        if(x>300)
          x1 += 1;
  
        if(x<200)
          x1 -= 1;
        


//        
//        Serial.print("Received X1: ");
//        Serial.print(x1);
//        Serial.print(" Y1: ");
//        Serial.println(y1);
        Braccio.ServoMovement(20,x1,  y1, 180, 90, 90,  73);  

    }
}
