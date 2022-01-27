#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int __init first_init(void)
{
    printk("\nModule loaded\n");
    return 0;
}

static void __exit second_exit(void)
{
    printk("\nModule removed \n");
}

module_init(first_init);
module_exit(second_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAI SRUJAN");
