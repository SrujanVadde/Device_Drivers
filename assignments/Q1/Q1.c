#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define mem_size 1024

dev_t dev=0;
static struct class *dev_class;
static struct cdev driver2_cdev;

static int __init driver2_init(void);
static void __exit driver2_exit(void);
static int driver2_open(struct inode *inode,struct file *file);
static int driver2_release(struct inode *inode,struct file *file);

static struct file_operations fops =
{
.owner=THIS_MODULE,
.open=driver2_open,
.release=driver2_release,
};


static int driver2_open(struct inode *inode,struct file *file)
{
printk("Device file opened...\n");
return 0;
}

static int driver2_release(struct inode *inode,struct file *file)
{
pr_info("Device file closed...\n");
return 0;
}

static int __init driver2_init(void)
{
if((alloc_chrdev_region(&dev,0,1,"driver2"))<0)
{
pr_info("cannot allocate major number\n");
return -1;
}
 
pr_info("Major number =%d minor number = %d ",MAJOR(dev),MINOR(dev));

cdev_init(&driver2_cdev,&fops);

if((cdev_add(&driver2_cdev,dev,1))<0)
{
pr_info("cannot add device to the system\n");
goto r_class;
}

if((dev_class=class_create(THIS_MODULE,"driver2"))==NULL)
{
pr_info("cannot create the struct class\n");
goto r_class;
}

if((device_create(dev_class,NULL,dev,NULL,"driver2"))==NULL)
{
pr_info("cannot create the device\n");
goto r_device;
}


pr_info("driver Inserted\n");
return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

static void __exit driver2_exit(void)
{

device_destroy(dev_class,dev);
class_destroy(dev_class);
cdev_del(&driver2_cdev);
unregister_chrdev_region(dev,1);
pr_info("Driver closed\n");
}

module_init(driver2_init);
module_exit(driver2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");

