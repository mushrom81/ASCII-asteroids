#include <time.h>
#include <stdlib.h>
#include <stdio.h> // DEBUG
#include <math.h>
#include <windows.h>
#include "fastTerminalCanvas.h"
#include "getKey.h"
#include "ASCIIsteroids.h"

const int CANVAS_WIDTH = 170;
const int CANVAS_HEIGHT = 100;

double *G_printingPointer;
double G_printingPointer_length;

double *GETSHIPLINES_RETURN;
double *MOVE_RETURN;
double *GETLASERLINES_RETURN;
double *JOIN_RETURN;
int GETASTEROIDLINES_RETURN_size = 0;
double *GETASTEROIDLINES_RETURN = NULL;

int G_shipLaserCount = 0;
double **G_shipLasers;

int G_asteroidCount = 0;
int G_asteroidsSize = 0;
double **G_asteroids = NULL;

int G_keyKpressed = 0;

HANDLE hConsole;

void init() {
    srand(time(NULL));
    MOVE_RETURN = malloc(5 * sizeof(double));
    GETSHIPLINES_RETURN = malloc(5 *4 * sizeof(double));
    GETLASERLINES_RETURN = malloc(4 * 4 * sizeof(double));
    G_shipLasers = malloc(4 * sizeof(double*));
    for (int i = 0; i < 4; i++) {
        G_shipLasers[i] = malloc(5 * sizeof(double));
    };
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 23);
};

double randNum(double max) {
    return rand() * max / RAND_MAX;
};

double fixRotation(double deg) {
    if (deg < 0) { deg += 360; };
    if (deg >= 360) { deg -= 360; };
    return deg;
};

double sinDeg(double deg) {
    return sin(deg * M_PI / 180);
};

double cosDeg(double deg) {
    return cos(deg * M_PI / 180);
};

double atanDeg(double ratio) {
    return atan(ratio) * 180.0 / M_PI;
};

int join(double* ptr1, int siz1, double* ptr2, int siz2, double* ptr3, int siz3, double* finalPtr, int finalSiz) {
    if (siz1 + siz2 + siz3 > finalSiz) {
        free(finalPtr);
        finalPtr = malloc((siz1 + siz2 + siz3) * sizeof(double));
    };
    for (int i = 0; i < siz1; i++) {
        finalPtr[i] = ptr1[i];
    };
    for (int i = 0; i < siz2; i++) {
        finalPtr[siz1 + i] = ptr2[i];
    };
    for (int i = 0; i < siz3; i++) {
        finalPtr[siz1 + siz2 + i] = ptr3[i];
    };
    JOIN_RETURN = finalPtr;
    return finalSiz;
}; // Returns size of joined array
// Return var points to joined arrays

void tickShip(double x, double y, double x_vel, double y_vel, double rotation, double acceleration, double friction, double size) {
    if (getKey('a')) { rotation = fixRotation(rotation - 5); };
    if (getKey('d')) { rotation = fixRotation(rotation + 5); };
    if (getKey('w')) {
        x_vel += cosDeg(rotation) * acceleration;
        y_vel += sinDeg(rotation) * acceleration;
    };
    if (getKey('k')) {
        if (G_keyKpressed == 0) { addLaser(x, y, x_vel, y_vel, rotation); };
        G_keyKpressed = 1;
    } else {
        G_keyKpressed = 0;
    };
    move(x, y, x_vel, y_vel, rotation, friction, size);
    getAsteroidLines();
    int shipLineCount = getShipLines(x, y, rotation, size);
    for (int i = 0; i < G_asteroidCount * 16; i += 4) {
        for (int j = 0; j < shipLineCount * 4; j += 4) {
            if (lineIntersect(  GETSHIPLINES_RETURN[j], GETSHIPLINES_RETURN[j + 1], GETSHIPLINES_RETURN[j + 2], GETSHIPLINES_RETURN[j + 3],
                                GETASTEROIDLINES_RETURN[i], GETASTEROIDLINES_RETURN[i + 1], GETASTEROIDLINES_RETURN[i + 2], GETASTEROIDLINES_RETURN[i + 3]))
            {
                double gameOver[] = {44.16667175292969, 11, 22.166671752929688, 11, 22.166671752929688, 11, 22.166671752929688, 40, 22.166671752929688, 40, 44.16667175292969, 42, 44.16667175292969, 42, 42.16667175292969, 28, 42.16667175292969, 28, 37.16667175292969, 28, 61.16667175292969, 44, 63.16667175292969, 11, 63.16667175292969, 11, 83.16667175292969, 11, 83.16667175292969, 11, 82.16667175292969, 47, 83.16667175292969, 27, 62.16667175292969, 27, 98.16667175292969, 46, 101.16667175292969, 11, 101.16667175292969, 11, 111.16667175292969, 46, 111.16667175292969, 46, 116.16667175292969, 12, 116.16667175292969, 12, 123.16667175292969, 45, 136.1666717529297, 43, 136.1666717529297, 11, 136.1666717529297, 11, 152.1666717529297, 12, 137.1666717529297, 29, 150.1666717529297, 27, 136.1666717529297, 42, 151.1666717529297, 42, 22.166671752929688, 93, 20.166671752929688, 61, 20.166671752929688, 61, 44.16667175292969, 61, 44.16667175292969, 61, 45.16667175292969, 91, 45.16667175292969, 91, 22.166671752929688, 93, 61.16667175292969, 62, 71.16667175292969, 91, 71.16667175292969, 91, 84.16667175292969, 63, 99.16667175292969, 65, 98.16667175292969, 95, 98.16667175292969, 95, 116.16667175292969, 93, 100.16667175292969, 66, 117.16667175292969, 67, 99.16667175292969, 81, 117.16667175292969, 81, 134.1666717529297, 94, 135.1666717529297, 65, 135.1666717529297, 65, 152.1666717529297, 75, 152.1666717529297, 75, 135.1666717529297, 81, 135.1666717529297, 81, 150.1666717529297, 93};
                int shipLineCount = getShipLines(x, y, rotation, size);
                G_printingPointer_length = join(G_printingPointer, 4 * (shipLineCount + G_shipLaserCount + G_asteroidCount * 4), gameOver, 4 * 31, NULL, 0, NULL, 0);
                printScreen(JOIN_RETURN, (31 + shipLineCount + G_shipLaserCount + G_asteroidCount * 4), 0);
                while (1) { if (getKey('q')) { exit(0); }; };
            };
        };
    };
}; // Calls getAsteroidLines and getShipLines

void move(double x, double y, double x_vel, double y_vel, double rotation, double friction, double size) {
    x += x_vel;
    y += y_vel;
    if (x > CANVAS_WIDTH - 5 * size) { x -= 5 + CANVAS_WIDTH; }
    else if (x < -5 * size) { x += 5 + CANVAS_WIDTH; };
    if (y > CANVAS_HEIGHT - 5 * size) { y -= 5 + CANVAS_HEIGHT; }
    else if (y < -5 * size) { y += 5 + CANVAS_HEIGHT; };
    MOVE_RETURN[0] = x;
    MOVE_RETURN[1] = y;
    MOVE_RETURN[2] = x_vel * friction;
    MOVE_RETURN[3] = y_vel * friction;
    MOVE_RETURN[4] = rotation;
}; // Return var contains {x, y, x_vel, y_vel, rotation}

int getShipLines(double x, double y, double rotation, double size) {
    double shipPoints[] = {
        x + cosDeg(rotation) * 5 * size,
        y + sinDeg(rotation) * 5 * size,
        x + cosDeg(rotation + 140) * 3 * size,
        y + sinDeg(rotation + 140) * 3 * size,
        x + cosDeg(rotation - 140) * 3 * size,
        y + sinDeg(rotation - 140) * 3 * size,
        x + cosDeg(rotation - 180) * 5 * size,
        y + sinDeg(rotation - 180) * 5 * size
    };
    int shipIndexes[] = {0, 1, 2, 3,  2, 3, 4, 5,  4, 5, 0, 1,  2, 3, 6, 7,  6, 7, 4, 5};
    int lineCount = 3 + (2 * getKey('w'));
    for (int i = 0; i < 4 * lineCount; i++) {
        GETSHIPLINES_RETURN[i] = shipPoints[shipIndexes[i]];
    };
    return lineCount;
}; // Returns number of lines in ship
// Return var contains lines in ship

void addLaser(double x, double y, double x_vel, double y_vel, double rotation) {
    if (G_shipLaserCount < 4) {
        G_shipLasers[G_shipLaserCount][2] = x_vel + cosDeg(rotation) * 2;
        G_shipLasers[G_shipLaserCount][3] = y_vel + sinDeg(rotation) * 2;
        G_shipLasers[G_shipLaserCount][0] = x + G_shipLasers[G_shipLaserCount][2];
        G_shipLasers[G_shipLaserCount][1] = y + G_shipLasers[G_shipLaserCount][3];
        G_shipLasers[G_shipLaserCount][4] = 27;
        G_shipLaserCount++;

    };
};

void removeLaser(int index) {
    G_shipLaserCount--;
    double *temp = G_shipLasers[index];
    G_shipLasers[index] = G_shipLasers[G_shipLaserCount];
    G_shipLasers[G_shipLaserCount] = temp;
};

void tickLasers(double size) {
    double laserCollisions[4];
    getAsteroidLines();
    for (int i = 0; i < G_shipLaserCount; i++) {
        G_shipLasers[i][4]--;
        move(G_shipLasers[i][0], G_shipLasers[i][1], G_shipLasers[i][2], G_shipLasers[i][3], 0, 1, size);
        laserCollisions[0] = G_shipLasers[i][0];
        laserCollisions[1] = G_shipLasers[i][1];
        laserCollisions[2] = MOVE_RETURN[0];
        laserCollisions[3] = MOVE_RETURN[1];
        G_shipLasers[i][0] = MOVE_RETURN[0];
        G_shipLasers[i][1] = MOVE_RETURN[1];
        if (G_shipLasers[i][4] <= 0) {
            removeLaser(i);
            i--;
            continue;
        };
        for (int j = 0; j < G_asteroidCount; j++) {
            for (int k = 0; k < 4; k++) {
                if (lineIntersect(  laserCollisions[0], laserCollisions[1], laserCollisions[2], laserCollisions[3],
                                    GETASTEROIDLINES_RETURN[16 * j + 4 * k], GETASTEROIDLINES_RETURN[16 * j + 4 * k + 1], GETASTEROIDLINES_RETURN[16 * j + 4 * k + 2], GETASTEROIDLINES_RETURN[16 * j + 4 * k + 3])) {
                    removeLaser(i);
                    i--;
                    splitAsteroid(j);
                    j = G_asteroidCount;
                    k = 4;
                    continue; // STOPPED HERE, FIGURING OUT HOW I CAN GET CONTINUE TO AFFECT THE i LOOP. ALSO NEED TO CALL getAsteroidLines
                };
            };
        };
    };
}; // Calls getAsteroidLines

void getLaserLines() {
    for (int i = 0; i < G_shipLaserCount; i++) {
        GETLASERLINES_RETURN[4 * i] = G_shipLasers[i][0];
        GETLASERLINES_RETURN[4 * i + 1] = G_shipLasers[i][1];
        GETLASERLINES_RETURN[4 * i + 2] = G_shipLasers[i][0] + G_shipLasers[i][2];
        GETLASERLINES_RETURN[4 * i + 3] = G_shipLasers[i][1] + G_shipLasers[i][3];
    };
}; // Returns var contains lines in lasers

void addAsteroid(double x, double y, double rotation, double size, double speed) {
    if (size <= 0) { return; };
    G_asteroidCount++;
    while (G_asteroidCount > G_asteroidsSize) {
        G_asteroidsSize++;
        G_asteroids = realloc(G_asteroids, G_asteroidsSize * sizeof(double*));
        G_asteroids[G_asteroidsSize - 1] = malloc(5 * sizeof(double));
    };
    G_asteroids[G_asteroidCount - 1][0] = x;
    G_asteroids[G_asteroidCount - 1][1] = y;
    G_asteroids[G_asteroidCount - 1][2] = cosDeg(rotation) * speed;
    G_asteroids[G_asteroidCount - 1][3] = sinDeg(rotation) * speed;
    G_asteroids[G_asteroidCount - 1][4] = size + 1;
};

void removeAsteroid(int index) {
    G_asteroidCount--;
    double *temp = G_asteroids[index];
    G_asteroids[index] = G_asteroids[G_asteroidCount];
    G_asteroids[G_asteroidCount] = temp;
};

void splitAsteroid(int index) {
    double x = G_asteroids[index][0] + G_asteroids[index][3];
    double y = G_asteroids[index][1] + G_asteroids[index][4];
    double rotation = atanDeg(G_asteroids[index][3] / G_asteroids[index][2]);
    double speed = sqrt(G_asteroids[index][2] * G_asteroids[index][2] + G_asteroids[index][3] * G_asteroids[index][3]);
    double size = G_asteroids[index][4];
    removeAsteroid(index);
    addAsteroid(x, y, fixRotation(rotation + randNum(60) - 30), size - 2, speed + randNum(1) - 0.3);
    addAsteroid(x, y, fixRotation(rotation + randNum(60) - 30), size - 2, speed + randNum(1) - 0.3);
};

void tickAsteroids(double size) {
    for (int i = 0; i < G_asteroidCount; i++) {
        move(G_asteroids[i][0], G_asteroids[i][1], G_asteroids[i][2], G_asteroids[i][3], 0, 1, size);
        G_asteroids[i][0] = MOVE_RETURN[0];
        G_asteroids[i][1] = MOVE_RETURN[1];
    };
};

void getAsteroidLines() {
    if (GETASTEROIDLINES_RETURN_size < G_asteroidCount * 16) {
        free(GETASTEROIDLINES_RETURN);
        GETASTEROIDLINES_RETURN = malloc(G_asteroidCount * 16 * sizeof(double));
        GETASTEROIDLINES_RETURN_size = G_asteroidCount * 16;
    };
    double asteroidPoints[8];
    int asteroidLinesIndex[] = {0, 1, 2, 3,  2, 3, 4, 5,  4, 5, 6, 7,  6, 7, 0, 1,};
    for (int i = 0; i < G_asteroidCount; i++) {
        asteroidPoints[0] = G_asteroids[i][0];
        asteroidPoints[1] = G_asteroids[i][1];
        asteroidPoints[2] = G_asteroids[i][0] + pow(1.7, G_asteroids[i][4]);
        asteroidPoints[3] = G_asteroids[i][1] + pow(1.7, G_asteroids[i][4]);
        asteroidPoints[4] = G_asteroids[i][0] + 2 * pow(1.7, G_asteroids[i][4]);
        asteroidPoints[5] = G_asteroids[i][1];
        asteroidPoints[6] = G_asteroids[i][0] + pow(1.7, G_asteroids[i][4]);
        asteroidPoints[7] = G_asteroids[i][1] - pow(1.7, G_asteroids[i][4]);
        for (int j = 0; j < 16; j++) {
            GETASTEROIDLINES_RETURN[16 * i + j] = asteroidPoints[asteroidLinesIndex[j]];
        };
    };
}; // Return var contains lines in asteroids

int main() {
    init();
    initCanvas("chars.txt", CANVAS_WIDTH, CANVAS_HEIGHT);
    double ship[] = {CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 0, 0, 0, 0.01, 0.998, 1};
    addAsteroid(0, 0, randNum(360), 3, 0.4);
    addAsteroid(0, CANVAS_HEIGHT, randNum(360), 3, 0.4);
    addAsteroid(CANVAS_WIDTH, 0, randNum(360), 3, 0.4);
    addAsteroid(CANVAS_WIDTH, CANVAS_HEIGHT, randNum(360), 3, 0.4);
    while (1) {
        resizeCanvas(CANVAS_WIDTH, CANVAS_HEIGHT);
        tickShip(ship[0], ship[1], ship[2], ship[3], ship[4], ship[5], ship[6], ship[7]);
        for (int i = 0; i < 5; i++) { ship[i] = MOVE_RETURN[i]; };
        int shipLineCount = getShipLines(ship[0], ship[1], ship[4], ship[7]);
        tickAsteroids(ship[7]);
        tickLasers(ship[7]);
        getAsteroidLines();
        getLaserLines();
        G_printingPointer_length = join(GETSHIPLINES_RETURN, shipLineCount * 4,
                                        GETLASERLINES_RETURN, G_shipLaserCount * 4,
                                        GETASTEROIDLINES_RETURN, G_asteroidCount * 16,
                                        G_printingPointer, G_printingPointer_length);
        G_printingPointer = JOIN_RETURN;
        printScreen(G_printingPointer, shipLineCount + G_shipLaserCount + G_asteroidCount * 4, 20);
    };
};
