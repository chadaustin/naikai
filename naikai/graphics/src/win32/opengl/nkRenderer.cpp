#include <stdio.h>
#include "nkRenderer.h"


nkRenderer::nkRenderer()
{
}


nkRenderer::~nkRenderer()
{
}


NS_IMPL_ISUPPORTS1(nkRenderer, nkIRenderer)


NS_IMETHODIMP
nkRenderer::Attach(nkIWindow* window)
{
  return NS_OK;
}


NS_IMETHODIMP
nkRenderer::Detach()
{
  return NS_OK;
}


NS_IMETHODIMP
nkRenderer::Draw()
{
  return NS_OK;
}
