#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

int add(int ,int);

int add(int a,int b)
{
    return (a+b);
}

EXPORT_SYMBOL(add);

static int __init enter(void)
{
    printk("\n*********Module Inserted***********\n");
    return 0;
}

static void __exit bye(void)
{
    printk("\n\n*********Module removed***********\n");
}

module_init(enter);
module_exit(bye);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sai Srujan");

