#ifndef NK_I_PRIVATE_MENU_H
#define NK_I_PRIVATE_MENU_H


#include <windows.h>
#include "nkIMenu.h"


class nkWindow;


#define NK_IPRIVATEMENU_IID \
  { 0x859a8ff6, 0x1dd2, 0x11b2, \
    { 0xb9, 0x0d, 0xb6, 0xcc, 0x64, 0x0b, 0x5e, 0x5b } }


class nkIPrivateMenu : public nkIMenu
{
public:
  NS_DEFINE_STATIC_IID_ACCESSOR(NK_IPRIVATEMENU_IID)

  NS_IMETHOD GetMenuHandle(HMENU& menu) = 0;
  NS_IMETHOD Attach(nkWindow* window) = 0;
  NS_IMETHOD Detach(nkWindow* window) = 0;
};


#endif
