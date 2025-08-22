#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <cstdio>

extern "C" {
    void assertFail(const char *exp, const char *file, int line) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Assertion failed: %s in file %s, line %d", exp, file, line);
        throw std::runtime_error(buffer);
    }
}
