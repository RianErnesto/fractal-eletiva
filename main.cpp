#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_LENGTH 10
#define MAX_PARTICLES_IN_SCREEN 15

using namespace std;

class Particle
{
public:
    static char character;
    int x;
    int y;

    Particle()
    {
        this->x = 0;
        this->y = 0;
    }

    Particle(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    bool isParticleOutOfMatrix(char characterInPosition) {
        return characterInPosition != '*';
    }
};

class Matrix
{
public:
    char matrix[MAX_LENGTH][MAX_LENGTH];

    Matrix(char character = ' ')
    {
        fillMatrix(character);
    }

    Matrix(char matrix[][MAX_LENGTH])
    {
        int i, j;
        for (i = 0; i < MAX_LENGTH; i++)
        {
            for (j = 0; j < MAX_LENGTH; j++)
            {
                this->matrix[i][j] = matrix[i][j];
            }
        }
    }

    void fillMatrix(char character)
    {
        int i, j;
        for (i = 0; i < MAX_LENGTH; i++)
        {
            for (j = 0; j < MAX_LENGTH; j++)
            {
                this->matrix[i][j] = character;
            }
        }
    }

    void printMatrix()
    {
        int i, j;
        for (i = 0; i < MAX_LENGTH; i++)
        {
            for (j = 0; j < MAX_LENGTH; j++)
            {
                cout << this->matrix[i][j];
            }
            cout << endl;
        }
    }

    void changeCharacterInPosition(int x, int y, char character)
    {
        this->matrix[x][y] = character;
    }

    bool isNextPositionParticle(int x, int y)
    {
        return this->matrix[x][y] == Particle::character;
    }

    bool isNextPositionOutMatrix(int x, int y)
    {
        return (x < 0 || x >= MAX_LENGTH || y < 0 || y >= MAX_LENGTH);
    }

    bool isNextPositionAvailable(int x, int y) {
        return !this->isNextPositionOutMatrix(x,y) && !this->isNextPositionParticle(x,y);
    }
};

char Particle::character = '*';

void spawnMovingParticleInMatrix(Matrix *matrix, Particle *particle)
{
    int yPosition = rand() % MAX_LENGTH;
    particle->y = yPosition;
    particle->x = 0;
    matrix->changeCharacterInPosition(0, yPosition, Particle::character);
}

void spawnFixedParticlesInMatrix(Matrix *matrix, int *currentQuantityOfParticlesInScreen)
{
    double quantityParticles = (MAX_LENGTH * 10) / 100;
    *currentQuantityOfParticlesInScreen += quantityParticles;
    int positionParticle = ceil(MAX_LENGTH / (quantityParticles + 1));
    int gapPositions = positionParticle;

    int i;
    for (i = 0; i < quantityParticles; i++, positionParticle += gapPositions)
    {
        matrix->changeCharacterInPosition(MAX_LENGTH - 1, positionParticle, Particle::character);
    }
}

bool isParticleNear(Matrix matrix, Particle particle) {
    return (matrix.isNextPositionParticle(particle.x + 1, particle.y) 
            || matrix.isNextPositionParticle(particle.x - 1, particle.y)
            || matrix.isNextPositionParticle(particle.x, particle.y + 1)
            || matrix.isNextPositionParticle(particle.x, particle.y - 1));
}

void moveParticle(Matrix *matrix, Particle *particle)
{
    // 0 - UP
    // 1 - RIGHT
    // 2 - DOWN
    // 3 - LEFT
    while (true)
    {
        int direction = rand() % 4;
        switch (direction)
        {
            // UP
        case 0:
            if(matrix->isNextPositionOutMatrix(particle->x - 1, particle->y)) {
                matrix->changeCharacterInPosition(particle->x, particle->y, ' ');
                return;
            }
            if(matrix->isNextPositionParticle(particle->x - 1, particle->y)) 
                return;
            matrix->changeCharacterInPosition(particle->x, particle->y, ' '); 
            matrix->changeCharacterInPosition(particle->x - 1, particle->y, '*');
            particle->x--; 
            return;
            // RIGHT
        case 1:
            if(matrix->isNextPositionOutMatrix(particle->x, particle->y + 1)) {
                matrix->changeCharacterInPosition(particle->x, particle->y, ' ');
                return;
            }
            if(matrix->isNextPositionParticle(particle->x, particle->y + 1)) 
                return;
            matrix->changeCharacterInPosition(particle->x, particle->y, ' '); 
            matrix->changeCharacterInPosition(particle->x, particle->y + 1, '*'); 
            particle->y++;
            return;
            // DOWN
        case 2:
            if(matrix->isNextPositionOutMatrix(particle->x + 1, particle->y)) {
                matrix->changeCharacterInPosition(particle->x, particle->y, ' ');
                return;
            }
            if(matrix->isNextPositionParticle(particle->x + 1, particle->y)) 
                return;
            matrix->changeCharacterInPosition(particle->x, particle->y, ' '); 
            matrix->changeCharacterInPosition(particle->x + 1, particle->y, '*'); 
            particle->x++;
            return;
            // LEFT
        case 3:
            if(matrix->isNextPositionOutMatrix(particle->x, particle->y - 1)) {
                matrix->changeCharacterInPosition(particle->x, particle->y, ' ');
                return;
            }
            if(matrix->isNextPositionParticle(particle->x, particle->y - 1)) 
                return;
            matrix->changeCharacterInPosition(particle->x, particle->y, ' '); 
            matrix->changeCharacterInPosition(particle->x, particle->y - 1, '*');
            particle->y--;
            return;
        }
    }
}

bool isEndGame(Matrix matrix, Particle particle) {
    return matrix.isNextPositionParticle(particle.x++, particle.y);
}

int main()
{
    srand(time(NULL));
    Matrix matrix;
    Particle particle;
    int currentQuantityParticlesInScreen = 0;

    spawnMovingParticleInMatrix(&matrix, &particle);
    currentQuantityParticlesInScreen++;
    spawnFixedParticlesInMatrix(&matrix, &currentQuantityParticlesInScreen);

    while(true) {
        matrix.printMatrix();
        moveParticle(&matrix, &particle);
        system("cls || clear");
        if(particle.isParticleOutOfMatrix(matrix.matrix[particle.x][particle.y])) {
            spawnMovingParticleInMatrix(&matrix, &particle);
            continue;
        }
        if(isParticleNear(matrix, particle)) {
            if(currentQuantityParticlesInScreen >= MAX_PARTICLES_IN_SCREEN)
                return 0;
            spawnMovingParticleInMatrix(&matrix, &particle);
            currentQuantityParticlesInScreen++;
            continue;
        }
        // if(isEndGame(matrix, particle)) {
        //     cout << "\n\nCongratulations !!";
        //     return 0;
        // }
    }


    return 0;
}