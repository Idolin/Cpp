#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <cmath>

using namespace std;

FILE *output;

double e = 0.0000001; //accuracy
double g = 9.8; //acceleration of gravity
double dt = 0.00001;   //step for time
double windageLinearCoefficient = 0.1;
double windageSquareCoefficient = 0.000001;

struct Vector   //vector
{
    double x, y;

    Vector() : x(0), y(0)
    {}

    Vector(double x, double y) : x(x), y(y)
    {}

    const Vector operator+(const Vector &v) const
    {
        return Vector(this->x + v.x, this->y + v.y);
    }

    const Vector operator-(const Vector &v) const
    {
        return Vector(this->x - v.x, this->y - v.y);
    }

    const Vector operator*(const double k) const
    {
        return Vector(this->x * k, this->y * k);
    }

    const Vector operator*(const int k) const
    {
        return Vector(this->x * k, this->y * k);
    }

    const Vector operator/(const double k) const
    {
        return Vector(this->x / k, this->y / k);
    }
};

const Vector operator*(const int k, const Vector &v)
{
    return Vector(v.x * k, v.y * k);
}

double abs(const Vector &v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

void printCoordinate(const char *description, const Vector &v)  //prints the coordinates in more readable form
{
    fputs(description, output);
    fputs(": ", output);
    fprintf(output, "%lf", v.x);
    fputs(", ", output);
    fprintf(output, "%lf\n", v.y);
}

Vector getCoordinatesWithoutWindage(double velocity, double angle, double time = -1)
{
    double fallTime = 2 * velocity * sin(angle) / g;  //calculating of the time of fall
    if((time < 0) or (time > fallTime))
        time = fallTime;
    double x = velocity * cos(angle) * time;    // x = vx*t;
    double y = velocity * sin(angle) * time - g * time * time / 2;  // y = vy*t-(g*t^2)/2;
    return Vector(x, y);
}

Vector getCoordinatesVerletLinear(double velocity, double angle, double time = -1)
{
    double nowTime = dt;
    Vector rsb(0, 0);
    if((time >= 0) and (dt / 2 - time > 0)) //if time given to calculate for is too small
        return rsb; //then return the starting point
    Vector v(velocity * cos(angle), velocity * sin(angle)); //projections of initial velocity
    Vector r = v * dt;    //second point
    Vector a = -windageLinearCoefficient * v; //acceleration at the starting point
    a.y -= g;
    v = v + a * dt; //velocity at second point
    a = -windageLinearCoefficient * v; //acceleration at the second point
    a.y -= g;
    while((r.y > 0) or ((time > 0) and (nowTime <= time)))  //while point above zero and don't reach given time
    {
        Vector rn = 2 * r - rsb + a * dt * dt;  // r(t+dt) = 2*r(t)-r(t-dt)+a(t)*dt^2;
        v = (rn - rsb) / (2 * dt);  // v(t) = (r(t+dt)-r(t-dt))/(2*dt);
        rsb = r;    //updating r(t-dt) and r(t)
        r = rn;
        a = -windageLinearCoefficient * v;  //updating a(t)
        a.y -= g;
        nowTime += dt;  //updating time
    }
    return r;
}

Vector calculateForTime(Vector &v, double time)
{
    Vector r;
    // x = vx/k*(1-e^(-k*t));
    r.x = v.x / windageLinearCoefficient * (1 - exp(-windageLinearCoefficient * time));
    // y = ((vy+g/k)*(1-e^(-k*t))-g*t)/k;
    r.y = ((v.y + g / windageLinearCoefficient) * (1 - exp(-windageLinearCoefficient * time)) - g * time) /
          windageLinearCoefficient;
    return r;
}

Vector getCoordinatesAccurateLinear(double velocity, double angle, double time = -1)
{
    if(windageLinearCoefficient < e)  //if coefficient too close to zero
        return getCoordinatesWithoutWindage(velocity, angle, time);   //then calcuate as it is zero
    Vector r;
    Vector v(velocity * cos(angle), velocity * sin(angle)); //projections of initial velocity
    if(time >= 0)   //time was given
    {
        r = calculateForTime(v, time);
        if(r.y >= 0)    //if object in the air or just have landed
            return r;   //then return object coordinates
        else    //else
            return getCoordinatesAccurateLinear(velocity, angle);   //return coordinates of landing place
    } else
    {
        double timer, timel, timem;
        timer = v.y / g;
        timel = 0;
        while(calculateForTime(v, timer).y > 0) //looking for some value of time which more then time of landing
            timer *= 1.5;
        timem = timel + (timer - timel) / 2;
        r = calculateForTime(v, timem);
        while(abs(r.y) > e)    //binary search of landing time
        {
            if(r.y > 0)
                timel = timem;
            else
                timer = timem;
            timem = timel + (timer - timel) / 2;
            r = calculateForTime(v, timem);
        }
        return r;
    }
}

Vector getCoordinatesVerletSquare(double velocity, double angle, double time = -1)
{
    double nowTime = dt;
    Vector rsb(0, 0);
    if((dt / 2 - time > 0) and (time >= 0))   //if time given to calculate for is too small
        return rsb; //then return the starting point
    Vector v(velocity * cos(angle), velocity * sin(angle)); //projections of initial velocity
    Vector r = v * dt;  //second point
    Vector a = -abs(v) * v * windageSquareCoefficient;  //acceleration at the starting point
    a.y -= g;
    while((r.y > 0) or ((time > 0) and (nowTime <= time)))  //while point above zero and don't reach given time
    {
        Vector rn = 2 * r - rsb + a * dt * dt;  // r(t+dt) = 2*r(t)-r(t-dt)+a(t)*dt^2;
        v = (rn - rsb) / (2 * dt);  // v(t) = (r(t+dt)-r(t-dt))/(2*dt);
        rsb = r;    //updating r(t-dt) and r(t)
        r = rn;
        a = -abs(v) * v * windageSquareCoefficient; //updating a(t)
        a.y -= g;
        nowTime += dt;  //updating a(t)
    }
    return r;
}

void err(const char *s) //prints the error message and exits
{
    fputs(s, output);
    exit(1);
}

int main2_phys()
{
    double velocity, angle;
    puts("Enter initial velocity (m/s)");
    //scanf("%lf", &velocity);
    velocity = 300;
    if(velocity < 0)    //checking if given velocity less than zero
        err("Initial velocity must be above 0");
    puts("Enter initial angle (0-180 degrees)");
    //scanf("%lf", &angle);
    angle = 10;
    if((angle < 0) or (angle > 180))    //checking if angle in possible range
        err("Initial angle must be from 0 to 180");
    angle = angle / 180 * M_PI; // a = a/180*pi; converting angle from degrees to radians
    output = fopen("Coordinates.txt", "w"); //opening output file
    //counting and printing four values
    printCoordinate("Without windage", getCoordinatesWithoutWindage(velocity, angle));
    printCoordinate("Verlet, linear dependence", getCoordinatesVerletLinear(velocity, angle));
    printCoordinate("Accurate, linear dependence", getCoordinatesAccurateLinear(velocity, angle));
    printCoordinate("Verlet, square dependence", getCoordinatesVerletSquare(velocity, angle));
    fclose(output); //closing files
    return 0;
}
