#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

const int BOARD_WIDTH = 15;
const int BOARD_HEIGHT = 20;

const float BLOCK_SIZE = (float)WINDOW_WIDTH / BOARD_WIDTH;
int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

int totalClearedLines = 0;

//the available shapes 
const int tetrominoes[7][16] = {
    // I
    {
        0, 0, 0, 0,
        1, 1, 1, 1,
        0, 0, 0, 0,
        0, 0, 0, 0},
    // O
    {
        0, 0, 0, 0,
        0, 2, 2, 0,
        0, 2, 2, 0,
        0, 0, 0, 0},
    // T
    {
        0, 0, 0, 0,
        3, 3, 3, 0,
        0, 3, 0, 0,
        0, 0, 0, 0},
    // S
    {
        0, 0, 0, 0,
        0, 4, 4, 0,
        4, 4, 0, 0,
        0, 0, 0, 0},
    // Z
    {
        0, 0, 0, 0,
        5, 5, 0, 0,
        0, 5, 5, 0,
        0, 0, 0, 0},
    // J
    {
        0, 0, 0, 0,
        6, 0, 0, 0,
        6, 6, 6, 0,
        0, 0, 0, 0},
    // L
    {
        0, 0, 0, 0,
        0, 0, 7, 0,
        7, 7, 7, 0,
        0, 0, 0, 0}};

float colors[8][4] = {
    {0, 0, 0, 1},
    {0.0f, 1.0f, 1.0f, 1}, // 1 = Cyan (I)
    {1.0f, 1.0f, 0.0f, 1}, // 2 = Yellow (O)
    {0.6f, 0.0f, 1.0f, 1}, // 3 = Purple (T)
    {0.0f, 1.0f, 0.0f, 1}, // 4 = Green (S)
    {1.0f, 0.0f, 0.0f, 1}, // 5 = Red (Z)
    {0.0f, 0.0f, 1.0f, 1}, // 6 = Blue (J)
    {1.0f, 0.5f, 0.0f, 1}  // 7 = Orange (L)
};

// the properties of each piece
struct Piece
{
    int x, y;
    int type;
    int rotation;
};

Piece currentPiece;

int rotate(int x, int y, int rotation)
{
    switch (rotation % 4)
    {
    case 0:
        return y * 4 + x; // 0 degrees
    case 1:
        return 12 + y - (x * 4); // 90 degrees
    case 2:
        return 15 - (y * 4) - x; // 180 degrees
    case 3:
        return 3 - y + (x * 4); // 270 degrees
    }
    return 0;
}

//checkes if the piece fits
bool doesPieceFit(const Piece &p)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int index = rotate(px, py, p.rotation);
            int block = tetrominoes[p.type][index];
            if (block != 0)
            {
                int boardX = p.x + px;
                int boardY = p.y + py;

                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT)
                    return false;

                if (board[boardY][boardX] != 0)
                    return false;
            }
        }
    }
    return true;
}

void lockPiece(const Piece &p)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int index = rotate(px, py, p.rotation);
            int block = tetrominoes[p.type][index];
            if (block != 0)
            {
                int boardX = p.x + px;
                int boardY = p.y + py;
                if (boardY >= 0 && boardY < BOARD_HEIGHT && boardX >= 0 && boardX < BOARD_WIDTH)
                {
                    board[boardY][boardX] = block;
                }
            }
        }
    }
}


{
    int linesCleared = 0;
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (board[y][x] == 0)
            {
                full = false;
                break;
            }
        }
        if (full)
        {
            linesCleared++;

            for (int row = y; row > 0; row--)
            {
                for (int col = 0; col < BOARD_WIDTH; col++)
                {
                    board[row][col] = board[row - 1][col];
                }
            }
            for (int col = 0; col < BOARD_WIDTH; col++)
                board[0][col] = 0;

            y--;
        }
    }
    return linesCleared;
}

void drawBlock(int x, int y, float r, float g, float b)
{
    float xf = x * BLOCK_SIZE;
    float yf = y * BLOCK_SIZE;

    float x1 = (float)x / BOARD_WIDTH * 2 - 1;
    float y1 = 1 - yf / WINDOW_HEIGHT * 2;
    float x2 = ((float)x + 1) / BOARD_WIDTH * 2 - 1;
    float y2 = 1 - (yf + BLOCK_SIZE) / WINDOW_HEIGHT * 2;

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    glColor3f(0, 0, 0);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawBoard()
{
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            int block = board[y][x];
            if (block != 0)
            {
                float *c = colors[block];
                drawBlock(x, y, c[0], c[1], c[2]);
            }
        }
    }
}

void drawPiece(const Piece &p)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int index = rotate(px, py, p.rotation);
            int block = tetrominoes[p.type][index];
            if (block != 0)
            {
                float *c = colors[block];
                drawBlock(p.x + px, p.y + py, c[0], c[1], c[2]);
            }
        }
    }
}

void error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main()
{
    srand((unsigned)time(NULL));

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris OpenGL", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    currentPiece.type = rand() % 7;
    currentPiece.x = BOARD_WIDTH / 2 - 2;
    currentPiece.y = 0;
    currentPiece.rotation = 0;

    double lastFallTime = glfwGetTime();
    double fallInterval = 0.5;

    bool keyLeftPressed = false;
    bool keyRightPressed = false;
    bool keyDownPressed = false;
    bool keyRotatePressed = false;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !keyLeftPressed)
        {
            Piece moved = currentPiece;
            moved.x -= 1;
            if (doesPieceFit(moved))
                currentPiece = moved;
            keyLeftPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
        {
            keyLeftPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !keyRightPressed)
        {
            Piece moved = currentPiece;
            moved.x += 1;
            if (doesPieceFit(moved))
                currentPiece = moved;
            keyRightPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
        {
            keyRightPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !keyDownPressed)
        {
            Piece moved = currentPiece;
            moved.y += 1;
            if (doesPieceFit(moved))
                currentPiece = moved;
            keyDownPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        {
            keyDownPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !keyRotatePressed)
        {
            Piece rotated = currentPiece;
            rotated.rotation = (rotated.rotation + 1) % 4;
            if (doesPieceFit(rotated))
                currentPiece = rotated;
            keyRotatePressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
        {
            keyRotatePressed = false;
        }

        double currentTime = glfwGetTime();
        if (currentTime - lastFallTime > fallInterval)
        {
            Piece moved = currentPiece;
            moved.y += 1;
            if (doesPieceFit(moved))
            {
                currentPiece = moved;
            }
            else
            {
                lockPiece(currentPiece);
                int lines = clearLines();
                if (lines > 0)
                {
                    totalClearedLines += lines;
                    std::cout << "Cleared " << lines << " line(s)! Total: " << totalClearedLines << ".\n";
                }

                currentPiece.type = rand() % 7;
                currentPiece.x = BOARD_WIDTH / 2 - 2;
                currentPiece.y = 0;
                currentPiece.rotation = 0;

                if (!doesPieceFit(currentPiece))
                {
                    std::cout << "Game Over! Total Lines Cleared: " << totalClearedLines << "\n";
                    break;
                }
            }
            lastFallTime = currentTime;
        }

        std::stringstream ss;
        ss << "Tetris OpenGL | Lines: " << totalClearedLines;
        glfwSetWindowTitle(window, ss.str().c_str());

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawBoard();
        drawPiece(currentPiece);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
