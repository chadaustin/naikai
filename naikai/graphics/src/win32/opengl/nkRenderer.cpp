#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "nsCOMPtr.h"
#include "nsIServiceManagerUtils.h"
#include "nkIWindowingService.h"
#include "nkIWindow.h"
#include "nkIWin32Window.h"
#include "nkRenderer.h"


class Painter : public nkICommand
{
public:
  Painter(nkRenderer* renderer) {
    m_renderer = renderer;
  }

  NS_DECL_ISUPPORTS
  NS_DECL_NKICOMMAND

private:
  nsCOMPtr<nkRenderer> m_renderer;
};


NS_IMPL_ISUPPORTS1(Painter, nkICommand)


NS_IMETHODIMP
Painter::Execute(nsISupports* caller)
{
  return m_renderer->Draw();
}


////

class Resizer : public nkICommand
{
public:
  Resizer(nkRenderer* renderer) {
    m_renderer = renderer;
  }

  NS_DECL_ISUPPORTS
  NS_DECL_NKICOMMAND

private:
  nsCOMPtr<nkRenderer> m_renderer;
};


NS_IMPL_ISUPPORTS1(Resizer, nkICommand)


NS_IMETHODIMP
Resizer::Execute(nsISupports* caller)
{
  return m_renderer->Resize();
}


////


nkRenderer::nkRenderer()
{
  m_window = 0;
  m_dc = 0;
  m_rc = 0;
}


nkRenderer::~nkRenderer()
{
  // detach ourselves! (shouldn't be necessary)
  Detach();
}


NS_IMPL_ISUPPORTS1(nkRenderer, nkIRenderer)


NS_IMETHODIMP
nkRenderer::Attach(nkIWindow* window_)
{
  nsresult rv;
  nsCOMPtr<nkIWindowingService> ws(
    do_GetService("@naikai.aegisknight.org/windowing;1", &rv));
  if (NS_FAILED(rv) || !ws) {
    return NS_ERROR_FAILURE;
  }

  // QI to a Win32 window (we aren't allowed to attach this type of
  // renderer to anything else)
  nsCOMPtr<nkIWin32Window> window(do_QueryInterface(window_, &rv));
  if (NS_FAILED(rv) || !window) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  // get HWND
  rv = window->GetHandle(m_window);
  if (NS_FAILED(rv) || !m_window) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  static const PIXELFORMATDESCRIPTOR pfd =  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    24,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    16,
    0,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };

  m_dc = GetDC(m_window);
  if (!m_dc) {
    return NS_ERROR_FAILURE;
  }

  int pixel_format = ChoosePixelFormat(m_dc, &pfd);
  if (!pixel_format) {
    ReleaseDC(m_window, m_dc);
    m_dc = 0;
    return NS_ERROR_FAILURE;
  }

  if (!SetPixelFormat(m_dc, pixel_format, &pfd)) {
    ReleaseDC(m_window, m_dc);
    m_dc = 0;
    return NS_ERROR_FAILURE;
  }

  m_rc = wglCreateContext(m_dc);
  if (!m_rc) {
    ReleaseDC(m_window, m_dc);
    m_dc = 0;
    return NS_ERROR_FAILURE;
  }

  if (!wglMakeCurrent(m_dc, m_rc)) {
    wglDeleteContext(m_rc);
    m_rc = 0;
    ReleaseDC(m_window, m_dc);
    m_dc = 0;
    return NS_ERROR_FAILURE;
  }


  ws->SetOnIdle(new Painter(this));
  window_->SetOnResize(new Resizer(this));

  rv = Resize();
  if (NS_FAILED(rv)) {
    return rv;
  }

  return Setup();
}


NS_IMETHODIMP
nkRenderer::Detach()
{
  if (m_rc) {
    wglMakeCurrent(0, 0);
    wglDeleteContext(m_rc);
    m_rc = 0;
  }

  if (m_dc) {
    ReleaseDC(m_window, m_dc);
    m_dc = 0;
  }

  m_window = 0;  
  return NS_OK;
}


NS_IMETHODIMP
nkRenderer::Draw()
{
  static const struct {
    int x, y, z;
  } vertices[] = {
    {  1,  1,  1 },
    {  1,  1, -1 },
    {  1, -1, -1 },
    {  1, -1,  1 },
    { -1,  1,  1 },
    { -1,  1, -1 },
    { -1, -1, -1 },
    { -1, -1,  1 },
  };

  static const int faces[6][4] = {
    { 0, 1, 2, 3 },
    { 4, 5, 6, 7 },
    { 0, 1, 5, 4 },
    { 3, 2, 6, 7 },
    { 0, 3, 7, 4 },
    { 1, 2, 6, 5 },
  };

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  DWORD t = GetTickCount();
  glTranslatef(0, 0, -10);
  glRotatef(t / 25.0f, 1, 0, 0);
  glRotatef(t / 20.0f, 0, 1, 0);
  glRotatef(t / 15.0f, 0, 0, 1);

  glBegin(GL_QUADS);
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 4; ++j) {
      int f = faces[i][j];
      int x = vertices[f].x;
      int y = vertices[f].y;
      int z = vertices[f].z;
      glColor3f(sin(x * t / 500.0),
		sin(y * t / 700.0),
		sin(z * t / 900.0));
      glVertex3f(x, y, z);
    }
  }
  glEnd();

  SwapBuffers(m_dc);

  return NS_OK;
}


nsresult
nkRenderer::Resize()
{
  RECT client_rect;
  if (!GetClientRect(m_window, &client_rect)) {
    return NS_ERROR_FAILURE;
  }

  int width = client_rect.right;
  int height = client_rect.bottom;

  if (height == 0) {
    height = 1;
  }

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, GLfloat(width) / height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();		

  return NS_OK;
}


nsresult
nkRenderer::Setup()
{
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
  return NS_OK;
}
