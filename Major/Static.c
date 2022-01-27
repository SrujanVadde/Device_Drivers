#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>

dev_t dev=MKDEV(200,0);

static int __init enter_init(void)
{
register_chrdev_region(dev,1,"My_Driver");
printk("Major=%d Minor=%d \n",MAJOR(dev),MINOR(dev));
printk("Kernel Module Inserted\n");
return 0;
}

static void __exit bye_exit(void)
{
unregister_chrdev_region(dev,1);
printk("Module removed\n");
}

module_init(enter_init);
module_exit(bye_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");


