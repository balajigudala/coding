#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0x3e6bcf2, "module_layout" },
	{ 0x5a9b429, "class_destroy" },
	{ 0x9b1b5569, "device_destroy" },
	{ 0x56ad8d8, "kthread_stop" },
	{ 0xb6d9badf, "wake_up_process" },
	{ 0x4c625029, "kthread_create_on_node" },
	{ 0x8d682e50, "device_create" },
	{ 0x6871c306, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x34e1fe8f, "cdev_add" },
	{ 0x6009ea21, "cdev_init" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x1000e51, "schedule" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x985b0476, "module_put" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B7B43D72380770FDC0F319A");
