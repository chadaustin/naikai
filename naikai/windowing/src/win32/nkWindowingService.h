#ifndef NAIKAI_WINDOWING_SERVICE_H
#define NAIKAI_WINDOWING_SERVICE_H


#include "nsCOMPtr.h"
#include "nkIWindowingService.h"


#define NK_WINDOWING_SERVICE_CID \
  { 0xfc0784f8, 0x1dd1, 0x11b2,  \
    { 0x99, 0x68, 0xf8, 0xa8, 0x3f, 0xb0, 0x77, 0x4b } }

#define NK_WINDOWING_SERVICE_CONTRACTID "@naikai.aegisknight.org/windowing;1"


class nkWindowingService : public nkIWindowingService
{
public:
  nkWindowingService();

  NS_DECL_ISUPPORTS
  NS_DECL_NKIWINDOWINGSERVICE

private:
  nsCOMPtr<nkICommand> m_on_idle;
};


#endif
