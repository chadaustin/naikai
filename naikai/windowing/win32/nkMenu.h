#ifndef NK_MENU_H
#define NK_MENU_H


#include <windows.h>
#include "nkIMenu.h"


class nkWindowingService;


class nkMenu : public nkIMenu
{
private:
  nkMenu(HMENU menu);
  ~nkMenu();

public:
  NS_DECL_ISUPPORTS
  NS_DECL_NKIMENU

private:
  bool m_packed;
  HMENU m_menu;

  friend nkWindowingService;
};


#endif
