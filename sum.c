#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include "add.h"
static int a=10;
static int b=20;

static int __init load(void)
{
    printk("\n****sum is %d:  ***\n",add(a,b));
    
    return 0;
}

static void __exit bye(void)
{
    printk("\n\n*********BYE***********\n");
}

module_init(load);
module_exit(bye);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAI SRUJAN");