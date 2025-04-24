#define F (1 << 14)  // 2^14 = 16384 (a common choice for 14-bit fractional precision)

struct real{
    int value;
   };
 
// Initialize a real number with an integer value
void real_init(struct real *r, int value);

// Convert a real number to an integer (rounding toward zero)
int real_to_int(struct real *r);

// Add two real numbers
struct real real_add(struct real *r1, struct real *r2);

// Subtract two real numbers
struct real real_subtract(struct real *r1, struct real *r2);

// Multiply two real numbers
struct real real_multiply(struct real *r1, struct real *r2);

// Divide two real numbers
struct real real_divide(struct real *r1, struct real *r2);

// Multiply a real number by an integer
struct real real_multiply_by_int(struct real *r, int n);

// Divide a real number by an integer
struct real real_divide_by_int(struct real *r, int n);
