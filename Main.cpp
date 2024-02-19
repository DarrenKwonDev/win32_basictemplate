#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    // windowclass
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    // windowclose�� ���
    RegisterClass(&wc);

    // Create the window. ������ �ڵ鰪�� ��ȯ��
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data -> ���� ����[state management]�� ���� (https://learn.microsoft.com/ko-kr/windows/win32/learnwin32/managing-application-state-)
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // window �����ֱ�
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    // �޽����� ������ GetMessage()�� �ش� �����带 ��� ���·� �ξ� CPU �ڿ��� ������� �ʽ��ϴ�. 
    // �׷��� ��¥�� while true ������ cpu 100% ��� �Դµ� �̰� �� �׷�.
    MSG msg = { };

    // https://learn.microsoft.com/ko-kr/windows/win32/api/winuser/nf-winuser-getmessage
    // msg queue���� �޽����� �̾ƿ�
    while (GetMessage(&msg, NULL, 0, 0) > 0)  // PostQuitMessage(0) ������ 0�� ��ȯ�Ͽ� while ����
    {
        TranslateMessage(&msg); // Ű �Է�(Ű ������, Ű ����)�� ���ڷ� ��ȯ�մϴ�.
        DispatchMessage(&msg); // ���ν������� �޽��� ����
    }

    return 0;
}

// https://learn.microsoft.com/ko-kr/windows/win32/learnwin32/writing-the-window-procedure
/*
������ ���ν����� ����Ǵ� ���� ������ �����忡�� ���� â�� ���� �ٸ� ��� �޽����� �����մϴ�. 
���� ������ ���ν��� ������ ���� �ɸ��� ó���� ���ؾ� ��.
���� ����, blocking ���� �Ǹ� �ȵȴ�.
���� ��� ���α׷��� TCP ������ ���� ������ ������ ������ ������ ����Ѵٰ� �����մϴ�. 
������ ���ν��� ������ �� �۾��� �����ϸ� ��û�� �Ϸ�� ������ UI�� �������� �ʽ��ϴ�. 
�� �ð� ���� â�� ���콺 �Ǵ� Ű���� �Է��� ó���ϰų�, ��ü������ �ٽ� �׸��ų�, ���� �� �����ϴ�.

��� Windows�� �⺻ �����Ǵ� ��Ƽ�½�ŷ ��� �� �ϳ��� ����Ͽ� �۾��� �ٸ� ������� �̵��ؾ� �մϴ�.
(���� �ɸ� �� ������ �ٸ� �����忡�� �۾� ó���϶�� ��)

�� �����带 ����ϴ�.
������ Ǯ�� ����մϴ�.
�񵿱� I/O ȣ���� ����մϴ�.
�񵿱� ���ν��� ȣ���� ����մϴ�.
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // �� �޽��� ���� ����
    // https://learn.microsoft.com/ko-kr/windows/win32/winmsg/windowing
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // WM_QUIT �޽����� ��ġ�Ͽ� �޽��� ������ ����ǵ��� �Ѵ�
        return 0;
    case WM_CLOSE:
        if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }
        // Else: User canceled. Do nothing.
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        // All painting occurs here, between BeginPaint and EndPaint.
        HDC hdc = BeginPaint(hwnd, &ps); // �Լ��� ȣ���Ͽ� �׸��� �۾��� �����մϴ�
        
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps); // �׸��� �۾��� �������մϴ�
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}