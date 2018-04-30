#ifndef EULAR_H
#define EULAR_H

#include "matrices.h"

class Eular
{
public:
        double pitch ,heading, roll;
        Eular();
        ~Eular();

        Eular(Eular &e);

        void setPHR(double x, double y,double z){ pitch =x; heading =y; roll = z; }

        void normalize();

        Eular operator+(Eular &e2);//+
        Eular operator-(Eular &e2);//-
        Eular operator*(double n);//*
        void operator=(Eular &e2);//=
        bool operator==(Eular &e2);//==

        Matrix4 toMatrix4();
        static Eular toEular(Matrix4 &m);
};

#endif // EULAR_H
