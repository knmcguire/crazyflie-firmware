#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// Test function declarations
int test_controller_mellinger(void);
int test_controller_pid(void);
int test_math3d(void);
int test_planner(void);
int test_pptraj(void);
int test_kalman_core(void);
int test_power_distribution(void);

typedef struct {
    const char* name;
    int (*test_func)(void);
} test_case_t;

// Test registry
static test_case_t test_cases[] = {
    {"controller_mellinger", test_controller_mellinger},
    {"controller_pid", test_controller_pid},
    {"math3d", test_math3d},
    {"planner", test_planner},
    {"pptraj", test_pptraj},
    {"kalman_core", test_kalman_core},
    {"power_distribution", test_power_distribution},
};

static const int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

void run_all_tests(void) {
    int passed = 0;
    int failed = 0;
    
    printf("=== Crazyflie Firmware Standalone Tests (Pure C) ===\n");
    printf("Running %d test suites...\n\n", num_tests);
    
    for (int i = 0; i < num_tests; i++) {
        printf("Running %s... ", test_cases[i].name);
        fflush(stdout);
        
        double start_time = get_time_ms();
        int result = test_cases[i].test_func();
        double end_time = get_time_ms();
        
        if (result == 0) {
            printf("PASSED (%.3f ms)\n", end_time - start_time);
            passed++;
        } else {
            printf("FAILED (%.3f ms)\n", end_time - start_time);
            failed++;
        }
    }
    
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", num_tests);
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    
    if (failed > 0) {
        printf("Some tests failed!\n");
        exit(1);
    } else {
        printf("All tests passed!\n");
    }
}

void run_specific_test(const char* test_name) {
    for (int i = 0; i < num_tests; i++) {
        if (strcmp(test_cases[i].name, test_name) == 0) {
            printf("Running %s... ", test_name);
            fflush(stdout);
            
            double start_time = get_time_ms();
            int result = test_cases[i].test_func();
            double end_time = get_time_ms();
            
            if (result == 0) {
                printf("PASSED (%.3f ms)\n", end_time - start_time);
                exit(0);
            } else {
                printf("FAILED (%.3f ms)\n", end_time - start_time);
                exit(1);
            }
        }
    }
    
    printf("Test '%s' not found!\n", test_name);
    printf("Available tests:\n");
    for (int i = 0; i < num_tests; i++) {
        printf("  %s\n", test_cases[i].name);
    }
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        run_specific_test(argv[1]);
    } else {
        run_all_tests();
    }
    
    return 0;
}
