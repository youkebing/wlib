/*
 * (c)	2011 wzt
 *
 */

#include <wos/kernel.h>
#include <wos/gdt.h>
#include <wos/task.h>
#include <wos/string.h>
#include <wos/cpu.h>
#include <wos/type.h>

X86_REG *get_cpu_info(int orig_eax)
{
        X86_REG *reg;

        reg = (X86_REG *)kmalloc(sizeof(X86_REG));
        if (!reg)
                return NULL;

        asm("cpuid"
                :"=a"(reg->eax), "=b"(reg->ebx), "=c"(reg->ecx), "=d"(reg->edx)
                :"a"(orig_eax));

        return reg;
}

int get_vendor_id(void)
{
        X86_REG *reg;

        if ((reg = get_cpu_info(0)) == NULL)
                return -1;
/*
        printk("eax=0x%08x\tebx=0x%08x\tecx=0x%08x\tedx=0x%08x\n",
                reg->eax, reg->ebx, reg->ecx, reg->edx);
*/

        memcpy(x86_cpu_info->vendor, (char *)&(reg->ebx), 4);
        memcpy(x86_cpu_info->vendor + 4, (char *)&(reg->edx), 4);
        memcpy(x86_cpu_info->vendor + 8, (char *)&(reg->ecx), 4);
        *(x86_cpu_info->vendor + 12) = 0;

        strcpy(x86_cpu_info->processor, intel64_ia32_processors[reg->eax]);

        return 0;
}

int get_extend_info(void)
{
        X86_REG *reg;

        reg = get_cpu_info(0x80000000);
        if (!reg)
                return -1;

        printk("eax=0x%08x\tebx=0x%08x\tecx=0x%08x\tedx=0x%08x\n",
                reg->eax, reg->ebx, reg->ecx, reg->edx);
        //printk("%s\n", (unsigned char *)&(reg->eax));
        return 0;
}

/* get Model, Family, Stepping information. */
int get_mfsi_info(void)
{
        X86_REG *reg;

        if ((reg = get_cpu_info(1)) == NULL);
                return -1;

/*
        printk("eax=0x%08x\tebx=0x%08x\tecx=0x%08x\tedx=0x%08x\n",
                reg->eax, reg->ebx, reg->ecx, reg->edx);
*/

        x86_cpu_info->stepping = (reg->eax) & 0xf;
        x86_cpu_info->model = ((reg->eax) & 0xf0) >> 4;
        x86_cpu_info->family = ((reg->eax) >> 8) & 0xf;

        x86_cpu_info->clflush_cache_size = ((reg->ebx) >> 8) & 0xff;
        x86_cpu_info->local_apic_id = ((reg->ebx) >> 16) & 0xffff;

        return 0;
}

int __get_tcp_info(unsigned int value)
{
        unsigned int instruction_tlb;
        unsigned int data_tlb;
        unsigned int cache_size;

        if (!value || (value & 0x80000000))
                return -1;

        instruction_tlb = (value >> 8) & 0xff;
        data_tlb = (value >> 16) & 0xff;
        cache_size = (value >> 24) & 0xff;

        //printk("0x%x, 0x%x, 0x%x\n", instruction_tlb, data_tlb, cache_size);
        return 0;
}

/* get TLB, Cache, Prefetch information. */
int get_tcp_info(void)
{
        X86_REG *reg;

        reg = get_cpu_info(2);
        if (!reg)
                return -1;

/*
        printk("eax=0x%08x\tebx=0x%08x\tecx=0x%08x\tedx=0x%08x\n",
                reg->eax, reg->ebx, reg->ecx, reg->edx);
*/

        /* cpuid just execute once. */
        if (((reg->eax) & 0xf) != 1)
                return -1;

        __get_tcp_info(reg->eax);
        __get_tcp_info(reg->ebx);
        __get_tcp_info(reg->ecx);
        __get_tcp_info(reg->edx);

        return 0;
}

void show_x86_cpu_info(void)
{
        printk("vendor: %s\n", x86_cpu_info->vendor);
        printk("processor: %s\n", x86_cpu_info->processor);
        printk("stepping: %d\n", x86_cpu_info->stepping);
        printk("model: %d\n", x86_cpu_info->model);
        printk("family: %d\n", x86_cpu_info->family);
        printk("clflush cache size: %d\n", x86_cpu_info->clflush_cache_size);
        printk("local apic id: %d\n", x86_cpu_info->local_apic_id);
}

int init_x86_cpu_info(void)
{
        x86_cpu_info = (X86_CPU_INFO *)kmalloc(sizeof(X86_CPU_INFO));
        if (!x86_cpu_info)
                return -1;

        return 0;
}

void init_cpu(void)
{
	init_x86_cpu_info();
	get_vendor_id();
	get_mfsi_info();
	get_tcp_info();
	show_x86_cpu_info();
}
