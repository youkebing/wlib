#include <wos/printk.h>
#include <wos/string.h>
#include <wos/vga.h>

int vfprintf(char *format, va_list arg)
{
	int flag = 0, ret = 0;
	const char *p = format;

	while (*p) {
		switch (*p) {
		case '%':
			if (flag) {
				flag = 0;
				putc(*p);
				ret++;
			}
			else {
				flag = 1;
			}
			break;
		case 'd':
			if (flag) {
				char buf[32];
				flag = 0;

				/* FIXME: can't print 0. */
				itoa(va_arg(arg, int), buf, 10);
				puts(buf);
				ret += strlen(buf);
			}
			else {
				putc(*p);
				ret++;
			}
			break;
                case 'x':
                        if (flag) {
                                char buf[64];
                                flag = 0;

                                itoa(va_arg(arg, int), buf, 16);
                                puts(buf);
                                ret += strlen(buf);
                        }
                        else {
                                putc(*p);
                                ret++;
                        }
                        break;
		case 'b':
                        if (flag) {
                                char buf[16];
                                flag = 0;

                                itoa(va_arg(arg, int), buf, 2);
                                puts(buf);
                                ret += strlen(buf);
                        }
                        else {
                                putc(*p);
                                ret++;
                        }
                        break;
		case 's':
			if (flag) {
				char *str = va_arg(arg, char*);
				flag = 0;
				
				puts(str);
				ret += strlen(str);
			}
			else {
				putc(*p);
				ret++;
			}
			break;
                case 'c':
                        if (flag) {
                                char s = va_arg(arg, char);
                                flag = 0;

                                putc(s);
				ret++;
                        }
                        else {
                                putc(*p);
                                ret++;
                        }
                        break;
		default:
			putc(*p);
			ret++;
			break;
		}
		*p++;
	}

	va_end(arg);
	return ret;
}

int printk(char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	return vfprintf(format, arg);
}

void printk_test(void)
{
/*
	printk("hello, world.\n\n");
	printk("%d\n", 4);
	printk("0");
	printk("0x%x\n", 12345678);
	printk("!%d\n", 0);
	printk("%x\n", 0x0);
*/
}
