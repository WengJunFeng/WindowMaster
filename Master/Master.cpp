// Master.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Master.h"
using namespace AceBear;

CMaster *g_pMasterApp = NULL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CMaster master(hInstance);
    g_pMasterApp = &master;

    DWORD dwRet = master.Init(L"MaSTeR", nullptr, master.CalcPos(), WS_OVERLAPPEDWINDOW);
    if (dwRet != NOERROR) return dwRet;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

namespace AceBear
{
    CMaster::CMaster(HINSTANCE hInstApp)
        :m_hInstApp(hInstApp), m_hWndMain(0)
    {
    }

    CMaster::~CMaster()
    {

    }

    HINSTANCE CMaster::GetAppInst()
    {
        return g_pMasterApp->m_hInstApp;
    }

    RECT CMaster::CalcPos()
    {
        RECT rc;
        rc.left = ::GetSystemMetrics(SM_CXSCREEN) - 280;
        rc.right = rc.left + 240;
        rc.top = 120;
        rc.bottom = rc.top + 480;
        return rc;
    }

    const wchar_t* CMaster::RegisterCls()
    {
        WNDCLASSEX wcex;
        ::ZeroMemory(&wcex, sizeof(WNDCLASSEX));
        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = (WNDPROC)CBaseWnd::BaseWndProc;
        wcex.hInstance = m_hInstApp;
        wcex.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
        wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
        wcex.lpszClassName = m_wszClsName;

        RegisterClassExW(&wcex);

        return m_wszClsName;
    }

    LRESULT CMaster::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
            HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
            HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return 0L;
    }

    BOOL CMaster::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
    {
        RECT rcMain;
        ::GetClientRect(hwnd, &rcMain);

        RECT rcTool = rcMain;
        rcTool.bottom = rcTool.top + 50;
        m_wndTool.Init(L"ToolBox", this, rcTool);
        ::ShowWindow(hwnd, SW_SHOW);
        return TRUE;
    }

    void CMaster::OnDestroy(HWND hWnd)
    {
        ::PostQuitMessage(0);
    }
}