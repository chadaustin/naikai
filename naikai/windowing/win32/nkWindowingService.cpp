#include "nkWindowingService.h"
#include "nkWindow.h"


nkWindowingService::nkWindowingService()
{
  NS_INIT_REFCNT();
}


NS_IMPL_ISUPPORTS1(nkWindowingService, nkIWindowingService);


NS_IMETHODIMP
nkWindowingService::CreateWindow(nkIWindow** rv)
{
  HINSTANCE instance = GetModuleHandle(NULL);

  // register the window class
  // don't bother checking to see if window class registration fails
  // if it does fail, window creation will
  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));
  wc.lpfnWndProc   = nkWindow::WindowProc;
  wc.hInstance     = instance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.lspzClassName = "NaikaiWindow";
  RegisterClass(&wc);

  HWND window = CreateWindow(
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

  ShowWindow(window, SW_SHOW);
  UpdateWindow(window);

  *rv = new nkWindow(window);
  NS_ADDREF(*rv);
  return NS_OK;
}


NS_IMETHODIMP
nkWindowingService::CreateMenu(nkIMenu** rv)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindowingService::CreateMenuItem(nkICommand** rv)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindowingService::GetOnIdle(nkICommand** on_idle)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindowingService::SetOnIdle(nkICommand* on_idle)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindowingService::Run()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}


NS_IMETHODIMP
nkWindowingService::PumpEvents()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
