#ifndef CPU_H
#define CPU_H

#define NULL            (void *)0

typedef struct x86_reg {
        unsigned int eax;
        unsigned int ebx;
        unsigned int ecx;
        unsigned int edx;
}X86_REG;

typedef struct x86_cpu_info {
        char vendor[32];
        char processor[64];
        unsigned int stepping;
        unsigned int model;
        unsigned int family;
        unsigned int clflush_cache_size;
        unsigned int local_apic_id;
}X86_CPU_INFO;

/* see also Intel(R) 64 and IA-32 ARCHITECTURES Software Developer's Manual, 
   Volume 2A - section CPUID.*/
static const char *intel64_ia32_processors[14] = {
        NULL,
        "Later Intel486 Processors and Pentium Processors",
        "Pentium Pro and Pentium II Processors, Intel® Celeron® Processors",
        "Pentium III Processors",
        NULL,
        "Pentium D Processor (8xx)",
        "Pentium D Processor (9xx)",
        NULL,
        NULL,
        NULL,
        "Intel Core Duo Processor",
        "Intel Core i7 Processor",
        NULL,
        "Intel Core 2 Duo Processor 8000 Series"
        };

X86_CPU_INFO *x86_cpu_info;

#endif
