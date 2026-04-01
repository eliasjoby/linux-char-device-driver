#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/version.h>

#define DEVICE_NAME "mychardev"

static int major;

/* Kernel version parameters */
static int kernel_version_0;
static int kernel_version_1;

/* Timer parameter */
static int time = 30;

/* Module parameters */
module_param(kernel_version_0, int, 0000);
module_param(kernel_version_1, int, 0000);
module_param(time, int, 0000);

/* State tracking */
static int read_done = 0;
static int write_done = 0;
static unsigned long start_time;

static char username[100] = "unknown";

static DECLARE_WAIT_QUEUE_HEAD(wq);

/* READ */
static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    printk(KERN_INFO "Device read operation performed\n");

    read_done = 1;
    wake_up_interruptible(&wq);

    return 0;
}

/* WRITE */
static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    if (!read_done) {
        printk(KERN_INFO "Write before read not allowed\n");
        return -EINVAL;
    }

    if (len >= sizeof(username))
        len = sizeof(username) - 1;

    if (copy_from_user(username, buf, len)) {
        printk(KERN_INFO "Error copying from user\n");
        return -EFAULT;
    }

    username[len] = '\0';

    printk(KERN_INFO "Username received: %s\n", username);

    write_done = 1;
    wake_up_interruptible(&wq);

    return len;
}

/* File operations */
static struct file_operations fops = {
    .read = dev_read,
    .write = dev_write,
};

/* INIT */
static int __init my_init(void)
{
    int current_major = (LINUX_VERSION_CODE >> 16) & 0xFF;
    int current_minor = (LINUX_VERSION_CODE >> 8) & 0xFF;

    printk(KERN_INFO "Given version: %d.%d\n", kernel_version_0, kernel_version_1);
    printk(KERN_INFO "Current version: %d.%d\n", current_major, current_minor);

    /* Version check */
    if (kernel_version_0 != current_major || kernel_version_1 != current_minor) {
        printk(KERN_INFO "Kernel version mismatch!\n");
        return -EINVAL;
    }

    /* Register character device */
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major;
    }

    start_time = jiffies;

    printk(KERN_INFO "Device registered successfully\n");
    printk(KERN_INFO "Major=%d Minor=0 Timer=%d sec\n", major, time);

    return 0;
}

/* EXIT */
static void __exit my_exit(void)
{
    unsigned long elapsed = (jiffies - start_time) / HZ;

    printk(KERN_INFO "Time elapsed: %lu sec\n", elapsed);

    if (read_done && write_done && elapsed <= time) {
        printk(KERN_INFO "Successfully completed the actions within time\n");
        printk(KERN_INFO "Username: %s\n", username);
    } else {
        printk(KERN_INFO "Failure: Conditions not met\n");
    }

    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");