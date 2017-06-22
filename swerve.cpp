#include <stdio.h>
#include <math.h>

struct WheelVector {
    float angle;
    float speed;
};

class Swerve{
    public:
        Swerve(float width, float length, float maxSpeed, float maxStick, float tune);
        void calculateWheelVector(float (&joystick)[3], WheelVector (&vectors)[4]);
    private:
        float width;
        float length;
        float maxSpeed;
        float tune;
        float maxStick;
};

Swerve::Swerve(float width, float length, float maxSpeed, float maxStick, float tune) :
    width(width), length(length), maxSpeed(maxSpeed), maxStick(maxStick), tune(tune){
}

void Swerve::calculateWheelVector(float (&joystick)[3], WheelVector (&vectors)[4]){
    float r = pow(pow(this->length, 2.0) + pow(this->width, 2.0), 0.5);
    float a = joystick[1] - joystick[2] * (this->length / r);
    float b = joystick[1] + joystick[2] * (this->length / r);
    float c = joystick[0] - joystick[2] * (this->width / r);
    float d = joystick[0] + joystick[2] * (this->width / r);
    float maxWs;
    
    vectors[0].speed = pow((pow(b, 2.0) + pow(c, 2.0)), 0.5);
    maxWs = vectors[0].speed;
    vectors[1].speed = pow((pow(b, 2.0) + pow(d, 2.0)), 0.5);
    maxWs = vectors[1].speed > vectors[1].speed ? vectors[1].speed : maxWs;
    vectors[2].speed = pow((pow(a, 2.0) + pow(d, 2.0)), 0.5);
    maxWs = vectors[2].speed > maxWs ? vectors[2].speed : maxWs;
    vectors[3].speed = pow((pow(a, 2.0) + pow(c, 2.0)), 0.5);
    maxWs = vectors[3].speed > maxWs ? vectors[3].speed : maxWs;

    vectors[0].speed = maxWs > 1 ? vectors[0].speed / maxWs : vectors[0].speed;
    vectors[1].speed = maxWs > 1 ? vectors[1].speed / maxWs : vectors[1].speed;
    vectors[2].speed = maxWs > 1 ? vectors[2].speed / maxWs : vectors[2].speed;
    vectors[3].speed = maxWs > 1 ? vectors[3].speed / maxWs : vectors[3].speed;

    vectors[0].angle = (c==0 && b==0) ? 0.0 : (atan2(b, c) * 180 / M_PI);
    vectors[1].angle = (d==0 && b==0) ? 0.0 : (atan2(b, d) * 180 / M_PI);
    vectors[2].angle = (d==0 && a==0) ? 0.0 : (atan2(a, d) * 180 / M_PI);
    vectors[3].angle = (c==0 && a==0) ? 0.0 : (atan2(a, c) * 180 / M_PI);
}

int main(){
    Swerve s(40.0, 60.0, 100.0, 1.0, .5);
    WheelVector v[4];
     
    float joystick[3] = {1.0,1.0,0.0};

    s.calculateWheelVector(joystick, v);
    int i;
    for(i=0;i<4;i++){
        printf("w[%d] = (%f, %f)\n", i, v[i].angle, v[i].speed);
    }
}
