#include "nsIGenericFactory.h"
#include "nkRenderer.h"


NS_GENERIC_FACTORY_CONSTRUCTOR(nkRenderer)


static nsModuleComponentInfo components[] =
{ {

  "Naikai Rendering Component",
  NK_RENDERER_CID,
  NK_RENDERER_CONTRACTID,
  nkRendererConstructor,

} };


NS_IMPL_NSGETMODULE(nkRenderingModule, components)
