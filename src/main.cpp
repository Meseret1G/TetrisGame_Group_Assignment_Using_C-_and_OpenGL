

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

