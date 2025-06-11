
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

struct Piece
{
    int x, y;
    int type;
    int rotation;
};

Piece currentPiece;

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
                float* c = colors[block];
                drawBlock(x, y, c[0], c[1], c[2]);
            }
        }
    }
}

void drawPiece(const Piece& p)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int index = rotate(px, py, p.rotation);
            int block = tetrominoes[p.type][index];
            if (block != 0)
            {
                float* c = colors[block];
                drawBlock(p.x + px, p.y + py, c[0], c[1], c[2]);
            }
        }
    }
}


void spawnPiece()
{
 currentPiece.type = rand() % 7;
 currentPiece.x = BOARD_WIDTH / 2 - 2;
 currentPiece.y = 0;
 currentPiece.rotation = 0;
}
bool gameOver = false;
float fallSpeed = 0.5f;
float fallTimer = 0;
void processInput(GLFWwindow *window)
{
 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
 {
 Piece test = currentPiece;
 test.x--;
 if (doesPieceFit(test))
 currentPiece = test;
 }
 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
 {
 Piece test = currentPiece;
 test.x++;
 if (doesPieceFit(test))
 currentPiece = test;
 }
 if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
 {
 Piece test = currentPiece;
 test.rotation = (test.rotation + 1) % 4;
 if (doesPieceFit(test))
 currentPiece = test;
 }
 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
 {
 Piece test = currentPiece;
 test.y++;
 if (doesPieceFit(test))
 currentPiece = test;
 }
}
int main() // continuation from Person 1
{
 // Initialization code from Person 1 ...
 spawnPiece();
 float lastTime = (float)glfwGetTime();
 while (!glfwWindowShouldClose(window) && !gameOver)
 {
 float currentTime = (float)glfwGetTime();
 float deltaTime = currentTime - lastTime;
 lastTime = currentTime;
 fallTimer += deltaTime;
 processInput(window);
 if (fallTimer >= fallSpeed)
 {
 fallTimer = 0;
 Piece test = currentPiece;
 test.y++;
 if (doesPieceFit(test))
 {
 currentPiece = test;
 }
 else
 {
 lockPiece(currentPiece);
 int cleared = clearLines();
 totalClearedLines += cleared;
 spawnPiece();
 if (!doesPieceFit(currentPiece))
 {
 gameOver = true;
 }
 }
 }
 glClearColor(0, 0, 0, 1);
 glClear(GL_COLOR_BUFFER_BIT);
 drawBoard();
 drawPiece(currentPiece);
 glfwSwapBuffers(window);
 glfwPollEvents();
 }
 glfwDestroyWindow(window);
 glfwTerminate();
 return 0;
}
