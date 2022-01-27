#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

dev_t dev=0;

static int __init enter_init(void)
{
if((alloc_chrdev_region(&dev,0,1,"Dynamic_Driver"))<0)
{
printk("Allocation Failed\n");
return -1;
}
printk("Major=%d Minor=%d\n",MAJOR(dev),MINOR(dev));
printk("Module Inserted\n");
return 0;
}

static void __exit bye_exit(void)
{
unregister_chrdev_region(dev,1);
printk("Module Removed\n");
}

module_init(enter_init);
module_exit(bye_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");

