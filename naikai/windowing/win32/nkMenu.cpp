#include "nkMenu.h"


nkMenu::nkMenu(HMENU menu)
{
  m_menu = menu;
  m_packed = false;
}


nkMenu::~nkMenu()
{
  int i;

  // clear sub menu array
  for (i = 0; i < m_sub_menus.size(); ++i) {
    NS_IF_RELEASE(m_sub_menus[i]);
  }
  m_sub_menus.clear();

  // clear menu item array
  for (i = 0; i < m_menu_items.size(); ++i) {
    NS_IF_RELEASE(m_menu_items[i]);
  }
  m_menu_items.clear();

  // destroy the Win32 menu
  if (m_menu) {
    DestroyMenu(m_menu);
    m_menu = NULL;
  }
}


NS_IMPL_ISUPPORTS1(nkMenu, nkIMenu)


NS_IMETHODIMP
nkMenu::GetSubMenuCount(PRInt32* sub_menu_count)
{
  *sub_menu_count = m_sub_menus.size();
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::GetSubMenu(PRInt32 index, nkIMenu** rv)
{
  *rv = m_sub_menus[index];
  NS_ADDREF(*rv);
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::GetItemCount(PRInt32* item_count)
{
  *item_count = m_menu_items.size();
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::GetItem(PRInt32 index, nkICommand** rv)
{
  *rv = m_menu_items[index];
  NS_ADDREF(*rv);
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::AppendSubMenu(const PRUnichar* name, nkIMenu* menu)
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  nsresult rv = menu->Pack();
  if (NS_FAILED(rv)) {
    return rv;
  }

  HMENU hmenu;
  rv = menu->GetNative((void**)&hmenu);
  if (NS_FAILED(rv)) {
    return rv;
  }

  // append sub menu
  if (FALSE == ::AppendMenuW(m_menu,
			     MF_POPUP | MF_STRING, (UINT)hmenu, name)) {
    return NS_ERROR_FAILURE;
  }

  NS_IF_ADDREF(menu);
  m_sub_menus.push_back(menu);

  return NS_OK;
}


NS_IMETHODIMP
nkMenu::AppendSeparator()
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  // append separator
  if (FALSE == ::AppendMenu(m_menu, MF_SEPARATOR, 0, 0)) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}


NS_IMETHODIMP
nkMenu::AppendItem(const PRUnichar* name, nkICommand* command)
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  // append menu item
  if (FALSE == ::AppendMenuW(m_menu, MF_STRING | MF_ENABLED, 0, name)) {
    return NS_ERROR_FAILURE;
  }

  NS_IF_ADDREF(command);
  m_menu_items.push_back(command);

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
