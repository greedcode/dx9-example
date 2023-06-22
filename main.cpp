#include <d3d9.h>
#include <d3dx9.h>

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3d_device;
LPD3DXFONT font;

const int window_width = 1280;
const int window_height = 720;

void init_d3d(HWND hwnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferWidth = window_width;
    d3dpp.BackBufferHeight = window_height;
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d_device);
    D3DXCreateFont(d3d_device, 24, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma", &font);
}

void render_frame()
{
    d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3d_device->BeginScene();
    static int dx = rand() % 3 - 1;
    static int dy = rand() % 3 - 1;
    static int x = 10;
    static int y = 10;
    RECT rect = { x, y, x + 100, y + 100 };
    static double h = 0;
    D3DCOLOR color = D3DCOLOR_XRGB(static_cast<int>((sin(0.3 * h + 0.0) + 1.0) * 127), static_cast<int>((sin(0.3 * h + 2.0) + 1.0) * 127), static_cast<int>((sin(0.3 * h + 4.0) + 1.0) * 127));
    h += 1.0;
    font->DrawText(NULL, L"rainbow", -1, &rect, DT_CENTER | DT_VCENTER, color);
    x += dx * 5;
    y += dy * 5;
    if (x <= 0 || x + 100 >= window_width)
        dx = -dx;
    if (y <= 0 || y + 100 >= window_height)
        dy = -dy;
    d3d_device->EndScene();
    d3d_device->Present(NULL, NULL, NULL, NULL);
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
    switch (message)
    {
    case WM_PAINT:
        render_frame();
        ValidateRect(hwnd, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, w_param, l_param);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show)
{
    const wchar_t class_name[] = L"dx9_window_class";
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = window_proc;
    wc.hInstance = h_instance;
    wc.lpszClassName = class_name;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, class_name, L"cool dx9 window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height, NULL, NULL, h_instance, NULL);
    if (hwnd == NULL)
        return 0;
    init_d3d(hwnd);
    ShowWindow(hwnd, n_cmd_show);
    MSG msg;
    while (true)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        render_frame();
    }
    font->Release();
    d3d_device->Release();
    d3d->Release();
    return 0;
}