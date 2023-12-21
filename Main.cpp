// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define GLEW_STATIC

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <vector>

using namespace std;

// Constants
float VX = 0, VY = 0;
float ndcX = 0, ndcY = 0;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
float BALL_RADIUS = 0.05f;
float r = 0.003f * BALL_RADIUS / 0.5f;
const float GRAVITY = -0.0000098f;
int n = 5; // no. of balls;
int colour = 0;

bool isDragging = false;
double prevX, prevY;

//Ball structure
struct ball {
    float x, y; //position
    float vx, vy; //velocity
    int c; //colour
    float rad;
};

vector <ball> balls;

void mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Convert mouse position to normalized device coordinates (NDC)
        ndcX = static_cast<float>(xpos) / WINDOW_WIDTH * 2.0f - 1.0f;
        ndcY = 1.0f - static_cast<float>(ypos) / WINDOW_HEIGHT * 2.0f;

        if (ndcX + BALL_RADIUS <= 1.0f && ndcX - BALL_RADIUS >= -1.0f && ndcY + BALL_RADIUS <= 1.0f && ndcY - BALL_RADIUS >= -1.0f) {

            std::srand(static_cast<unsigned int>(std::time(0)));
            const float minVelocity = -0.001f; // Minimum velocity
            const float maxVelocity = 0.005f;
            // Create a new ball at the mouse click position
            ball b;
            b.x = ndcX;
            b.y = ndcY;
            b.vx = static_cast<float>(rand()) / RAND_MAX * (maxVelocity - minVelocity) + minVelocity;
            b.vy = static_cast<float>(rand()) / RAND_MAX * (maxVelocity - minVelocity) + minVelocity;

            b.c = colour;
            b.rad = BALL_RADIUS;

            balls.push_back(b);

            //BALL_RADIUS += 0.005;
            colour++;
            if (colour > 5)
                colour = 0;
        }

    }

}




// Function to draw the ball
void drawBall(float x, float y, float rad) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glBegin(GL_TRIANGLE_FAN);

    for (float angle = 0.0f; angle <= 360.0f; angle += 0.1f) {
        float x_ = rad * cos(angle);
        float y_ = rad * sin(angle);
        glVertex2f(x_, y_);
    }

    glEnd();

    glPopMatrix();
}

// Function to update ball position
void updateBallPosition() {
    for (auto& b : balls) {
        //glBegin(GL_LINE_STRIP);
        glColor3f(1, 1, 1);
        //glVertex2f(0,0);
        b.x += b.vx;
        b.y += b.vy + GRAVITY / 2.0f;

        // Apply gravity to the ball
        b.vy = b.vy + GRAVITY - r * b.vy;
        b.vx = b.vx - r * b.vx;
        //b.vx *= damping;

        // Bounce off the window boundaries
        if (b.x + b.rad >= 1.0f - (-1.0f * 0.5f / 1000) || b.x - b.rad <= -1.0f + -1.0f * 0.5f / 1000) {
            b.vx = -b.vx;
        }

        if (b.y + b.rad >= 1.0f - (-1.0f * 0.5f / 1000) || b.y - b.rad <= -1.0f - 1.0f * 0.5f / 1000) {
            b.vy = -b.vy;
        }

        //glVertex2f(b.x, b.y);
        glEnd();
    }
}

// GLFW error callback function
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
    // Initialize GLFW
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bouncing Ball", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }


    // Set the viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glLineWidth(5.0f);
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Update ball position
        glBegin(GL_LINE_STRIP);

        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
        glEnd();

        glfwSetMouseButtonCallback(window, mouseClickCallback);

        updateBallPosition();

        // Draw the ball
        for (auto& b : balls) {
            if (b.c == 0)
                glColor3f(0.8f, 0.2f, 0.5f);
            else if (b.c == 1)
                glColor3f(0.7f, 0.1f, 0.1f);
            else if (b.c == 2)
                glColor3f(0.2f, 0.2f, 0.7f);
            else if (b.c == 3)
                glColor3f(0.4f, 0.1f, 0.5f);
            else if (b.c == 4)
                glColor3f(0.25f, 0.7f, 0.2f);
            else if (b.c == 5)
                glColor3f(0.6f, 0.7f, 0.5f);
            else {
                glColor3f(0.7f, 0.2f, 0.7f);
            }
            //glVertex2f(b.x, b.y);
            drawBall(b.x, b.y, b.rad);
        }


        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
