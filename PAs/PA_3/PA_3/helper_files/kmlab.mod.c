#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2c635209, "module_layout" },
	{ 0xfee2724b, "no_llseek" },
	{ 0x3c12dfe, "cancel_work_sync" },
	{ 0x82ee90dc, "timer_delete_sync" },
	{ 0x82d822d9, "proc_remove" },
	{ 0x367b5b62, "proc_create" },
	{ 0x202508d9, "proc_mkdir" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x92997ed8, "_printk" },
	{ 0x2d5f69b3, "rcu_read_unlock_strict" },
	{ 0xa031e726, "pid_task" },
	{ 0x3279207c, "find_vpid" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xaf88e69b, "kmem_cache_alloc_trace" },
	{ 0x30a93ed, "kmalloc_caches" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0xa07a37f0, "memchr" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x37a0cba, "kfree" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9D312909101174CC6015C7B");
