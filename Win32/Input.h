#pragma once


class Input
{
public:
    void OnKeyDown(unsigned char key);
    void OnKeyUp(unsigned char key);

    void OnMouseMove(int x, int y);
    void OnMouseDown(int x, int y);

    bool KeyIsPressed(unsigned char key) const;

    int GetMouseX();
    int GetMouseY();
private:
    bool keystate[256] = {};
    int mouseX = 0;
    int mouseY = 0;

};