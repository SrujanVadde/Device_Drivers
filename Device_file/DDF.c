#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>


dev_t dev=0;
static struct class *dev_class;


static int __init enter_init(void)
{
if((alloc_chrdev_region(&dev,0,1,"Dynamic_Driver"))<0)
{
printk("Allocation Failed\n");
return -1;
}
printk("Major=%d Minor=%d\n",MAJOR(dev),MINOR(dev));
if((dev_class=class_create(THIS_MODULE,"my_driver"))==NULL)
{
pr_err("cannot create the class\n");
goto r_class;
}

if((device_create(dev_class,NULL,dev,NULL,"my_driver"))==NULL)
{
pr_err("cannot create the device\n");
goto r_device;
}
printk("Module Inserted\n");
return 0;
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

static void __exit bye_exit(void)
{
device_destroy(dev_class,dev);
class_destroy(dev_class);
unregister_chrdev_region(dev,1);
printk("Module Removed\n");
}

module_init(enter_init);
module_exit(bye_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");

