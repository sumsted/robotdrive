#include <stdio.h>

#define byte char

class Mecanum {
    public:
        Mecanum(float width, float height, float minPwm, float maxPwm, float maxStick, float tune);
        void calculateWheelSpeed(float (&joystick)[3], int (&wheelSpeed)[4]);
        static float myabs(float val);
        static float myround(float val);
    private:
        float width;
        float length;
        float minPwm;
        float maxPwm;
        float tune;
        float maxStick;
};

Mecanum::Mecanum(float width, float length, float minPwm, float maxPwm, float maxStick, float tune) :
    width(width), length(length), minPwm(minPwm), maxPwm(maxPwm), maxStick(maxStick), tune(tune){
}

void Mecanum::calculateWheelSpeed(float (&joystick)[3], int (&wheelSpeed)[4]){
    float speedRatio[4];
    float maxRatio = this->maxStick;
    byte i;

    speedRatio[0] = joystick[1] - joystick[0] + joystick[2] * ((this->width / 2)+(this->length /2));
    maxRatio = Mecanum::myabs(speedRatio[0]) > maxRatio ? Mecanum::myabs(speedRatio[0]) : maxRatio;
    speedRatio[1] = joystick[1] + joystick[0] - joystick[2] * ((this->width / 2)+(this->length /2));
    maxRatio = Mecanum::myabs(speedRatio[1]) > maxRatio ? Mecanum::myabs(speedRatio[1]) : maxRatio;
    speedRatio[2] = joystick[1] - joystick[0] - joystick[2] * ((this->width / 2)+(this->length /2));
    maxRatio = Mecanum::myabs(speedRatio[2]) > maxRatio ? Mecanum::myabs(speedRatio[2]) : maxRatio;
    speedRatio[3] = joystick[1] + joystick[0] + joystick[2] * ((this->width / 2)+(this->length /2));
    maxRatio = Mecanum::myabs(speedRatio[3]) > maxRatio ? Mecanum::myabs(speedRatio[3]) : maxRatio;

    for(i=0;i<4;i++){
        wheelSpeed[i] = (int) Mecanum::myround(((this->maxPwm - this->minPwm)/2.0*speedRatio[i])/maxRatio * this->tune + ((this->maxPwm + this->minPwm) / 2));
    }
}


float Mecanum::myround(float val){
    return float(val - int(val) >= 0.5 ? int(val)+1 : int(val));
}

float Mecanum::myabs(float val){
    return val < 0.0 ? -val : val;
}

int main(){
    int wheelSpeed[4];

    // wheelbase width and length, controller min and max pwm, max abs stick value, tune percentage of output pwm 
    Mecanum m(40.0, 60.0, 499, 2500, 1.0, 0.5);

    // joystick x, y, rot, values from -1.0 to 1.0 or max stick
    float joystick[3] = {1.0,1.0,01.0};

    m.calculateWheelSpeed(joystick, wheelSpeed);
    int i;
    for(i=0;i<4;i++){
        printf("w[%d] = %d\n", i, wheelSpeed[i]);
    }
}