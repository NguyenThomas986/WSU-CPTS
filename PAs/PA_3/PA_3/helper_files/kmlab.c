#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
/* Declare the supplied helper without changing its provided header. */
int get_cpu_use(int pid, unsigned long *cpu_use);
#include "kmlab_given.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nguyen");
MODULE_DESCRIPTION("CPTS360 KM PA");

#define DEBUG 1

// store per process data (you may modify it as needed)
typedef struct {
    struct list_head list;
    unsigned int pid;
    unsigned long cpu_time;
} proc_list;

// Global variables as needed ...
static struct proc_dir_entry *kmlab_dir;
static struct proc_dir_entry *kmlab_status;

/* Head of the registered-process list; initially empty. */
static LIST_HEAD(process_list);
static DEFINE_SPINLOCK(process_lock);
static struct timer_list update_timer;
static struct work_struct update_work;
static bool stopping;

/* Each open reader gets a stable view, even while CPU times change. */
struct status_snapshot {
   size_t length;
   char output[PAGE_SIZE];
};

static void update_cpu_times(struct work_struct *work)
{
   proc_list *entry;
   proc_list *next;
   unsigned long cpu_time;
   unsigned long flags;

   spin_lock_irqsave(&process_lock, flags);
   list_for_each_entry_safe(entry, next, &process_list, list) {
      /* The supplied helper only performs an RCU lookup and field read. */
      if (get_cpu_use(entry->pid, &cpu_time) == 0) {
         entry->cpu_time = cpu_time;
      } else {
         list_del(&entry->list);
         kfree(entry);
      }
   }
   spin_unlock_irqrestore(&process_lock, flags);
}

static void update_timer_callback(struct timer_list *timer)
{
   unsigned long flags;

   spin_lock_irqsave(&process_lock, flags);
   if (!stopping) {
      schedule_work(&update_work);
      mod_timer(&update_timer, jiffies + 5 * HZ);
   }
   spin_unlock_irqrestore(&process_lock, flags);
}

/* Each write supplies one PID, optionally followed by a newline. */
static ssize_t status_write(struct file *file, const char __user *buffer,
                            size_t count, loff_t *position)
{
   char input[32];
   int pid;
   int result;
   proc_list *entry;
   proc_list *new_entry;
   unsigned long flags;

   if (count == 0)
      return -EINVAL;
   if (count >= sizeof(input))
      return -E2BIG;
   if (copy_from_user(input, buffer, count))
      return -EFAULT;

   /* Reject embedded terminators instead of ignoring trailing input. */
   if (memchr(input, '\0', count))
      return -EINVAL;
   input[count] = '\0';

   result = kstrtoint(input, 10, &pid);
   if (result != 0)
      return result;
   if (pid <= 0)
      return -EINVAL;

   /* GFP_KERNEL allocation may sleep, so allocate before locking. */
   new_entry = kmalloc(sizeof(*new_entry), GFP_KERNEL);
   if (new_entry == NULL)
      return -ENOMEM;
   new_entry->pid = pid;
   new_entry->cpu_time = 0;

   spin_lock_irqsave(&process_lock, flags);
   list_for_each_entry(entry, &process_list, list) {
      if (entry->pid == pid) {
         spin_unlock_irqrestore(&process_lock, flags);
         kfree(new_entry);
         return -EEXIST;
      }
   }
   list_add_tail(&new_entry->list, &process_list);
   spin_unlock_irqrestore(&process_lock, flags);

   return count;
}

/* Capture at most one page of complete PID/CPU-time lines at open. */
static int status_open(struct inode *inode, struct file *file)
{
   struct status_snapshot *snapshot;
   int line_length;
   proc_list *entry;
   unsigned long flags;

   if (!(file->f_mode & FMODE_READ))
      return 0;

   snapshot = kzalloc(sizeof(*snapshot), GFP_KERNEL);
   if (snapshot == NULL)
      return -ENOMEM;

   spin_lock_irqsave(&process_lock, flags);
   list_for_each_entry(entry, &process_list, list) {
      line_length = snprintf(snapshot->output + snapshot->length,
                             PAGE_SIZE - snapshot->length,
                             "%u: %lu\n", entry->pid, entry->cpu_time);
      if (line_length >= PAGE_SIZE - snapshot->length)
         break;
      snapshot->length += line_length;
   }
   spin_unlock_irqrestore(&process_lock, flags);

   file->private_data = snapshot;
   return 0;
}

static ssize_t status_read(struct file *file, char __user *buffer,
                           size_t count, loff_t *position)
{
   struct status_snapshot *snapshot = file->private_data;

   /* User copies can sleep; no spinlock is held here. */
   return simple_read_from_buffer(buffer, count, position,
                                  snapshot->output, snapshot->length);
}

static int status_release(struct inode *inode, struct file *file)
{
   kfree(file->private_data);
   file->private_data = NULL;
   return 0;
}

static const struct proc_ops status_ops = {
   .proc_open = status_open,
   .proc_release = status_release,
   .proc_read = status_read,
   .proc_write = status_write,
   .proc_lseek = no_llseek,
};


// kmlab_init - Called when the module is loaded
static int __init kmlab_init(void)
{
   #ifdef DEBUG
   pr_info("KMLAB MODULE LOADING\n");
   #endif
   INIT_WORK(&update_work, update_cpu_times);
   timer_setup(&update_timer, update_timer_callback, 0);
   stopping = false;

   kmlab_dir = proc_mkdir("kmlab", NULL);
   if (kmlab_dir == NULL)
      return -ENOMEM;

   kmlab_status = proc_create("status", 0666, kmlab_dir, &status_ops);
   if (kmlab_status == NULL) {
      proc_remove(kmlab_dir);
      return -ENOMEM;
   }

   mod_timer(&update_timer, jiffies + 5 * HZ);
   pr_info("KMLAB MODULE LOADED\n");
   return 0;   
}

// kmlab_exit - Called when the module is unloaded
static void __exit kmlab_exit(void)
{
   unsigned long flags;
   proc_list *entry;
   proc_list *next;

   #ifdef DEBUG
   pr_info("KMLAB MODULE UNLOADING\n");
   #endif
   /* Stop rearming before waiting, without holding the callback's lock. */
   spin_lock_irqsave(&process_lock, flags);
   stopping = true;
   spin_unlock_irqrestore(&process_lock, flags);
   del_timer_sync(&update_timer);
   cancel_work_sync(&update_work);

   proc_remove(kmlab_status);
   proc_remove(kmlab_dir);

   /* Timer, work, and proc callbacks have finished; the list is private. */
   list_for_each_entry_safe(entry, next, &process_list, list) {
      list_del(&entry->list);
      kfree(entry);
   }

   pr_info("KMLAB MODULE UNLOADED\n");
}

// Register init and exit functions
module_init(kmlab_init);
module_exit(kmlab_exit);
