#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define MAX_DEVICES	5

MODULE_LICENSE("GPL");
dev_t device_number;
bool dynamic = true;
struct class *my_class;
static struct cdev my_cdev[MAX_DEVICES];

static int mydevice_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

static int mydevice_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

ssize_t mydevice_read(struct file *file, char __user *user_buffer,
		      size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return 0;
}

ssize_t mydevice_write(struct file *file, const char __user *user_buffer,
		       size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return count;
}

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = mydevice_open,
	.release = mydevice_release,
	.read = mydevice_read,
	.write = mydevice_write
};


static int mynull_device_init(void)
{
    int retval;
    int i = 0;

    pr_info("%s: In init\n", __func__);
    if (dynamic) {
        retval = alloc_chrdev_region(&device_number, 0, MAX_DEVICES, "embedded");
    }
    else {
        device_number = MKDEV(180, 0);
        retval = register_chrdev_region(device_number, MAX_DEVICES, "embedded");
    }
    if (!retval) {
	    int major = MAJOR(device_number);
	    dev_t my_device;
	    my_class = class_create(THIS_MODULE, "my_driver_class");
	    for (i = 0; i < MAX_DEVICES; i++) 
	    {
		    my_device = MKDEV(major, i);
		    cdev_init(&my_cdev[i], &fops);
		    retval = cdev_add(&my_cdev[i], my_device, 1);
		    if (retval) 
		    {
			    pr_info("%s: Failed in adding cdev to subsystem "
					    "retval:%d\n", __func__, retval);
		    }
		    else 
		    {
			    device_create(my_class, NULL, my_device, NULL, "my_null%d", i);
		    }
	    }
    }
    else
	    pr_err("%s: Failed in allocating device number "
			    "Error:%d\n", __func__, retval);
    return retval;
}


static void mynull_device_exit(void)
{
	int i = 0;
	int major = MAJOR(device_number);
	dev_t my_device;
	for (i = 0; i < MAX_DEVICES; i++) 
	{
		my_device = MKDEV(major, i);
		cdev_del(&my_cdev[i]);
		device_destroy(my_class, my_device);
	}
	class_destroy(my_class);
	unregister_chrdev_region(device_number, MAX_DEVICES);
	pr_info("%s: In exit\n", __func__);
}

module_init(mynull_device_init);
module_exit(mynull_device_exit);
