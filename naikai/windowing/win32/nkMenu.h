#ifndef NK_MENU_H
#define NK_MENU_H


#include <vector>
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

  std::vector<nkIMenu*>    m_sub_menus;
  std::vector<nkICommand*> m_menu_items;

  friend nkWindowingService;
};


#endif
