#include "nkMenu.h"
#include "nkWindow.h"


class CommandDatabase {
public:
  CommandDatabase() {
    InitializeCriticalSection(&m_cs);
    m_command = 100;  // good starting value?
  }

  ~CommandDatabase() {
    DeleteCriticalSection(&m_cs);
  }

  int GetNextCommand() {
    EnterCriticalSection(&m_cs);
    int result = (m_command++) % 65536;
    LeaveCriticalSection(&m_cs);
    return result;
  }

private:
  CRITICAL_SECTION m_cs;
  int m_command;

} g_CommandDatabase;


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
    NS_IF_RELEASE(m_menu_items[i].second);
  }
  m_menu_items.clear();

  // destroy the Win32 menu
  if (m_menu) {
    DestroyMenu(m_menu);
    m_menu = NULL;
  }
}


NS_IMPL_ISUPPORTS2(nkMenu, nkIMenu, nkIPrivateMenu)


NS_IMETHODIMP
nkMenu::AppendSubMenu(const PRUnichar* name, nkIMenu* menu_)
{
  if (m_packed) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

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

  // append sub menu
  if (FALSE == ::AppendMenuW(m_menu,
			     MF_POPUP | MF_STRING, (UINT)hmenu, name)) {
    return NS_ERROR_FAILURE;
  }

  NS_IF_ADDREF(menu_);
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
  int c = g_CommandDatabase.GetNextCommand();
  if (FALSE == ::AppendMenuW(m_menu, MF_STRING | MF_ENABLED, c, name)) {
    return NS_ERROR_FAILURE;
  }

  NS_IF_ADDREF(command);
  m_menu_items.push_back(std::pair<int, nkICommand*>(c, command));

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
nkMenu::GetMenuHandle(HMENU& menu)
{
  menu = m_menu;
  return NS_OK;
}


NS_IMETHODIMP
nkMenu::Attach(nkWindow* window)
{
  int i;

  // map all menu items
  for (i = 0; i < m_menu_items.size(); ++i) {
    window->MapCommand(m_menu_items[i].first, m_menu_items[i].second);
  }

  // attach all submenus
  for (i = 0; i < m_sub_menus.size(); ++i) {
    m_sub_menus[i]->Attach(window);
  }

  return NS_OK;
}


NS_IMETHODIMP
nkMenu::Detach(nkWindow* window)
{
  int i;

  // detach all submenus
  for (i = 0; i < m_sub_menus.size(); ++i) {
    m_sub_menus[i]->Detach(window);
  }

  // unmap all menu items
  for (i = 0; i < m_menu_items.size(); ++i) {
    window->UnmapCommand(m_menu_items[i].first);
  }

  return NS_OK;
}
