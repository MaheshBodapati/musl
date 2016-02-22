#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	int dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %2\n"
		"	bne %0, %3, 1f\n"
		"	addu %1, %4, $0\n"
		"	sc %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		"1:	\n"
		".set pop\n"
		: "=&r"(t), "=&r"(dummy), "+m"(*p) : "r"(t), "r"(s) : "memory" );
        return t;
}

#define a_cas_p a_cas_p
static inline void *a_cas_p(volatile void *p, void *t, void *s)
{
	void *dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	lld %0, %2\n"
		"	bne %0, %3, 1f\n"
		"	daddu %1, %4, $0\n"
		"	scd %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		"1:	\n"
		".set pop\n"
		: "=&r"(t), "=&r"(dummy), "+m"(*(long*)p) : "r"(t), "r"(s) : "memory" );
        return t;
}
#define a_swap a_swap
static inline int a_swap(volatile int *x, int v)
{
	int old, dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %2\n"
		"	addu %1, %3, $0\n"
		"	sc %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(old), "=&r"(dummy), "+m"(*x) : "r"(v) : "memory" );
        return old;
}
#define a_fetch_add a_fetch_add
static inline int a_fetch_add(volatile int *x, int v)
{
	int old, dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %2\n"
		"	addu %1, %0, %3\n"
		"	sc %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(old), "=&r"(dummy), "+m"(*x) : "r"(v) : "memory" );
        return old;
}
#define a_inc a_inc
static inline void a_inc(volatile int *x)
{
	int dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %1\n"
		"	addu %0, %0, 1\n"
		"	sc %0, %1\n"
		"	beq %0, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(dummy), "+m"(*x) : : "memory" );
}
#define a_dec a_dec
static inline void a_dec(volatile int *x)
{
	int dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %1\n"
		"	subu %0, %0, 1\n"
		"	sc %0, %1\n"
		"	beq %0, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(dummy), "+m"(*x) : : "memory" );
}
#define a_store a_store
static inline void a_store(volatile int *p, int x)
{
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"	sw %1, %0\n"
		"	sync\n"
		".set pop\n"
		: "+m"(*p) : "r"(x) : "memory" );
}

#define a_spin a_barrier
#define a_barrier a_barrier
static inline void a_barrier()
{
	a_cas(&(int){0}, 0, 0);
}
#define a_crash a_crash
static inline void a_crash()
{
	*(volatile char *)0=0;
}
#define a_and a_and
static inline void a_and(volatile int *p, int v)
{
	int dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %1\n"
		"	and %0, %0, %2\n"
		"	sc %0, %1\n"
		"	beq %0, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(dummy), "+m"(*p) : "r"(v) : "memory" );
}
#define a_or a_or
static inline void a_or(volatile int *p, int v)
{
	int dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %1\n"
		"	or %0, %0, %2\n"
		"	sc %0, %1\n"
		"	beq %0, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(dummy), "+m"(*p) : "r"(v) : "memory" );
}
#define a_or_l a_or_l
static inline void a_or_l(volatile void *p, long v)
{
	long dummy;
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"	sync\n"
		"1:	lld %0, %1\n"
		"	or %0, %0, %2\n"
		"	scd %0, %1\n"
		"	beq %0, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(dummy), "+m"(*(long*)p) : "r"(v) : "memory" );
}
#define a_and_64 a_and_64
static inline void a_and_64(volatile uint64_t *p, uint64_t v)
{
        long dummy;
        __asm__ __volatile__(
                ".set push\n"
                ".set noreorder\n"
                "       sync\n"
                "1:     lld %0, %1\n"
                "       and %0, %0, %2\n"
                "       scd %0, %1\n"
                "       beq %0, $0, 1b\n"
                "       nop\n"
                "       sync\n"
                ".set pop\n"
                : "=&r"(dummy), "+m"(*p) : "r"(v) : "memory" );
}
#define a_or_64 a_or_64
static inline void a_or_64(volatile uint64_t *p, uint64_t v)
{
        long dummy;
        __asm__ __volatile__(
                ".set push\n"
                ".set noreorder\n"
                "       sync\n"
                "1:     lld %0, %1\n"
                "       or %0, %0, %2\n"
                "       scd %0, %1\n"
                "       beq %0, $0, 1b\n"
                "       nop\n"
                "       sync\n"
                ".set pop\n"
                : "=&r"(dummy), "+m"(*p) : "r"(v) : "memory" );
}

