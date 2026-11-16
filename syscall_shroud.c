#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/syscalls.h>
#include <asm/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
MODULE_DESCRIPTION("Syscall Shroud - Safer spoofing via post_handler");

#define SYSCALL_GETPRIORITY "__arm64_sys_getpriority"
#define SYSCALL_SETPRIORITY "__arm64_sys_setpriority"

// Post-handler executes AFTER the original syscall has finished.
static void post_handler_getpriority(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
    // Override the return value (x0 register) to 0 (normal priority)
    regs->regs[0] = 0;
}

static void post_handler_setpriority(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
    // Pretend it succeeded (return 0)
    regs->regs[0] = 0;
}

static struct kprobe kp_getpriority = {
    .symbol_name = SYSCALL_GETPRIORITY,
    .post_handler = post_handler_getpriority,
};

static struct kprobe kp_setpriority = {
    .symbol_name = SYSCALL_SETPRIORITY,
    .post_handler = post_handler_setpriority,
};

static int __init syscall_shroud_init(void) {
    int ret = 0;
    ret += register_kprobe(&kp_getpriority);
    ret += register_kprobe(&kp_setpriority);

    if (ret < 0) {
        unregister_kprobe(&kp_setpriority);
        unregister_kprobe(&kp_getpriority);
        pr_err("syscall_shroud: failed to register kprobes\n");
        return ret;
    }

    pr_info("syscall_shroud: loaded safely\n");
    return 0;
}

static void __exit syscall_shroud_exit(void) {
    unregister_kprobe(&kp_setpriority);
    unregister_kprobe(&kp_getpriority);
    pr_info("syscall_shroud: unloaded\n");
}

module_init(syscall_shroud_init);
module_exit(syscall_shroud_exit);
