#include "Window.h"
#include "App.h"
#include "imgui_impl_win32.h"

Window::Window(App& app, int width, int height, const wchar_t* name)
    : app(app)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = SetupProc;
    wc.lpszClassName = L"MyWndClass";
    wc.hInstance = GetModuleHandle(nullptr);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        wc.hInstance,
        this
    );

    ShowWindow(hwnd, SW_SHOW);
}

Window::~Window()
{
    DestroyWindow(hwnd);
}

HWND Window::GetHWND() const
{
    return hwnd;
}

int Window::GetWidth() const
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    return rect.right - rect.left;
}

int Window::GetHeight() const
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    return rect.bottom - rect.top;
}

//消息分发
//我们将SetUpProc作为该窗口的过程函数
LRESULT CALLBACK Window::SetupProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{  
    if (msg == WM_NCCREATE)
    {
        auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* wnd = reinterpret_cast<Window*>(cs->lpCreateParams);

        //更改指定窗口的属性。 该函数还会在额外的窗口内存中设置指定偏移量的值。
        //第二个参数是偏移值(offset，int类型。在这里我们是枚举)，用于指定是哪个属性发生变化
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&ThunkProc);

        return TRUE; // 🔥关键
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::ThunkProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{   
    Window* wnd = reinterpret_cast<Window*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));

    return wnd->HandleMsg(hwnd, msg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
    {
        int w = LOWORD(lParam);
        int h = HIWORD(lParam);
        app.OnResize(w, h);
    }
    return 0;

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        app.input.OnMouseDown(x, y);
        app.input.OnKeyDown(VK_LBUTTON); 
        app.input.mouseClicked = true;

    }
    return 0;

    case WM_LBUTTONUP: 
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        app.input.OnKeyUp(VK_LBUTTON);
        app.input.OnMouseUp(x, y);
        app.input.mouseClicked = false;
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        app.input.OnMouseMove(x, y);
    }
    return 0;

    // 其他 case 处理...


    case WM_KEYDOWN:
        app.input.OnKeyDown((unsigned char)wParam);
        return 0;

    case WM_KEYUP:
        app.input.OnKeyUp((unsigned char)wParam);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Window::ProcessMessages()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}