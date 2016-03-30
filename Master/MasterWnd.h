#pragma once
#include "ToolWnd.h"

namespace AceBear {
 
    class CMasterWnd : public CBaseWnd
    {
    public:
        CMasterWnd(HINSTANCE hInstApp);
        virtual ~CMasterWnd();

        static HINSTANCE GetAppInst();

        RECT CalcPos();
    protected:
        LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual const wchar_t* RegisterCls();

        BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
        void OnDestroy(HWND hWnd);
        void OnSize(HWND hwnd, UINT state, int cx, int cy);
    private:
        static const wchar_t c_wszClassName[];
        HINSTANCE m_hInstApp;
        HWND m_hWndMain;
        CToolWnd m_wndTool;
    };

}