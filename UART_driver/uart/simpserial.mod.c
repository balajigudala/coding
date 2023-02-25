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
	{ 0x1035c7c2, "__release_region" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x85bd1608, "__request_region" },
	{ 0xdbdf6c92, "ioport_resource" },
	{ 0xf0aa0a49, "__register_chrdev" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0xcf2a6966, "up" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x81b395b3, "down_interruptible" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x56470118, "__warn_printk" },
	{ 0x2a068163, "try_module_get" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xc5850110, "printk" },
	{ 0x985b0476, "module_put" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "28B636E0ECC9B59633CB2F2");
