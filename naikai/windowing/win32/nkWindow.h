#ifndef NK_WINDOW_H
#define NK_WINDOW_H


#include <windows.h>
#include "nkIWindow.h"


class nkWindowingService;


class nkWindow : public nkIWindow
{
public:
  nkWindow(HWND window);
  ~nkWindow();

  NS_DECL_ISUPPORTS
  NS_DECL_NKIWINDOW

private:
  static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
  HWND m_window;

  friend nkWindowingService;
};


#endif
