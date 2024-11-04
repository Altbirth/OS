#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>

#define PROC_NAME "seconds"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jofli Delcoro");
MODULE_DESCRIPTION("A kernel module LAB Seconds Module");
MODULE_VERSION("1.0");

// Variable to store the initial jiffies value when the module is loaded
static unsigned long start_jiffies;

// Function to be called when /proc/seconds is read
static ssize_t proc_seconds_read(struct file *file, char __user *buf, size_t count, loff_t *pos) {
    char buffer[32];
    int len;
    unsigned long elapsed_jiffies = jiffies - start_jiffies;
    unsigned long elapsed_seconds = elapsed_jiffies / HZ;

    // If offset is non-zero, it means EOF is reached (for non-seekable files)
    if (*pos > 0) {
        return 0;
    }

    // Write the elapsed seconds to the buffer
    len = snprintf(buffer, sizeof(buffer), "%lu\n", elapsed_seconds);

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
    .proc_read = proc_seconds_read,
};

// Module initialization function
static int __init seconds_proc_init(void) {
    // Record the starting jiffies
    start_jiffies = jiffies;

    // Create the /proc/seconds entry
    proc_create(PROC_NAME, 0, NULL, &proc_file_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// Module cleanup function
static void __exit seconds_proc_exit(void) {
    // Remove the /proc/seconds entry
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(seconds_proc_init);
module_exit(seconds_proc_exit);
