#define F (1 << 14)  // 2^14 = 16384 (a common choice for 14-bit fractional precision)

typedef struct fixed_t
{
    int value;
}fixed_t;
 
// Initialize a real number with an integer value
void real_init(fixed_t *r, int value);

// Convert a real number to an integer (rounding toward zero)
int real_to_int(fixed_t *r);

// Add a real number to an integer
fixed_t real_add_to_int(fixed_t *r1, int n);

// Add two real numbers
fixed_t real_add(fixed_t *r1, fixed_t *r2);

// Subtract two real numbers
fixed_t real_subtract(fixed_t *r1, fixed_t *r2);

// Multiply two real numbers
fixed_t real_multiply(fixed_t *r1, fixed_t *r2);

// Divide two real numbers
fixed_t real_divide(fixed_t *r1, fixed_t *r2);

// Multiply a real number by an integer
fixed_t real_multiply_by_int(fixed_t *r, int n);

// Divide a real number by an integer
fixed_t real_divide_by_int(fixed_t *r, int n);
