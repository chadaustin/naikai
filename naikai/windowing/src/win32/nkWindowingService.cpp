#include "nkWindowingService.h"
#include "nkMenu.h"
#include "nkWindow.h"


nkWindowingService::nkWindowingService()
{
  NS_INIT_REFCNT();
}


NS_IMPL_ISUPPORTS1(nkWindowingService, nkIWindowingService);


NS_IMETHODIMP
nkWindowingService::CreateFrameWindow(nkIWindow** rv)
{
  HINSTANCE instance = GetModuleHandle(NULL);

  // register the window class
  // don't bother checking to see if window class registration fails
  // if it does fail, window creation will
  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));
  wc.style         = CS_OWNDC;  // required for the OpenGL renderer
  wc.lpfnWndProc   = nkWindow::WindowProc;
  wc.hInstance     = instance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = "NaikaiWindow";
  RegisterClass(&wc);

  HWND window = ::CreateWindowA(
    "NaikaiWindow",
    "Naikai Window",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0,
    CW_USEDEFAULT, 0,
    NULL, NULL, instance, NULL);    

  if (!window) {
    *rv = NULL;
    return NS_ERROR_FAILURE;
  }

  ::ShowWindow(window, SW_SHOW);
  ::UpdateWindow(window);

  *rv = new nkWindow(window);
  NS_ADDREF(*rv);
  return NS_OK;
}


NS_IMETHODIMP
nkWindowingService::CreateMenu(nkIMenu** rv)
{
  HMENU menu = ::CreateMenu();
  if (!menu) {
    return NS_ERROR_FAILURE;
  }

  *rv = new nkMenu(menu);
  NS_ADDREF(*rv);
  return NS_OK;
}


NS_IMETHODIMP
nkWindowingService::GetOnIdle(nkICommand** on_idle)
{
  *on_idle = m_on_idle;
  return NS_OK;
}


NS_IMETHODIMP
nkWindowingService::SetOnIdle(nkICommand* on_idle)
{
  m_on_idle = on_idle;
  return NS_OK;
}


NS_IMETHODIMP
nkWindowingService::Run()
{
  MSG msg;
  while (true) {

    bool available = true;
    if (!m_on_idle) {
      if (GetMessage(&msg, NULL, 0, 0) <= 0) {
	return NS_OK;
      }
    } else {
      available = (0 != PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
      if (available && msg.message == WM_QUIT) {
	return NS_OK;
      }
    }

    if (available) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (m_on_idle) {
      m_on_idle->Execute(this);
    }

  }
  return NS_OK;
}


NS_IMETHODIMP
nkWindowingService::PumpEvents()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
