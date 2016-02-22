#ifndef _INTERNAL_RELOC_H
#define _INTERNAL_RELOC_H

#include <features.h>
#include <elf.h>
#include <stdint.h>

#if UINTPTR_MAX == 0xffffffff
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
typedef Elf32_Sym Sym;
#define R_TYPE(x) ((x)&255)
#define R_SYM(x) ((x)>>8)
#define R_INFO	ELF32_R_INFO
#else
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
typedef Elf64_Sym Sym;
#define R_TYPE(x) ((x)&0x7fffffff)
#define R_SYM(x) ((x)>>32)
#define R_INFO	ELF64_R_INFO
#define ELF64 1
#endif

#ifdef __mips64
#undef R_TYPE
#undef R_SYM
#undef R_INFO
#define R_TYPE(INFO) ({										\
	uint64_t r_info = (INFO);								\
	uint32_t *type, r_type;									\
	type = (((uint32_t*) &r_info) + 1);						\
	r_type = (uint32_t) *(((unsigned char*) type) + 3) |	\
	(uint32_t) *(((unsigned char*) type) + 2) << 8 |		\
	(uint32_t) *(((unsigned char*) type) + 1) << 16;		\
	r_type;													\
})

#define R_SYM(INFO) ({						\
	uint64_t r_info = (INFO);				\
	uint32_t symidx;						\
	symidx = *(((uint32_t*) &r_info) + 0);	\
	symidx;									\
})

#define R_INFO(SYM,TYPE1) ({					\
	uint32_t *pinfo;							\
	uint64_t r_info = 0;						\
	pinfo = ((uint32_t*) &r_info) + 1;			\
	*((uint32_t*) &r_info + 0) = (SYM);			\
	*((unsigned char*) pinfo + 3) = (TYPE1);	\
	r_info;										\
})
#endif


/* These enum constants provide unmatchable default values for
 * any relocation type the arch does not use. */
enum {
	REL_NONE = 0,
	REL_SYMBOLIC = -100,
	REL_GOT,
	REL_PLT,
	REL_RELATIVE,
	REL_OFFSET,
	REL_OFFSET32,
	REL_COPY,
	REL_SYM_OR_REL,
	REL_DTPMOD,
	REL_DTPOFF,
	REL_TPOFF,
	REL_TPOFF_NEG,
	REL_TLSDESC,
	REL_FUNCDESC,
	REL_FUNCDESC_VAL,
};

struct fdpic_loadseg {
	uintptr_t addr, p_vaddr, p_memsz;
};

struct fdpic_loadmap {
	unsigned short version, nsegs;
	struct fdpic_loadseg segs[];
};

struct fdpic_dummy_loadmap {
	unsigned short version, nsegs;
	struct fdpic_loadseg segs[1];
};

#include "reloc.h"

#ifndef FDPIC_CONSTDISP_FLAG
#define FDPIC_CONSTDISP_FLAG 0
#endif

#ifndef DL_FDPIC
#define DL_FDPIC 0
#endif

#ifndef DL_NOMMU_SUPPORT
#define DL_NOMMU_SUPPORT 0
#endif

#if !DL_FDPIC
#define IS_RELATIVE(x,s) ( \
	(R_TYPE(x) == REL_RELATIVE) || \
	(R_TYPE(x) == REL_SYM_OR_REL && !R_SYM(x)) )
#else
#define IS_RELATIVE(x,s) ( ( \
	(R_TYPE(x) == REL_FUNCDESC_VAL) || \
	(R_TYPE(x) == REL_SYMBOLIC) ) \
	&& (((s)[R_SYM(x)].st_info & 0xf) == STT_SECTION) )
#endif

#ifndef NEED_MIPS_GOT_RELOCS
#define NEED_MIPS_GOT_RELOCS 0
#endif

#ifndef DT_DEBUG_INDIRECT
#define DT_DEBUG_INDIRECT 0
#endif

#define AUX_CNT 32
#define DYN_CNT 32

typedef void (*stage2_func)(unsigned char *, size_t *);
typedef _Noreturn void (*stage3_func)(size_t *);

#endif
