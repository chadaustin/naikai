#include "nsIGenericFactory.h"
#include "nkWindowingService.h"


NS_GENERIC_FACTORY_CONSTRUCTOR(nkWindowingService)


static nsModuleComponentInfo components[] =
{ {

  "Naikai Windowing System (Win32) Component",
  NK_WINDOWING_SERVICE_CID,
  NK_WINDOWING_SERVICE_CONTRACTID,
  nkWindowingServiceConstructor,

} };


NS_IMPL_NSGETMODULE(nkWindowingSystemModule, components)
