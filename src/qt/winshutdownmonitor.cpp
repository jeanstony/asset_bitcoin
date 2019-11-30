// Copyright (c) 2014-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/winshutdownmonitor.h>

#if defined(Q_OS_WIN)
#include <shutdown.h>
#include <util/system.h>

#include <windows.h>
#include <qt/guiutil.h>
#include <QDebug>

#include <openssl/rand.h>
#include <locale>
#include <codecvt>

// If we don't want a message to be processed by Qt, return true and set result to
// the value that the window procedure should return. Otherwise return false.
bool WinShutdownMonitor::nativeEventFilter(const QByteArray &eventType, void *pMessage, long *pnResult)
{
       Q_UNUSED(eventType);

       MSG *pMsg = static_cast<MSG *>(pMessage);

       // Seed OpenSSL PRNG with Windows event data (e.g.  mouse movements and other user interactions)
       if (RAND_event(pMsg->message, pMsg->wParam, pMsg->lParam) == 0) {
            // Warn only once as this is performance-critical
            static bool warned = false;
            if (!warned) {
                LogPrintf("%s: OpenSSL RAND_event() failed to seed OpenSSL PRNG with enough data.\n", __func__);
                warned = true;
            }
       }

       switch(pMsg->message)
       {
           case WM_QUERYENDSESSION:
           {
               // Initiate a client shutdown after receiving a WM_QUERYENDSESSION and block
               // Windows session end until we have finished client shutdown.
               StartShutdown();
               *pnResult = FALSE;
               return true;
           }

           case WM_ENDSESSION:
           {
               *pnResult = FALSE;
               return true;
           }
       }

       return false;
}

void WinShutdownMonitor::registerShutdownBlockReason(const QString& strReason, const YTMHWND& mainWinId)
{
    typedef BOOL (WINAPI *PSHUTDOWNBRCREATE)(YTMHWND, LPCWSTR);
    PSHUTDOWNBRCREATE shutdownBRCreate = (PSHUTDOWNBRCREATE)GetProcAddress(GetModuleHandleA("User32.dll"), "ShutdownBlockReasonCreate");
    if (shutdownBRCreate == nullptr) {
        qWarning() << "registerShutdownBlockReason: GetProcAddress for ShutdownBlockReasonCreate failed";
        return;
    }

    std::string tmp = GUIUtil::toStdString(strReason);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(tmp.c_str());

    if (shutdownBRCreate(mainWinId, wide.c_str()))
        qInfo() << "registerShutdownBlockReason: Successfully registered: " + strReason;
    else
        qWarning() << "registerShutdownBlockReason: Failed to register: " + strReason;
}
#endif
