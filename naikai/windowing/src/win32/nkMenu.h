#ifndef NK_MENU_H
#define NK_MENU_H


#include <utility>
#include <vector>
#include <windows.h>
#include "nkIPrivateMenu.h"


class nkICommand;

class nkWindowingService;
class nkWindow;


class nkMenu : public nkIPrivateMenu
{
private:
  nkMenu(HMENU menu);
  ~nkMenu();

public:
  NS_DECL_ISUPPORTS
  NS_DECL_NKIMENU

  NS_IMETHOD GetMenuHandle(HMENU& menu);
  NS_IMETHOD Attach(nkWindow* window);
  NS_IMETHOD Detach(nkWindow* window);

private:
  bool m_packed;
  HMENU m_menu;

  std::vector<nkIPrivateMenu*>              m_sub_menus;
  std::vector<std::pair<int, nkICommand*> > m_menu_items;

  friend nkWindowingService;
  friend nkWindow;
};


#endif
