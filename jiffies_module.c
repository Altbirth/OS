#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define PROC_NAME "jiffies"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jofli Delcoro");
MODULE_DESCRIPTION("A kernel module LAB Jiffies Module");
MODULE_VERSION("1.0");

// Function to be called when /proc/jiffies is read
static ssize_t proc_jiffies_read(struct file *file, char __user *buf, size_t count, loff_t *pos) {
    char buffer[32];
    int len;
    unsigned long jiffies_value = jiffies;

    // If offset is non-zero, it means EOF is reached (for non-seekable files)
    if (*pos > 0) {
        return 0;
    }

    // Write the jiffies value to the buffer
    len = snprintf(buffer, sizeof(buffer), "%lu\n", jiffies_value);

    // Copy buffer to userspace
    if (copy_to_user(buf, buffer, len)) {
        return -EFAULT;
    }

    // Update offset to prevent multiple reads of the same value
    *pos = len;

    return len;
}

// Define file operations structure
static const struct proc_ops proc_file_ops = {
    .proc_read = proc_jiffies_read,
};

// Module initialization function
static int __init jiffies_proc_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_file_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// Module cleanup function
static void __exit jiffies_proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(jiffies_proc_init);
module_exit(jiffies_proc_exit);
