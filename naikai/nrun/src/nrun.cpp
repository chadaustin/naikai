#include <stdio.h>
#include <stdlib.h>
#include "nsXPCOM.h"
#include "nsIServiceManager.h"
#include "nkIRunnable.h"


static void run(const char* contract_id);


int main(int argc, char** argv)
{
  if (argc <= 1) {
    puts("nrun <contractid>");
    return EXIT_FAILURE;
  }

  const char* contract_id = argv[1];

  // initialize XPCOM
  nsresult rv = NS_InitXPCOM2(nsnull, nsnull, nsnull);
  if (NS_FAILED(rv)) {
    printf("XPCOM initialization failed: [%x]\n", rv);
    return EXIT_FAILURE;
  }

  // check for new components
  nsComponentManager::AutoRegister(nsIComponentManager::NS_Startup, nsnull);

  // run the application
  run(contract_id);
  
  // shut down and go home
  NS_ShutdownXPCOM(nsnull);
  return EXIT_SUCCESS;
}


void run(const char* contract_id)
{
  // create an instance of our component
  nsresult rv;
  nsCOMPtr<nkIRunnable> runnable = do_CreateInstance(contract_id, &rv);
  if (NS_FAILED(rv)) {
    printf("Failed to create component (%s): [%x]\n", contract_id, rv);
    return;
  }

  rv = runnable->Run();
  if (NS_FAILED(rv)) {
    printf("Run failed: [%x]\n", rv);
    return;
  }
}
