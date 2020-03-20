#ifndef ASCIISTEROIDS_H
#define ASCIISTEROIDS_H

void init();
double randNum(double max);
double fixRotation(double deg);
double sinDeg(double deg);
double cosDeg(double deg);
double atanDeg(double ratio);
int join3(double* ptr1, int siz1, double* ptr2, int siz2, double* ptr3, int siz3, double* finalPtr, int finalSiz);
void tickShip(double x, double y, double x_vel, double y_vel, double rotation, double acceleration, double friction, double size);
void move(double x, double y, double x_vel, double y_vel, double rotation, double friction, double size);
int getShipLines(double x, double y, double rotation, double size);
void addLaser(double x, double y, double x_vel, double y_vel, double rotation);
void removeLaser(int index);
void tickLasers(double size);
void getLaserLines();
void addAsteroid(double x, double y, double rotation, double size, double speed);
void removeAsteroid(int index);
void splitAsteroid(int index);
void tickAsteroids(double size);
void getAsteroidLines();
int main();

#endif