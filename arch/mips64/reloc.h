#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ENDIAN_SUFFIX "el"
#else
#define ENDIAN_SUFFIX ""
#endif

#ifdef __mips_soft_float
#define FP_SUFFIX "-sf"
#else
#define FP_SUFFIX ""
#endif

#define LDSO_ARCH "mips64" ENDIAN_SUFFIX FP_SUFFIX

#define TPOFF_K (-0x7000)

#define REL_SYM_OR_REL  4611		/* (R_MIPS_64 << 8) | R_MIPS_REL32 */
#define REL_PLT         R_MIPS_JUMP_SLOT
#define REL_COPY        R_MIPS_COPY
#define REL_DTPMOD      R_MIPS_TLS_DTPMOD64
#define REL_DTPOFF      R_MIPS_TLS_DTPREL64
#define REL_TPOFF       R_MIPS_TLS_TPREL64

#define NEED_MIPS_GOT_RELOCS 1
#define DT_DEBUG_INDIRECT DT_MIPS_RLD_MAP
#define ARCH_SYM_REJECT_UND(s) (!((s)->st_other & STO_MIPS_PLT))

#define CRTJMP(pc,sp) __asm__ __volatile__( \
	"move $sp,%1 ; jr %0" : : "r"(pc), "r"(sp) : "memory" )



typedef uint32_t Elf32_Word;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Addr;

typedef struct
{
  Elf32_Word    r_sym;          /* Symbol index */
  unsigned char r_ssym;         /* Special symbol for 2nd relocation */
  unsigned char r_type3;        /* 3rd relocation type */
  unsigned char r_type2;        /* 2nd relocation type */
  unsigned char r_type1;        /* 1st relocation type */
} _Elf64_Mips_R_Info;

typedef union
{
  Elf64_Xword   r_info_number;
  _Elf64_Mips_R_Info r_info_fields;
} _Elf64_Mips_R_Info_union;

typedef struct
{
  Elf64_Addr    r_offset;               /* Address */
  _Elf64_Mips_R_Info_union r_info;      /* Relocation type and symbol index */
} Elf64_Mips_Rel;

typedef struct
{
  Elf64_Addr    r_offset;               /* Address */
  _Elf64_Mips_R_Info_union r_info;      /* Relocation type and symbol index */
  Elf64_Sxword  r_addend;               /* Addend */
} Elf64_Mips_Rela;

#define ELF64_MIPS_R_TYPE(i) \
  (((_Elf64_Mips_R_Info_union)(i)).r_info_fields.r_type1 \
   | ((Elf32_Word)(__extension__ (_Elf64_Mips_R_Info_union)(i) \
                   ).r_info_fields.r_type2 << 8) \
   | ((Elf32_Word)(__extension__ (_Elf64_Mips_R_Info_union)(i) \
                   ).r_info_fields.r_type3 << 16) \
   | ((Elf32_Word)(__extension__ (_Elf64_Mips_R_Info_union)(i) \
                   ).r_info_fields.r_ssym << 24))





#define GETFUNCSYM(fp, sym, got) __asm__ ( \
	".hidden " #sym "\n" \
	".set push \n" \
	".set noreorder \n" \
	".align 8 \n" \
	"	bal 1f \n" \
	"	 nop \n" \
	"	.gpdword . \n" \
	"	.gpdword " #sym " \n" \
	"1:	ld %0, ($ra) \n" \
	"	dsubu %0, $ra, %0 \n" \
	"	ld $ra, 8($ra) \n" \
	"	daddu %0, %0, $ra \n" \
	".set pop \n" \
	: "=r"(*(fp)) : : "memory", "ra" )
