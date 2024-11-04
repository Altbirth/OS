#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>

#define PROC_NAME "jiffies"

static int jiffies_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "%llu\n", (unsigned long long) get_jiffies_64());
    return 0;
}

static int jiffies_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, jiffies_proc_show, NULL);
}

static const struct file_operations jiffies_fops = {
    .owner = THIS_MODULE,
    .open = jiffies_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init jiffies_proc_init(void) {
    proc_create(PROC_NAME, 0, NULL, &jiffies_fops);
    return 0;
}

static void __exit jiffies_proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
}

MODULE_LICENSE("GPL");
module_init(jiffies_proc_init);
module_exit(jiffies_proc_exit);