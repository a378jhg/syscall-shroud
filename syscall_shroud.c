#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <asm/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("a378jhg");
MODULE_DESCRIPTION("Syscall Shroud - Safe kretprobe implementation");

// Only hook getpriority to prevent any kernel deadlocks
#define SYSCALL_GETPRIORITY "__arm64_sys_getpriority"

static int ret_handler_getpriority(struct kretprobe_instance *ri, struct pt_regs *regs) {
    // Override return value to 0 (normal priority)
    regs->regs[0] = 0;
    return 0;
}

static struct kretprobe krp_getpriority = {
    .kp.symbol_name = SYSCALL_GETPRIORITY,
    .handler = ret_handler_getpriority,
    .maxactive = 20, // Limit concurrency to prevent memory overflow
};

static int __init syscall_shroud_init(void) {
    int ret = register_kretprobe(&krp_getpriority);
    if (ret < 0) {
        pr_err("syscall_shroud: failed to register kretprobe (%d)\n", ret);
        return ret;
    }
    pr_info("syscall_shroud: loaded safely\n");
    return 0;
}

static void __exit syscall_shroud_exit(void) {
    unregister_kretprobe(&krp_getpriority);
    pr_info("syscall_shroud: unloaded\n");
}

module_init(syscall_shroud_init);
module_exit(syscall_shroud_exit);
