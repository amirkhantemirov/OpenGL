#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <cmath>


float vortexX = 0.0f, vortexY = -0.8f;
float vortexRadius = 0.05f;


struct Particle {
    float x, y;
    bool collected = false;
};
std::vector<Particle> particles;


struct Obstacle {
    float x, y, size;
};
std::vector<Obstacle> obstacles;


float speed = 0.01f;
int score = 0;


void initGame() {
    srand(static_cast<unsigned int>(time(0)));


    for (int i = 0; i < 5; ++i) {
        particles.push_back({ (rand() % 200 - 100) / 100.0f, (rand() % 100) / 100.0f });
    }


    for (int i = 0; i < 3; ++i) {
        obstacles.push_back({ (rand() % 200 - 100) / 100.0f, (rand() % 100) / 100.0f, 0.1f });
    }
}


void drawVortex() {
    glColor3f(0.0f, 0.7f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(vortexX, vortexY);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(vortexX + cos(angle) * vortexRadius, vortexY + sin(angle) * vortexRadius);
    }
    glEnd();
}


void drawParticles() {
    glColor3f(1.0f, 1.0f, 0.0f);
    for (const auto& particle : particles) {
        if (!particle.collected) {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(particle.x, particle.y);
            for (int i = 0; i <= 360; i += 10) {
                float angle = i * 3.14159f / 180.0f;
                glVertex2f(particle.x + cos(angle) * 0.02f, particle.y + sin(angle) * 0.02f);
            }
            glEnd();
        }
    }
}

void drawObstacles() {
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const auto& obstacle : obstacles) {
        glBegin(GL_QUADS);
        glVertex2f(obstacle.x - obstacle.size, obstacle.y - obstacle.size);
        glVertex2f(obstacle.x + obstacle.size, obstacle.y - obstacle.size);
        glVertex2f(obstacle.x + obstacle.size, obstacle.y + obstacle.size);
        glVertex2f(obstacle.x - obstacle.size, obstacle.y + obstacle.size);
        glEnd();
    }
}



void checkCollisions() {

    for (auto& particle : particles) {
        if (!particle.collected &&
            sqrt(pow(vortexX - particle.x, 2) + pow(vortexY - particle.y, 2)) < vortexRadius) {
            particle.collected = true;
            score++;
        }
    }


    for (const auto& obstacle : obstacles) {
        if (sqrt(pow(vortexX - obstacle.x, 2) + pow(vortexY - obstacle.y, 2)) < vortexRadius + obstacle.size) {
            exit(0);
        }
    }
}

void updateObjects() {
    for (auto& particle : particles) {
        particle.y -= speed;
        if (particle.y < -1.0f) {
            particle.y = 1.0f;
            particle.x = (rand() % 200 - 100) / 100.0f;
            particle.collected = false;
        }
    }

    for (auto& obstacle : obstacles) {
        obstacle.y -= speed;
        if (obstacle.y < -1.0f) {
            obstacle.y = 1.0f;
            obstacle.x = (rand() % 200 - 100) / 100.0f;
        }
    }

    checkCollisions();
}
void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.9f);
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawVortex();
    drawParticles();
    drawObstacles();
    drawScore();
    glutSwapBuffers();
}



void keyboard(int key, int, int) {
    if (key == GLUT_KEY_LEFT && vortexX > -1.0f) vortexX -= 0.05f;
    if (key == GLUT_KEY_RIGHT && vortexX < 1.0f) vortexX += 0.05f;
    if (key == GLUT_KEY_UP && vortexY < 1.0f) vortexY += 0.05f;
    if (key == GLUT_KEY_DOWN && vortexY > -1.0f) vortexY -= 0.05f;
}


void timer(int) {
    updateObjects();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Энергетический вихрь");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    initGame();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;

}