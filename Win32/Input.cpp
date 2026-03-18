#include "Input.h"

void Input::OnKeyDown(unsigned char key)
{
    keystate[key] = true;
}

void Input::OnKeyUp(unsigned char key)
{
    keystate[key] = false;
}

bool Input::KeyIsPressed(unsigned char key) const
{
    return keystate[key];
}

void Input::OnMouseMove(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

void Input::OnMouseDown(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

int Input::GetMouseX() {
    return mouseX;
}

int Input::GetMouseY() {
    return mouseY;
}