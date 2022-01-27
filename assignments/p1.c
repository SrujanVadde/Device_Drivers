#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>               
#include<linux/uaccess.h> 

#define mem_size        1024 

dev_t dev=0;

static struct class *dev_class;
static struct cdev d1_cdev1,d1_cdev2;
uint8_t *kernel_buffer;

/*
** Function Prototypes
*/
static int      __init d1_init(void);
static void     __exit d1_exit(void);

static int      d1_1_open(struct inode *inode, struct file *file);
static int      d1_1_release(struct inode *inode, struct file *file);
static ssize_t  d1_1_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  d1_1_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static int      d1_2_open(struct inode *inode, struct file *file);
static int      d1_2_release(struct inode *inode, struct file *file);
static ssize_t  d1_2_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  d1_2_write(struct file *filp, const char *buf, size_t len, loff_t * off);
/*
** File Operations structure
*/
static struct file_operations fops1 =
{
        .owner          = THIS_MODULE,
        .read           = d1_1_read,
        .write          = d1_1_write,
        .open           = d1_1_open,
        .release        = d1_1_release,
};

static struct file_operations fops2 =
{
        .owner          = THIS_MODULE,
        .read           = d1_2_read,
        .write          = d1_2_write,
        .open           = d1_2_open,
        .release        = d1_2_release,
};


static int d1_1_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}


static int d1_1_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}


static ssize_t d1_1_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        }
        pr_info("Data Read : Done!\n");
        return mem_size;
}


static ssize_t d1_1_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        }
        pr_info("Data Write : Done!\n");
        return len;
}

static int d1_2_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}


static int d1_2_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}


static ssize_t d1_2_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        }
        pr_info("Data Read : Done!\n");
        return mem_size;
}


static ssize_t d1_2_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        }
        pr_info("Data Write : Done!\n");
        return len;
}

/* INIT FUNCTION  */

static int __init d1_init(void)
{
int major=0;
dev_t dnum1,dnum2;
        if((alloc_chrdev_region(&dev, 0, 2, "mydriver1_test")) <0)
        {
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        major=MAJOR(dev);
        
         dev_class = class_create(THIS_MODULE, "my_driver_class");
         dnum1=MKDEV(major,0);
         cdev_init(&d1_cdev1, &fops1);
         if((cdev_add(&d1_cdev1,dev,1)) < 0)
        {
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
         if((device_create(dev_class,NULL,dnum1,NULL,"driver123_0")) == NULL)
        {
            pr_info("Cannot create the Device 1\n");
            goto r_device;
        }
        
        
        dev_class = class_create(THIS_MODULE, "my_driver_class");
         dnum2=MKDEV(major,0);
         cdev_init(&d1_cdev2, &fops2);
         if((cdev_add(&d1_cdev1,dev,1)) < 0)
        {
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
         if((device_create(dev_class,NULL,dnum2,NULL,"driver123_1")) == NULL)
        {
            pr_info("Cannot create the Device 1\n");
            goto r_device;
        }
        
        
         if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0)
        {
            pr_info("Cannot allocate memory in kernel\n");
            goto r_device;
        }
        
        strcpy(kernel_buffer, "Hello_World");
        
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
        
        
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,2);
	return -1;
	
}


static void __exit d1_exit(void)
{
int major=0;
dev_t dnum1,dnum2;

kfree(kernel_buffer);



major=MAJOR(dev);
dnum1=MKDEV(major,0);
dnum2=MKDEV(major,1);

cdev_del(&d1_cdev1);
device_destroy(dev_class,dnum1);

cdev_del(&d1_cdev2);
device_destroy(dev_class,dnum2);

class_destroy(dev_class);

unregister_chrdev_region(dev, 2);
pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(d1_init);
module_exit(d1_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");

