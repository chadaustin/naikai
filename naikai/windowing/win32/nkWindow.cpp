#include "nkWindow.h"


nkWindow::nkWindow(HWND window)
{
  m_window = window;
  SetWindowLong(m_window, GWL_USERDATA, (LONG)this);
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
nkWindow::SetMenu(nkIMenu* menu_)
{
  if (!menu_) {    // clear menu

    m_menu->Detach(this);

    if (FALSE == ::SetMenu(m_window, 0)) {
      return NS_ERROR_FAILURE;
    }

    m_menu = 0;
    return NS_OK;

  } else {        // set menu

    nsCOMPtr<nkIPrivateMenu> menu(do_QueryInterface(menu_));
    if (!menu) {
      return NS_ERROR_ILLEGAL_VALUE;
    }

    nsresult rv = menu->Pack();
    if (NS_FAILED(rv)) {
      return rv;
    }
    
    HMENU hmenu;
    rv = menu->GetMenuHandle(hmenu);
    if (NS_FAILED(rv)) {
      return rv;
    }

    if (FALSE == ::SetMenu(m_window, hmenu)) {
      return NS_ERROR_FAILURE;
    }

    menu->Attach(this);

    m_menu = menu;
    return NS_OK;
  }
}


void
nkWindow::MapCommand(int command, nkICommand* the_command)
{
  NS_IF_ADDREF(the_command);
  m_commands[command] = the_command;
}


void
nkWindow::UnmapCommand(int command)
{
  NS_IF_RELEASE(m_commands[command]);
  m_commands.erase(command);
}


LRESULT CALLBACK
nkWindow::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  nkWindow* w = (nkWindow*)GetWindowLong(window, GWL_USERDATA);
  if (w) {
    return w->HandleMessage(message, wparam, lparam);
  } else {
    return DefWindowProc(window, message, wparam, lparam);
  }
}


LRESULT
nkWindow::HandleMessage(UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message) {
  case WM_CLOSE: {
    ::PostQuitMessage(0);
    return 0;
  }

  case WM_COMMAND: {
    WORD command = LOWORD(wparam);
    if (m_commands.count(command)) {
      nkICommand* c = m_commands[command];
      if (c) {

	// ignore result
	c->Execute(this);

      }
    }
    return 0;
  }

  default: {
    return DefWindowProc(m_window, message, wparam, lparam);
  }
  }
}
