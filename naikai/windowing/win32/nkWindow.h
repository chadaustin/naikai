#ifndef NK_WINDOW_H
#define NK_WINDOW_H


#include <map>
#include <windows.h>
#include "nsCOMPtr.h"
#include "nkIWindow.h"
#include "nkIPrivateMenu.h"


class nkICommand;

class nkWindowingService;
class nkMenu;


class nkWindow : public nkIWindow
{
private:
  nkWindow(HWND window);
  ~nkWindow();

public:
  NS_DECL_ISUPPORTS
  NS_DECL_NKIWINDOW

private:
  void MapCommand(int command, nkICommand* the_command);
  void UnmapCommand(int command);

  static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
  LRESULT HandleMessage(UINT, WPARAM, LPARAM);

private:
  HWND m_window;
  nsCOMPtr<nkIPrivateMenu> m_menu;
  std::map<int, nkICommand*> m_commands;

  friend nkWindowingService;
  friend nkMenu;
};


#endif
