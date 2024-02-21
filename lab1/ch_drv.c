#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
static dev_t first;
static struct cdev c_dev; 
static struct class *cl;

#define BUF_SIZE 16

static char in_buf[BUF_SIZE];
static char help_buf[BUF_SIZE];
int ptr = 0;

static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}

static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

void write_int(int x){
  if(x == 0){
    in_buf[ptr++] = ' '; 
    return;
  }
  write_int(x / 10);
  in_buf[ptr++] = x % 10 + 48;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver: read()\n");
  if(*off){
    return 0;
  }

  size_t real_len = min(len, sizeof(in_buf));

	if(copy_to_user(buf, in_buf, real_len)){
		return -EFAULT;
	}

  *off = real_len;
  return real_len;
}

static ssize_t my_write(struct file *f, const char __user *buf,  size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver: write()\n");

	if(copy_from_user(help_buf, buf, len)){
		return -EFAULT;
	}

  int rem = BUF_SIZE - ptr;
  if((1 << rem) < len){
    printk("Achtung: overflow, cleaning buffer...");
    memset(in_buf, 0, sizeof(in_buf));
    ptr = 0;
    rem = BUF_SIZE;
  }

  int count = snprintf(in_buf + ptr, rem, "%d ", (int) len);
  ptr += count;
  return len;
}

static struct file_operations mychdev_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .read = my_read,
  .write = my_write
};
 
static int __init ch_drv_init(void)
{
    printk(KERN_INFO "Hello!\n");
    if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0)
	  {
		return -1;
	  }
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
	  {
		unregister_chrdev_region(first, 1);
		return -1;
	  }
    if (device_create(cl, NULL, first, NULL, "mychdev") == NULL)
	  {
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	  }
    cdev_init(&c_dev, &mychdev_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
	  {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	  }
    return 0;
}
 
static void __exit ch_drv_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Bye!!!\n");
}
 
module_init(ch_drv_init);
module_exit(ch_drv_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Author");
MODULE_DESCRIPTION("The first kernel module");