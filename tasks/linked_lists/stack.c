#include "stack.h"
#include "assert.h"
#include <linux/slab.h>
#include <linux/gfp.h>

stack_entry_t* create_stack_entry(void *data)
{
	stack_entry_t* entry = NULL;
	assert(data);

	entry = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
	if (entry) {
		entry->data = data;
		return entry;
	}

	return NULL;
}

void delete_stack_entry(stack_entry_t *entry)
{
	assert(entry);
	kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
	assert(stack);
	assert(entry);
	list_add_tail(&entry->lh, stack);
}

stack_entry_t* stack_pop(struct list_head *stack)
{
	assert(stack);
	if (!list_empty(stack)) {
		stack_entry_t* res = list_entry(stack->prev, stack_entry_t, lh);
		list_del(stack->prev);
		return res;
	}
	return NULL;
}
