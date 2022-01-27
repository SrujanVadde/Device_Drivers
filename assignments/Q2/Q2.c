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
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev driver3_cdev;
uint8_t *kernel_buffer;
/*
** Function Prototypes
*/
static int      __init driver3_init(void);
static void     __exit driver3_exit(void);
static int      driver3_open(struct inode *inode, struct file *file);
static int      driver3_release(struct inode *inode, struct file *file);
static ssize_t  driver3_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  driver3_write(struct file *filp, const char *buf, size_t len, loff_t * off);
/*
** File Operations structure
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = driver3_read,
        .write          = driver3_write,
        .open           = driver3_open,
        .release        = driver3_release,
};
 

static int driver3_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}


static int driver3_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}


static ssize_t driver3_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        }
        pr_info("Data Read : Done!\n");
        return mem_size;
}


static ssize_t driver3_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        }
        pr_info("Data Write : Done!\n");
        return len;
}



static int __init driver3_init(void)
{
        if((alloc_chrdev_region(&dev, 0, 1, "driver3")) <0)
        {
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 

        cdev_init(&driver3_cdev,&fops);
 

        if((cdev_add(&driver3_cdev,dev,1)) < 0)
        {
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
 

        if((dev_class = class_create(THIS_MODULE,"driver3_class")) == NULL)
        {
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }
 

        if((device_create(dev_class,NULL,dev,NULL,"driver3")) == NULL)
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
        unregister_chrdev_region(dev,1);
        return -1;
}


static void __exit driver3_exit(void)
{
        kfree(kernel_buffer);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&driver3_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(driver3_init);
module_exit(driver3_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");
