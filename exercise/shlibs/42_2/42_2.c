#define _GNU_SOURCE
#include <dlfcn.h>
#include "../lib/tlpi_hdr.h"


int
main(int argc, char *argv[])
{
    void *libHandle;            /* Handle for shared library */
    void (*funcp)(void);        /* Pointer to function with no arguments */
    const char *err;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s lib-path func-name\n", argv[0]);

    /* Load the shared library and get a handle for later use */

    libHandle = dlopen(argv[1], RTLD_LAZY);
    if (libHandle == NULL)
        fatal("dlopen: %s", dlerror());

    /* Search library for symbol named in argv[2] */

    (void) dlerror();                           /* Clear dlerror() */
    *(void **) (&funcp) = dlsym(libHandle, argv[2]);
    err = dlerror();
    if (err != NULL)
        fatal("dlsym: %s", err);

    /* Try calling the address returned by dlsym() as a function
       that takes no arguments */

    (*funcp)();
	Dl_info info;
	if(dladdr(funcp, &info) == 0)
		errExit("dladdr");

	printf("pathname: %s\n", info.dli_fname);
	printf("base address: %p\n", info.dli_fbase);
	printf("name of nearest run-time symbol: %s\n", info.dli_sname);
	printf("actual value of the symbol: %p\n", info.dli_saddr);

    dlclose(libHandle);                         /* Close the library */

    exit(EXIT_SUCCESS);
}
