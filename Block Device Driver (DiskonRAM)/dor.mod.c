#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xcd71858e, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x12bb7a08, __VMLINUX_SYMBOL_STR(blk_init_queue) },
	{ 0x6d860b81, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0x43d98f4f, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x26da9b93, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0x4aa0d2de, __VMLINUX_SYMBOL_STR(put_disk) },
	{ 0x9a2301dd, __VMLINUX_SYMBOL_STR(__blk_end_request_cur) },
	{ 0x689389dc, __VMLINUX_SYMBOL_STR(blk_fetch_request) },
	{ 0xea7fe6b1, __VMLINUX_SYMBOL_STR(__blk_end_request_all) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xcac11e40, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0x62d73cc, __VMLINUX_SYMBOL_STR(blk_queue_logical_block_size) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "53634E879B38B55D6886E42");
