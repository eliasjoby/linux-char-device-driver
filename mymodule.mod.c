#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xa5cca762, "__register_chrdev" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x361be725, "param_ops_int" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0xe2964344, "__wake_up" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");

