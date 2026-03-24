#pragma once
#include <windows.h>

class App;

class Window
{
public:
    Window(App& app, int width, int height, const wchar_t* name);
  
    ~Window();

    bool ProcessMessages();
    HWND GetHWND() const;
    int GetWidth() const;
    int GetHeight() const;

private:
    static LRESULT CALLBACK SetupProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK ThunkProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM);

private:
    HWND hwnd;
    App& app;
    
};