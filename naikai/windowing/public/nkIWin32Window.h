#ifndef NK_I_WIN32_WINDOW_H
#define NK_I_WIN32_WINDOW_H


#include <windows.h>
#include "nsISupports.h"


#define NK_IWIN32WINDOW_IID \
  { 0xa8203f56, 0x1dd1, 0x11b2, \
    {  0x8a, 0x71, 0xc3, 0xea, 0x28, 0x6c, 0x5e, 0x75 } }


class nkIWin32Window : public nsISupports
{
public:
  NS_DEFINE_STATIC_IID_ACCESSOR(NK_IWIN32WINDOW_IID)

  NS_IMETHOD GetHandle(HWND& menu) = 0;
};


#endif
