#ifndef NK_RENDERER_H
#define NK_RENDERER_H


#include <windows.h>
#include "nkIRenderer.h"


#define NK_RENDERER_CID \
  { 0x9b7f9f26, 0x1dd1, 0x11b2, \
    { 0xbb, 0xf4, 0xfe, 0x65, 0xf2, 0x09, 0x9a, 0x41 } }

#define NK_RENDERER_CONTRACTID "@naikai.aegisknight.org/renderer;1"


class nkRenderer : public nkIRenderer
{
public:
  nkRenderer();
  ~nkRenderer();

  NS_DECL_ISUPPORTS
  NS_DECL_NKIRENDERER

  nsresult Resize();

private:
  nsresult Setup();

private:
  HWND  m_window;
  HDC   m_dc;
  HGLRC m_rc;
};


#endif
