#include "nkWindow.h"


nkWindow::nkWindow(HWND window)
{
  m_window = window;
}


nkWindow::~nkWindow()
{
  if (m_window) {
    ::DestroyWindow(m_window);
    m_window = NULL;
  }
}


NS_IMPL_ISUPPORTS1(nkWindow, nkIWindow)


NS_IMETHODIMP
nkWindow::GetTitle(PRUnichar** title)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindow::SetTitle(const PRUnichar* title)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindow::GetFullscreen(PRBool* fullscreen)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindow::SetFullscreen(PRBool fullscreen)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindow::GetVisible(PRBool* visible)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindow::SetVisible(PRBool visible)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindow::SetMenu(nkIMenu* menu)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


LRESULT CALLBACK
nkWindow::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message) {
    case WM_CLOSE: {
      ::PostQuitMessage(0);
      return 0;
    }

    default: {
      return ::DefWindowProc(window, message, wparam, lparam);
    }
  }
}
