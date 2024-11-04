#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>

#define PROC_NAME "seconds"

static unsigned long start_jiffies;

static int seconds_proc_show(struct seq_file *m, void *v) {
    unsigned long elapsed_jiffies = jiffies - start_jiffies;
    unsigned long seconds = elapsed_jiffies / HZ;
    seq_printf(m, "%lu\n", seconds);
    return 0;
}

static int seconds_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, seconds_proc_show, NULL);
}

static const struct file_operations seconds_fops = {
    .owner = THIS_MODULE,
    .open = seconds_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init seconds_proc_init(void) {
    start_jiffies = jiffies;  // Store the starting jiffies value
    proc_create(PROC_NAME, 0, NULL, &seconds_fops);
    return 0;
}

static void __exit seconds_proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
}

MODULE_LICENSE("GPL");
module_init(seconds_proc_init);
module_exit(seconds_proc_exit);
