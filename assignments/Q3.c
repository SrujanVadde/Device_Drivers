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
static struct class *dev_class1,*dev_class2;
static struct cdev driver4_cdev1,driver4_cdev2;
uint8_t *kernel_buffer;
/*
** Function Prototypes
*/
static int      __init driver4_init(void);
static void     __exit driver4_exit(void);
static int      driver4_open(struct inode *inode, struct file *file);
static int      driver4_release(struct inode *inode, struct file *file);
static ssize_t  driver4_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  driver4_write(struct file *filp, const char *buf, size_t len, loff_t * off);
/*
** File Operations structure
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = driver4_read,
        .write          = driver4_write,
        .open           = driver4_open,
        .release        = driver4_release,
};
 

static int driver4_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}


static int driver4_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}


static ssize_t driver4_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        }
        pr_info("Data Read : Done!\n");
        return mem_size;
}


static ssize_t driver4_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        }
        pr_info("Data Write : Done!\n");
        return len;
}



static int __init driver4_init(void)
{
        if((alloc_chrdev_region(&dev, 0, 2, "mydriver1_test")) <0)
        {
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
        dev=MKDEV(major,1);
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        cdev_init(&driver4_cdev1,&fops);
     //   cdev_init(&driver4_cdev2,&fops);
 

        if((cdev_add(&driver4_cdev1,dev,1)) < 0)
        {
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }

 

        if((dev_class1 = class_create(THIS_MODULE,"driver4_class_1")) == NULL)
        {
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }
       
 

        if((device_create(dev_class1,NULL,dev,NULL,"driver123_0")) == NULL)
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
        class_destroy(dev_class1);
    //    class_destroy(dev_class2);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}


static void __exit driver4_exit(void)
{
  kfree(kernel_buffer);
        device_destroy(dev_class1,dev);
   //     device_destroy(dev_class2,dev);
        class_destroy(dev_class1);
   //     class_destroy(dev_class2);
        cdev_del(&driver4_cdev1);
   //     cdev_del(&driver4_cdev2);
        unregister_chrdev_region(dev, 2);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(driver4_init);
module_exit(driver4_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRUJAN");
