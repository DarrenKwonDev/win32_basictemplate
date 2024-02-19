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

    // windowclose를 등록
    RegisterClass(&wc);

    // Create the window. 윈도우 핸들값을 반환함
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
        NULL        // Additional application data -> 상태 관리[state management]용 인자 (https://learn.microsoft.com/ko-kr/windows/win32/learnwin32/managing-application-state-)
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // window 보여주기
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    // 메시지가 없으면 GetMessage()는 해당 스레드를 대기 상태로 두어 CPU 자원을 사용하지 않습니다. 
    // 그래서 생짜로 while true 돌리면 cpu 100% 잡아 먹는데 이건 안 그럼.
    MSG msg = { };

    // https://learn.microsoft.com/ko-kr/windows/win32/api/winuser/nf-winuser-getmessage
    // msg queue에서 메시지를 뽑아옴
    while (GetMessage(&msg, NULL, 0, 0) > 0)  // PostQuitMessage(0) 받으면 0을 반환하여 while 종료
    {
        TranslateMessage(&msg); // 키 입력(키 누르기, 키 떼기)을 문자로 변환합니다.
        DispatchMessage(&msg); // 프로시저에게 메시지 전달
    }

    return 0;
}

// https://learn.microsoft.com/ko-kr/windows/win32/learnwin32/writing-the-window-procedure
/*
윈도우 프로시저가 실행되는 동안 동일한 스레드에서 만든 창에 대한 다른 모든 메시지를 차단합니다. 
따라서 윈도우 프로시저 내에서 오래 걸리는 처리를 피해야 함.
쉽게 말해, blocking 오래 되면 안된다.
예를 들어 프로그램이 TCP 연결을 열고 서버가 응답할 때까지 무기한 대기한다고 가정합니다. 
윈도우 프로시저 내에서 이 작업을 수행하면 요청이 완료될 때까지 UI가 응답하지 않습니다. 
이 시간 동안 창은 마우스 또는 키보드 입력을 처리하거나, 자체적으로 다시 그리거나, 닫을 수 없습니다.

대신 Windows에 기본 제공되는 멀티태스킹 기능 중 하나를 사용하여 작업을 다른 스레드로 이동해야 합니다.
(오래 걸릴 것 같으면 다른 스레드에서 작업 처리하라는 말)

새 스레드를 만듭니다.
스레드 풀을 사용합니다.
비동기 I/O 호출을 사용합니다.
비동기 프로시저 호출을 사용합니다.
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 각 메시지 세부 설명
    // https://learn.microsoft.com/ko-kr/windows/win32/winmsg/windowing
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // WM_QUIT 메시지를 배치하여 메시지 루프가 종료되도록 한다
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
        HDC hdc = BeginPaint(hwnd, &ps); // 함수를 호출하여 그리기 작업을 시작합니다
        
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps); // 그리기 작업을 마무리합니다
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}