#ifndef NK_WINDOW_H
#define NK_WINDOW_H


#include <windows.h>
#include "nsCOMPtr.h"
#include "nkIWindow.h"
#include "nkIMenu.h"


class nkWindowingService;


class nkWindow : public nkIWindow
{
private:
  nkWindow(HWND window);
  ~nkWindow();

public:
  NS_DECL_ISUPPORTS
  NS_DECL_NKIWINDOW

private:
  static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
  HWND m_window;

  nsCOMPtr<nkIMenu> m_menu;

  friend nkWindowingService;
};


#endif
