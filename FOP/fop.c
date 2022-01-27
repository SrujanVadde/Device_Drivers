#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>

dev_t dev=0;
static struct class *dev_class;
static struct cdev driver1_cdev;

static int __init driver1_init(void);
static void __exit driver1_exit(void);
static int driver1_open(struct inode *inode,struct file *file);
static int driver1_release(struct inode *inode,struct file *file);
static ssize_t driver1_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t driver1_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

static struct file_operations fops=
{
.owner=THIS_MODULE,
.read=driver1_read,
.write=driver1_write,
.open=driver1_open,
.release=driver1_release,
};

static int driver1_open(struct inode *inode, struct file *file)
{
pr_info("Driver Open Function Called...!!!\n");
return 0;
}

static int driver1_release(struct inode *inode, struct file *file)
{
pr_info("Driver Release Function Called...!!!\n");
return 0;
}

static ssize_t driver1_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
pr_info("Driver Read Function Called...!!!\n");
return 0;
}

static ssize_t driver1_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
pr_info("Driver Write Function Called...!!!\n");
return len;
}

static int __init driver1_init(void)
{
if((alloc_chrdev_region(&dev,0,1,"my_driver_1"))<0)
{
pr_err("cannot allocate major number\n");
return -1;
}

printk("Major =%d Minor=%d \n",MAJOR(dev),MINOR(dev));
cdev_init(&driver1_cdev,&fops);

if((cdev_add(&driver1_cdev,dev,1))<0)
{
pr_err("cannot add device to the system\n");
goto r_class;
}

if((dev_class=class_create(THIS_MODULE,"my_driver_1"))==NULL)
{
pr_err("cannot create struct class\n");

goto r_class;
}

if((device_create(dev_class,NULL,dev,NULL,"my_driver_1"))==NULL)
{
pr_err("cannot create device\n");
goto r_device;
}
printk("Driver Inserted...\n");
return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}


static void __exit driver1_exit(void)
{
device_destroy(dev_class,dev);
class_destroy(dev_class);
cdev_del(&driver1_cdev);
unregister_chrdev_region(dev,1);
printk("Driver removed..\n");
}

module_init(driver1_init);
module_exit(driver1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");





