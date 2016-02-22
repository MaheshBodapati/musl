#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) 0, __SYSCALL_LL_E((x))

__attribute__((visibility("hidden")))
long (__syscall)(long, ...);

#define SYSCALL_RLIM_INFINITY (-1UL/2)

#include <sys/stat.h>
struct kernel_stat {
    unsigned int st_dev;
    unsigned int __pad1[3];
    unsigned long long st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    int st_uid;
    int st_gid;
    unsigned int st_rdev;
    unsigned int __pad2[3];
    long long st_size;
    unsigned int st_atime_sec;
    unsigned int st_atime_nsec;
    unsigned int st_mtime_sec;
    unsigned int st_mtime_nsec;
    unsigned int st_ctime_sec;
    unsigned int st_ctime_nsec;
    unsigned int st_blksize;
    unsigned int __pad3;
    unsigned long long st_blocks;
};

static void __stat_fix(struct kernel_stat *kst, struct stat *st)
{
	extern void *memset(void *s, int c, size_t n);

	st->st_dev = kst->st_dev;
	memset (&st->st_pad1, 0, sizeof (st->st_pad1));
	st->st_ino = kst->st_ino;
	st->st_mode = kst->st_mode;
	st->st_nlink = kst->st_nlink;
	st->st_uid = kst->st_uid;
	st->st_gid = kst->st_gid;
	st->st_rdev = kst->st_rdev;
	memset (&st->st_pad2, 0, sizeof (st->st_pad2));
	st->st_size = kst->st_size;
	st->st_pad3 = 0;
	st->st_atim.tv_sec = kst->st_atime_sec;
	st->st_atim.tv_nsec = kst->st_atime_nsec;
	st->st_mtim.tv_sec = kst->st_mtime_sec;
	st->st_mtim.tv_nsec = kst->st_mtime_nsec;
	st->st_ctim.tv_sec = kst->st_ctime_sec;
	st->st_ctim.tv_nsec = kst->st_ctime_nsec;
	st->st_blksize = kst->st_blksize;
	st->st_blocks = kst->st_blocks;
	memset (&st->st_pad5, 0, sizeof (st->st_pad5));
	return;
}

#ifndef __clang__

static inline long __syscall0(long n)
{
	register long r7 __asm__("$7");
	register long r2 __asm__("$2");
	__asm__ __volatile__ (
		"daddu $2,$0,%2 ; syscall"
		: "=&r"(r2), "=r"(r7) : "ir"(n), "0"(r2), "1"(r7)
		: "$1", "$3", "$8", "$9", "$10", "$11", "$12", "$13",
		  "$14", "$15", "$24", "$25", "hi", "lo", "memory");
	return r7 ? -r2 : r2;
}

static inline long __syscall1(long n, long a)
{
	register long r4 __asm__("$4") = a;
	register long r7 __asm__("$7");
	register long r2 __asm__("$2");
	__asm__ __volatile__ (
		"daddu $2,$0,%2 ; syscall"
		: "=&r"(r2), "=r"(r7) : "ir"(n), "0"(r2), "1"(r7),
		  "r"(r4)
		: "$1", "$3", "$8", "$9", "$10", "$11", "$12", "$13",
		  "$14", "$15", "$24", "$25", "hi", "lo", "memory");
	return r7 ? -r2 : r2;
}

static inline long __syscall2(long n, long a, long b)
{
	struct kernel_stat kst = {0,};
	long ret;
	register long r4 __asm__("$4");
	register long r5 __asm__("$5");
	register long r7 __asm__("$7");
	register long r2 __asm__("$2");

	r5 = b;
	if (n == SYS_stat || n == SYS_fstat || n == SYS_lstat)
		r5 = (long) &kst;

	r4 = a;
	__asm__ __volatile__ (
		"daddu $2,$0,%2 ; syscall"
		: "=&r"(r2), "=r"(r7) : "ir"(n), "0"(r2), "1"(r7),
		  "r"(r4), "r"(r5)
		: "$1", "$3", "$8", "$9", "$10", "$11", "$12", "$13",
		  "$14", "$15", "$24", "$25", "hi", "lo", "memory");

	ret = r7 ? -r2 : r2;
	if (n == SYS_stat || n == SYS_fstat || n == SYS_lstat) 
		__stat_fix(&kst, (struct stat *)b);

	return ret;
}

static inline long __syscall3(long n, long a, long b, long c)
{
	register long r4 __asm__("$4") = a;
	register long r5 __asm__("$5") = b;
	register long r6 __asm__("$6") = c;
	register long r7 __asm__("$7");
	register long r2 __asm__("$2");
	__asm__ __volatile__ (
		"daddu $2,$0,%2 ; syscall"
		: "=&r"(r2), "=r"(r7) : "ir"(n), "0"(r2), "1"(r7),
		  "r"(r4), "r"(r5), "r"(r6)
		: "$1", "$3", "$8", "$9", "$10", "$11", "$12", "$13",
		  "$14", "$15", "$24", "$25", "hi", "lo", "memory");
	return r7 ? -r2 : r2;
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
	register long r4 __asm__("$4") = a;
	register long r5 __asm__("$5") = b;
	register long r6 __asm__("$6") = c;
	register long r7 __asm__("$7") = d;
	register long r2 __asm__("$2");
	__asm__ __volatile__ (
		"daddu $2,$0,%2 ; syscall"
		: "=&r"(r2), "=r"(r7) : "ir"(n), "0"(r2), "1"(r7),
		  "r"(r4), "r"(r5), "r"(r6)
		: "$1", "$3", "$8", "$9", "$10", "$11", "$12", "$13",
		  "$14", "$15", "$24", "$25", "hi", "lo", "memory");
	return r7 ? -r2 : r2;
}

#else

static inline long __syscall0(long n)
{
	return (__syscall)(n);
}

static inline long __syscall1(long n, long a)
{
	return (__syscall)(n, a);
}

static inline long __syscall2(long n, long a, long b)
{
	long r2;
	long old_b = b;
	struct kernel_stat kst = {0,};

	if (n == SYS_stat || n == SYS_fstat || n == SYS_lstat)
		b = (long) &kst;

	r2 = (__syscall)(n, a, b);

	if (r2 > -4096UL) 
		return r2;

	if (n == SYS_stat || n == SYS_fstat || n == SYS_lstat) 
		__stat_fix(kst, old_b);

	return r2;
}

static inline long __syscall3(long n, long a, long b, long c)
{
	return  (__syscall)(n, a, b, c);
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
	return  (__syscall)(n, a, b, c, d);
}

#endif

static inline long __syscall5(long n, long a, long b, long c, long d, long e)
{
	return  (__syscall)(n, a, b, c, d, e);
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
	return (__syscall)(n, a, b, c, d, e, f);
}

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__vdso_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6"
