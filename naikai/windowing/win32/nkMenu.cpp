#include "nkMenu.h"


nkMenu::nkMenu(HMENU menu)
{
  m_menu = menu;
  m_packed = false;
}


nkMenu::~nkMenu()
{
  if (m_menu) {
    DestroyMenu(m_menu);
    m_menu = NULL;
  }
}


NS_IMPL_ISUPPORTS1(nkMenu, nkIMenu)


NS_IMETHODIMP
nkMenu::AppendSubMenu(const PRUnichar* name, nkIMenu* menu)
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  // append sub menu
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::AppendSeparator()
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  // append separator
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::AppendItem(const PRUnichar* name, nkIMenuItem* item)
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  // append menu item
  if (FALSE == ::AppendMenu(m_menu, MF_STRING | MF_ENABLED, 0, "string")) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}


NS_IMETHODIMP
nkMenu::Pack()
{
  // pack the menu

  m_packed = true;
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::GetNative(void** rv)
{
  *((HMENU*)rv) = m_menu;
  return NS_OK;
}
