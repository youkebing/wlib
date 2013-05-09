#include <asm/atomic.h>

inline void atomic_inc(unsigned int *x)
{
	asm("LOCK\n\t"
		"incl %0":"=m"(*x):"m"(*x));
}

inline void atomic_sub(unsigned int *x)
{
	asm("LOCK\n\t"
		"decl %0":"=m"(*x):"m"(*x));
}
