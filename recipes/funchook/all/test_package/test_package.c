#include <stdio.h>
#include <stdlib.h>

#include <funchook.h>

#if defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#else
#  define NOINLINE __attribute__((noinline))
#endif

static int (*real_add)(int, int);

NOINLINE static int add(int a, int b) {
    volatile int x = a;
    volatile int y = b;
    return x + y;
}

NOINLINE static int hooked_add(int a, int b) { return real_add(a, b) + 100; }

int main(void) {
    real_add = add;

    funchook_t *fh = funchook_create();
    if (fh == NULL) {
        fprintf(stderr, "funchook_create failed\n");
        return EXIT_FAILURE;
    }

    int rv = funchook_prepare(fh, (void **)&real_add, hooked_add);
    if (rv != FUNCHOOK_ERROR_SUCCESS) {
        fprintf(stderr, "funchook_prepare failed: %s\n", funchook_error_message(fh));
        return EXIT_FAILURE;
    }

    rv = funchook_install(fh, 0);
    if (rv != FUNCHOOK_ERROR_SUCCESS) {
        fprintf(stderr, "funchook_install failed: %s\n", funchook_error_message(fh));
        return EXIT_FAILURE;
    }

    int result = add(2, 3);
    printf("hooked add(2, 3) = %d (expected 105)\n", result);

    funchook_uninstall(fh, 0);
    funchook_destroy(fh);

    return result == 105 ? EXIT_SUCCESS : EXIT_FAILURE;
}
