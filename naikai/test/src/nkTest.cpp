#include <stdio.h>
#include <stdlib.h>
#include "nsCOMPtr.h"
#include "nsIGenericFactory.h"
#include "nsIServiceManagerUtils.h"
#include "nsLiteralString.h"
#include "nsString2.h"
#include "nkIRenderer.h"
#include "nkIRunnable.h"
#include "nkIWindow.h"
#include "nkIWindowingService.h"


class ExitCommand : public nkICommand
{
public:
  ExitCommand() {
    NS_INIT_REFCNT();
  }

  NS_DECL_ISUPPORTS
  NS_DECL_NKICOMMAND
};

NS_IMPL_ISUPPORTS1(ExitCommand, nkICommand)

NS_IMETHODIMP
ExitCommand::Execute(nsISupports* /*caller*/)
{
  exit(0);
  return NS_OK;
}


// 028baccc-1dd2-11b2-b297-9e0223120227
#define NK_TEST_CID \
  { 0x028baccc, 0x1dd2, 0x11b2, \
    { 0xb2, 0x97, 0x9e, 0x02, 0x23, 0x12, 0x02, 0x27 } }

#define NK_TEST_CONTRACTID "@naikai.aegisknight.org/test;1"


class Runnable : public nkIRunnable
{
public:
  Runnable();

  NS_DECL_ISUPPORTS
  NS_DECL_NKIRUNNABLE
};


Runnable::Runnable()
{
  NS_INIT_REFCNT();
}

NS_IMPL_ISUPPORTS1(Runnable, nkIRunnable)

NS_IMETHODIMP
Runnable::Run()
{
  nsresult rv;
  nsCOMPtr<nkIWindowingService> ws =
    do_GetService("@naikai.aegisknight.org/windowing;1", &rv);
  if (!ws) {
    return NS_ERROR_FAILURE;
  }

  // create window
  nsCOMPtr<nkIWindow> window;
  rv = ws->CreateFrameWindow(getter_AddRefs(window));
  if (!window || NS_FAILED(rv)) {
    return NS_ERROR_FAILURE;
  }

  // create menu
  nsCOMPtr<nkIMenu> menu;
  rv = ws->CreateMenu(getter_AddRefs(menu));
  if (!menu || NS_FAILED(rv)) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nkIMenu> submenu;
  rv = ws->CreateMenu(getter_AddRefs(submenu));
  if (!submenu || NS_FAILED(rv)) {
    return NS_ERROR_FAILURE;
  }

  submenu->AppendItem(NS_LITERAL_STRING("New").get(), NULL);
  submenu->AppendSeparator();

  nsCOMPtr<nkICommand> ec(new ExitCommand());
  submenu->AppendItem(NS_LITERAL_STRING("Exit").get(), ec);

  menu->AppendSubMenu(NS_LITERAL_STRING("Game").get(), submenu);

  rv = window->SetMenu(menu);
  if (NS_FAILED(rv)) {
    return NS_ERROR_FAILURE;
  }

  // create renderer
  nsCOMPtr<nkIRenderer> renderer(
    do_CreateInstance("@naikai.aegisknight.org/renderer;1", &rv));
  if (!renderer) {
    return NS_ERROR_FAILURE;
  }

  rv = renderer->Attach(window);
  if (NS_FAILED(rv)) {
    return NS_ERROR_FAILURE;
  }

  return ws->Run();
}


NS_GENERIC_FACTORY_CONSTRUCTOR(Runnable);


static nsModuleComponentInfo components[] =
{ {

  "Naikai Test Component",
  NK_TEST_CID,
  NK_TEST_CONTRACTID,
  RunnableConstructor,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,

} };


NS_IMPL_NSGETMODULE(nkTestModule, components)
