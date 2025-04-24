#include <stdint.h>  
#include "fixed-point.h"

// Initialize a real number with an integer value
void real_init(struct real *r, int value) {
    r->value = value * F;  // Scale the integer value to fixed-point
}

// Convert a real number to an integer (rounding toward zero)
int real_to_int(struct real *r) {
    return r->value / F;  // Convert back to integer
}

// Add two real numbers
struct real real_add(struct real *r1, struct real *r2) {
    struct real result;
    result.value = r1->value + r2->value;  // Add the fixed-point values
    return result;
}

// Subtract two real numbers
struct real real_subtract(struct real *r1, struct real *r2) {
    struct real result;
    result.value = r1->value - r2->value;  // Subtract the fixed-point values
    return result;
}

// Multiply two real numbers
struct real real_multiply(struct real *r1, struct real *r2) {
    struct real result;
    result.value = (int64_t) r1->value * r2->value / F;  // Multiply and scale
    return result;
}

// Divide two real numbers
struct real real_divide(struct real *r1, struct real *r2) {
    struct real result;
    result.value = (int64_t) r1->value * F / r2->value;  // Divide and scale
    return result;
}

// Multiply a real number by an integer
struct real real_multiply_by_int(struct real *r, int n) {
    struct real result;
    result.value = r->value * n;  // Multiply the fixed-point value by an integer
    return result;
}

// Divide a real number by an integer
struct real real_divide_by_int(struct real *r, int n) {
    struct real result;
    result.value = r->value / n;  // Divide the fixed-point value by an integer
    return result;
}
