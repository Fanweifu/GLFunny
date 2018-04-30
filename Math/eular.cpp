#include "eular.h"

#ifndef EPSILON
#define EPSILON 0.001
#endif

Eular::Eular()
{
        pitch = 0;
        heading = 0;
        roll = 0;
}

Eular::Eular(Eular &e)
{
        pitch = e.pitch;
        heading = e.heading;
        roll = e.roll;
}

Eular::~Eular()
{
}


//规范化
void Eular::normalize()
{
        if(heading == 1.57)
        {
                pitch -= roll;
                roll=0;
        }
        else if(heading == -1.57)
        {
                pitch += roll;
                roll=0;
        }
        //角度范围的限制
        if(pitch > 3.14)  (*this).pitch -= 6.28;
    if(pitch <- 3.14) (*this).pitch += 6.28;
        if(roll > 3.14)  (*this).roll -= 6.28;
        if(roll <- 3.14) (*this).roll += 6.28;
        Matrix4 mat;
        if(heading > 1.57||heading <- 1.57) {
                mat = this->toMatrix4();
                (*this) = Eular::toEular(mat);
        }
}

//重载+ - * = ==
//+
Eular Eular::operator+(Eular &e2)
{
        Eular eular;
        eular.setPHR(pitch+e2.pitch,heading+e2.heading,roll+e2.roll);
        return eular;
}
//-
Eular Eular::operator-(Eular &e2)
{
        Eular eular;
        eular.setPHR(pitch-e2.pitch,heading-e2.heading,roll-e2.roll);
        return eular;
}
//*
Eular Eular::operator*(double n)
{
        Eular eular;
        eular.setPHR(pitch*n,heading*n,roll*n);
        return eular;
}
//=
void Eular::operator=(Eular &e2)
{

        (*this).setPHR(e2.pitch,e2.heading,e2.roll);

}
//==
bool Eular::operator==(Eular &e2)
{
        if((pitch == e2.pitch)&&(heading == e2.heading)&&(roll == e2.roll))
                return true;
        else return false;

}

Matrix4 Eular::toMatrix4()
{
        Matrix4 matrix;
        float
        memset(matrix.m,0,sizeof(matrix.m));
        matrix.m[0][0] = cos(pitch)*cos(roll) + sin(pitch)*sin(heading)*sin(roll);
        matrix.m[0][1] = -cos(pitch)*sin(roll) + sin(pitch)*sin(heading)*cos(roll);
        matrix.m[0][2] = sin(pitch)*cos(heading); matrix.m[1][0] = cos(heading)*sin(roll);
        matrix.m[1][1] = cos(heading)*cos(roll); matrix.m[1][2] = -sin(heading);
        matrix.m[2][0] = -sin(pitch)*cos(roll) + cos(pitch)*sin(heading)*sin(roll);
        matrix.m[2][1] = sin(roll)*sin(pitch) + cos(pitch)*sin(heading)*cos(roll);
        matrix.m[2][2] = cos(pitch)*cos(heading);
        matrix.m[3][3] = 1;
        return matrix;
}

CEular79 CEular79::Change2Eular79(CMatrix79 &mar)
{
        CEular79 eular;
        if(mar.m[1][2] == 1|| mar.m[1][2] == -1){
             eular.heading = mar.m[1][2] *1.57;
                 eular.roll = 0;
                 eular.pitch = atan2(mar.m[2][0],mar.m[0][0]);
        }
        else{
        eular.pitch = atan2(mar.m[0][2],mar.m[2][2]);
        eular.heading = asin(-mar.m[1][2]);
        eular.roll = atan2(mar.m[1][0],mar.m[1][1]);
        }
        return eular;
}

