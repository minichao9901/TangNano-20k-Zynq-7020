

// Remap all 4 64KB OCM to top of memory starting at 0xFFFC0000
// Open address filtering to include DDR at 0x00000000
#define MY_REMAP()   __asm__ __volatile__(\
    "mov  r5, #0x03                                           \n"\
    "mov  r6, #0                                              \n"\
    "LDR  r7, =0xF8000000  /* SLCR base address    */         \n"\
    "LDR  r8, =0xF8F00000  /* MPCORE base address  */         \n"\
    "LDR  r9, =0x0000767B  /* SLCR lock key        */         \n"\
    "mov  r10,#0x1F                                           \n"\
    "LDR  r11,=0x0000DF0D  /* SLCR unlock key                 \n"\
    "dsb                                                      \n"\
    "isb                   /* make sure it completes */       \n"\
    "pli  do_remap     /* preload the instruction cache */    \n"\
    "pli  do_remap+32                                         \n"\
    "pli  do_remap+64                                         \n"\
    "pli  do_remap+96                                         \n"\
    "pli  do_remap+128                                        \n"\
    "pli  do_remap+160                                        \n"\
    "pli  do_remap+192                                        \n"\
    "isb                   /* make sure it completes */       \n"\
    "b    do_remap                                            \n"\
    ".align 5, 0xFF         /* forces the next block to a cache line alignment */ \n"\
    "do_remap:              /* Unlock SLCR                         */ \n"\
    "str  r11, [r7, #0x8]   /* Configuring OCM remap value         */ \n"\
    "str  r10, [r7, #0x910] /* Lock SLCR                           */ \n"\
    "str  r9,  [r7, #0x4]   /* Disable SCU & address filtering     */ \n"\
    "str  r6,  [r8, #0x0]   /* Set filter start addr to 0x00000000 */ \n"\
    "str  r6,  [r8, #0x40]  /* Enable SCU & address filtering      */ \n"\
    "str  r5,  [r8, #0x0]                                             \n"\
    "dmb                                                              \n"\
   );
