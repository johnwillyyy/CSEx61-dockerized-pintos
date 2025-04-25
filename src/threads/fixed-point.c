#include <stdint.h>  
#include "fixed-point.h"

// Initialize a real number with an integer value
void real_init(fixed_t *r, int value) {
    r->value = value * F;  // Scale the integer value to fixed-point
}

// Convert a real number to an integer (rounding toward zero)
int real_to_int(fixed_t *r) {
    return r->value / F;  // Convert back to integer
}

// Convert a real number to an integer (rounding to the nearest)
int real_to_nearst_int(fixed_t *r) {
    return (r->value + (F/2))/ F;  
}

// Add two real numbers
fixed_t real_add(fixed_t *r1, fixed_t *r2) {
    fixed_t result;
    result.value = r1->value + r2->value;  // Add the fixed-point values
    return result;
}

// Add a real number to an integer
fixed_t real_add_to_int(fixed_t *r1, int n) {
    fixed_t result;
    result.value = r1->value + (n * F);  // Add the fixed-point value to int
    return result;
}

// Subtract two real numbers
fixed_t real_subtract(fixed_t *r1, fixed_t *r2) {
    fixed_t result;
    result.value = r1->value - r2->value;  // Subtract the fixed-point values
    return result;
}

// Multiply two real numbers
fixed_t real_multiply(fixed_t *r1, fixed_t *r2) {
    fixed_t result;
    result.value = (int64_t) r1->value * r2->value / F;  // Multiply and scale
    return result;
}

// Divide two real numbers
fixed_t real_divide(fixed_t *r1, fixed_t *r2) {
    fixed_t result;
    result.value = (int64_t) r1->value * F / r2->value;  // Divide and scale
    return result;
}

// Multiply a real number by an integer
fixed_t real_multiply_by_int(fixed_t *r, int n) {
    fixed_t result;
    result.value = r->value * n;  // Multiply the fixed-point value by an integer
    return result;
}

// Divide a real number by an integer
fixed_t real_divide_by_int(fixed_t *r, int n) {
    fixed_t result;
    result.value = r->value / n;  // Divide the fixed-point value by an integer
    return result;
}
