#include "stdafx.h"
#include "resource.h"
#include "Spy.h"
#include "MasterWnd.h"

namespace AceBear
{
    CSpy::CSpy()
    {
        Clear();
    }

    CSpy::~CSpy()
    {
        Clear();
    }

    void CSpy::Switch(DWORD dwProcessId, DWORD dwThreadId, HWND hWnd)
    {
        Clear();

        m_dwProcessId = dwProcessId;
        m_dwThreadId = dwThreadId;
        m_hWnd = hWnd;

        FindBaseName();
        FindChildren();

        CMasterWnd::GetMainWnd()->PostMessage(UM_SWITCHSPY, dwProcessId, dwThreadId);
    }

    void CSpy::Clear()
    {
        m_dwProcessId = 0;
        m_dwThreadId = 0;
        m_hWnd = 0;
        m_wszProcessName[0] = 0;
        m_wszWindowName[0] = 0;

        if (m_hProcess) {
            CloseHandle(m_hProcess);
            m_hProcess = 0;
        }

        m_vWnds.clear();
    }

    void CSpy::FindBaseName()
    {
        m_hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, m_dwProcessId);

        wchar_t buf[256];
        memset(buf, 0, sizeof(buf));
        DWORD dwRet = ::GetProcessImageFileName(m_hProcess, buf, sizeof(buf)/sizeof(wchar_t));
        if (dwRet) {
            wchar_t *p = wcsrchr(buf, L'\\');
            if (p)
                wcscpy_s(m_wszProcessName, p + 1);
            else
                wcscpy_s(m_wszProcessName, buf);
        }

        ::GetWindowText(m_hWnd, m_wszWindowName, sizeof(m_wszWindowName) / sizeof(wchar_t));

    }

    void CSpy::FindChildren()
    {
        ::EnumChildWindows(m_hWnd, EnumChildProc, (LPARAM)this);
    }

    BOOL CSpy::EnumChildProc(HWND hwnd, LPARAM lParam)
    {
        CSpy *pSpy = (CSpy*)lParam;
        vector<WNDESC> &vWnds = pSpy->m_vWnds;
        
        WNDESC desc;
        desc.hWnd = hwnd;
        GetWindowText(hwnd, desc.wszText, sizeof(desc.wszText) / sizeof(wchar_t));
        GetWindowRect(hwnd, &desc.rc);
        
        ScreenToClient(pSpy->m_hWnd, (POINT*)&desc.rc.left);
        ScreenToClient(pSpy->m_hWnd, (POINT*)&desc.rc.right);

        vWnds.push_back(desc);

        return TRUE;
    }
}