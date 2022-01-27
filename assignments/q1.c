#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/ioctl.h>
#include "ioctl.h"


static struct cdev my_cdev;
#define RSA question_1
//protocol
int RSA_open(struct inode *inode,struct file *filp);
ssize_t RSA_read(struct file *filp,char *ubuff,size_t count,loff_t *off);
ssize_t RSA_write(struct file *filp,const char *ubuff,size_t count,loff_t *off);
long RSA_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);
int RSA_release(struct inode *inode,struct file *filp);


struct file_operations fops=
{
    .owner      =        THIS_MODULE,
    .open       =        RSA_open,
    .read       =        RSA_read,
    .write      =        RSA_write,
    .unlocked_ioctl=     RSA_ioctl,
    .release    =        RSA_release,
};


struct status
{
    int size;
    char kbuffer[50];
    int r_w;
};
int status =0;

dev_t dev;

static int __init prog_init(void)
{
    int result;
    dev=MKDEV(255,10);
    if((result=register_chrdev_region(dev,1,"question_1"))<0)
    {
        printk("cannot allocated device number..!\n");
        return -1;
    }
    cdev_init(&my_cdev,&fops);
    if((result=cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add major number and cdev structure..!\n");
        return -1;
    }
    printk("DRIVER LOADED");
    return 0;
}
static void __exit prog_exit(void)
{
    unregister_chrdev_region(dev,1);
    cdev_del(&my_cdev);
    printk("\n DRIVER UNLOADED..!\n");
}

int RSA_open(struct inode *inode,struct file *filp)
{
    printk("OPEN CALL..!!\n");
    return 0;
}
char kbuff[50];
ssize_t RSA_write(struct file *filp,const char *ubuff,size_t count,loff_t *off)
{
    ssize_t retval;
    int result;
    result=copy_from_user((char )kbuff,(char)ubuff,count);
    if(result==0)
    {
        printk("\n write successfully..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        retval=(count-result);
        return retval;
    }
    else
    {
        printk("\n error in writing..\n");
        return -EFAULT;
    }

}
ssize_t RSA_read(struct file *filp,char *ubuff,size_t count,loff_t *off)
{
    ssize_t retval;
    int result;
    result=copy_to_user((char )ubuff,(char)kbuff,count);
    if(result==0)
    {
        printk("\n read successfully..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        retval=(count-result);
        return retval;
    }
    else
    {
        printk("\n error in reading..\n");
        return -EFAULT;
    }

}
long RSA_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
    char sbuff[50];
    int result;
    switch(cmd)
    {
        case WRITE_I:
            result=copy_from_user((char*)sbuff,(char*)arg,sizeof(sbuff));
            status=1;
            copy_to_user(&status,arg,sizeof(arg));
            break;
        case READ_I:
            result=copy_to_user((char*)arg,(char*)sbuff,sizeof(sbuff));
            status=0;
            break;
        case GET_STATUS:
            printk("\n status\n",status);
            break;
    }
    return 0;
}
int RSA_release(struct inode *inode,struct file *filp)
{
    printk("CLOSE CALL..!!\n");
    return 0;
}
module_init(prog_init);
module_exit(prog_exit);
