#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>

#include "stack.h"
#include "assert.h"

static int __init test_stack(void)
{
	int ret = 0;
	LIST_HEAD(data_stack);
	stack_entry_t *tos = NULL;
	const char *tos_data = NULL;
	const char* test_data[] = { "1", "2", "3", "4" };
	long i = 0;

	pr_alert("Testing basic stack");

	for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
		tos = create_stack_entry((void*)test_data[i]);
		if (!tos) {
			ret = -ENOMEM;
			break;
		}
		stack_push(&data_stack, tos);
	}

	for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
		tos = stack_pop(&data_stack);
		tos_data = STACK_ENTRY_DATA(tos, const char*);
		delete_stack_entry(tos);
		printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
		assert(!strcmp(tos_data, test_data[i]));
	}

	assert(stack_empty(&data_stack));
	if (ret == 0)
		pr_alert("Test success!\n");

	return ret;
}

static int __init print_processes_backwards(void)
{
	int ret = 0;
	LIST_HEAD(fname_stack); // stack of copied names
	stack_entry_t *tos = NULL;
	const char* comm_buf = NULL;
	struct task_struct *p = NULL;

	for_each_process(p) {
		comm_buf = kmalloc(sizeof(p->comm), GFP_KERNEL);
		if (!comm_buf) {
			ret = -ENOMEM;
			break;
		}
		get_task_comm((char*)comm_buf, p);
		tos = create_stack_entry((void*)comm_buf);
		if (!tos) {
			ret = -ENOMEM;
			kfree(comm_buf);
			break;
		}
		stack_push(&fname_stack, tos);
	}

	while (!stack_empty(&fname_stack)) {
		tos = stack_pop(&fname_stack);
		comm_buf = STACK_ENTRY_DATA(tos, const char*);
		printk(KERN_DEFAULT "%s\n", comm_buf);
		kfree(comm_buf);
		delete_stack_entry(tos);
	}

	assert(stack_empty(&fname_stack));

	if (ret == 0)
		pr_alert("Processes printed!\n");

	return ret;
}

static int __init ll_init(void)
{
	int ret = 0;
	printk(KERN_ALERT "Hello, linked_lists\n");

	ret = test_stack();
	if (ret)
		goto error;

	ret = print_processes_backwards();

error:
	return ret;
}

static void __exit ll_exit(void)
{
	printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
