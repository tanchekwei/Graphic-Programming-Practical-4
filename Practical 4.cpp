#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#pragma comment(lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

int page = 1;

bool rotateX = false, rotateY = false, rotateZ = false;

float rotateHandDegree = 0, rotateArmDegree = 0;

void drawPyramid(float size);
void drawCuboid(float size, float widthScale);

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        else if (wParam == VK_F1)
        {
            page = 1;
        }
        else if (wParam == VK_F2)
        {
            page = 2;
        }
        else if (wParam == 0x58)
        {
            if (rotateX)
                rotateX = false;
            else
                rotateX = true;
        }
        else if (wParam == 0x59)
        {
            if (rotateY)
                rotateY = false;
            else
                rotateY = true;
        }
        else if (wParam == 0x5A)
        {
            if (rotateZ)
                rotateZ = false;
            else
                rotateZ = true;
        }
        else if (wParam == VK_UP)
        {
            rotateHandDegree += 1;
        }
        else if (wParam == VK_DOWN)
        {
            rotateHandDegree -= 1;
        }
        else if (wParam == VK_RIGHT)
        {
            rotateArmDegree += 1;
        }
        else if (wParam == VK_LEFT)
        {
            rotateArmDegree -= 1;
        }
        else if (wParam == VK_SPACE)
        {
            rotateHandDegree = 0;
            rotateArmDegree = 0;
        }
    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.cAlphaBits = 8;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 0;

    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;

    // choose pixel format returns the number most similar pixel format available
    int n = ChoosePixelFormat(hdc, &pfd);

    // set pixel format returns whether it sucessfully set the pixel format
    if (SetPixelFormat(hdc, n, &pfd))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//--------------------------------------------------------------------

void display()
{
    // handle resolution problem
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = 1920;
    double h = 1080;
    double ar = w / h;
    glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    switch (page)
    {
    case 1:
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        if (rotateX)
        {
            glRotatef(1, 1, 0, 0);
        }
        if (rotateY)
        {
            glRotatef(1, 0, 1, 0);
        }
        if (rotateZ)
        {
            glRotatef(1, 0, 0, 1);
        }

        drawPyramid(0.5);
        break;
    case 2:
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glPushMatrix();
        {
        glRotatef(0 + rotateHandDegree, 0, 0, 1);
        glRotatef(0 + rotateArmDegree, 1, 1, 1);
        drawCuboid(0.3, 2.5);
        }
        glPopMatrix();

        // glLoadIdentity();
        glPushMatrix();
        {
            glRotatef(0 + rotateArmDegree, 1, 1, 1);
            glTranslatef(-0.75, 0, 0);
            drawCuboid(0.3, 2.5);
        }
        glPopMatrix();

        break;
    default:
        break;
    }
}

void drawPyramid(float size)
{
    glBegin(GL_LINE_STRIP);
    // front
    glColor3ub(30, 136, 229);
    glVertex3f(size / 2, size / 2, size);
    glVertex3f(size, 0, 0);
    glVertex3f(0, 0, 0);

    // left
    glColor3ub(223, 120, 239);
    //glVertex3f(0, size, size);
    //glVertex3f(0, 0, size);
    glVertex3f(size / 2, size / 2, size);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    // bottom
    glColor3ub(128, 226, 126);
    glVertex3f(0, size, 0);
    glVertex3f(size, size, 0);
    glVertex3f(size, 0, 0);
    glVertex3f(0, 0, 0);

    // right
    glColor3ub(255, 255, 114);
    //glVertex3f(size, 0, size);
    //glVertex3f(size, size, size);
    glVertex3f(size / 2, size / 2, size);
    glVertex3f(size, size, 0);
    glVertex3f(size, 0, 0);

    // behind
    glColor3ub(255, 201, 71);
    //glVertex3f(size, size, size);
    //glVertex3f(0, size, size);
    glVertex3f(size / 2, size / 2, size);
    glVertex3f(0, size, 0);
    glVertex3f(size, size, 0);

    // top
    //glColor3ub(115, 232, 255);
    //glVertex3f(0, size, size);
    //glVertex3f(size, size, size);
    //glVertex3f(size, 0, size);
    //glVertex3f(0, 0, size);

    glEnd();
}

void drawCuboid(float size, float widthScale)
{
    glBegin(GL_LINES);
    // front top
    glColor3ub(30, 136, 229);
    glVertex3f(0, 0, size);
    glVertex3f(size * widthScale, 0, size);

    // front right
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(size * widthScale, 0, 0);

    // front bottom
    glVertex3f(size * widthScale, 0, 0);
    glVertex3f(0, 0, 0);

    // front left
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);

    // left top
    glVertex3f(0, size, size);
    glVertex3f(0, 0, size);

    // left right
    glVertex3f(0, 0, size);
    glVertex3f(0, 0, 0);

    // left bottom
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    // left left
    glVertex3f(0, size, 0);
    glVertex3f(0, size, size);

    // right
    // right top
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(size * widthScale, size, size);

    // right right
    glVertex3f(size * widthScale, size, size);
    glVertex3f(size * widthScale, size, 0);

    // right bottom
    glVertex3f(size * widthScale, size, 0);
    glVertex3f(size * widthScale, 0, 0);

    // right left
    glVertex3f(size * widthScale, 0, 0);
    glVertex3f(size * widthScale, 0, size);

    // behind top
    glVertex3f(0, size, size);
    glVertex3f(size * widthScale, size, size);

    // behind bottom
    glVertex3f(0, size, 0);
    glVertex3f(size * widthScale, size, 0);

    glEnd();
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = WINDOW_TITLE;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wc))
        return false;

    HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
                             NULL, NULL, wc.hInstance, NULL);

    //--------------------------------
    //	Initialize window for OpenGL
    //--------------------------------

    HDC hdc = GetDC(hWnd);

    //	initialize pixel format for the window
    initPixelFormat(hdc);

    //	get an openGL context
    HGLRC hglrc = wglCreateContext(hdc);

    //	make context current
    if (!wglMakeCurrent(hdc, hglrc))
        return false;

    //--------------------------------
    //	End initialization
    //--------------------------------

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        display();

        SwapBuffers(hdc);
    }
    UnregisterClass(WINDOW_TITLE, wc.hInstance);
    return true;
}
