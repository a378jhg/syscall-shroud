#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/syscalls.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
MODULE_DESCRIPTION("Syscall Shroud - Spoof priority/ioprio syscalls");

#if defined(CONFIG_ARM64)
#define SYSCALL_GETPRIORITY "__arm64_sys_getpriority"
#define SYSCALL_SETPRIORITY "__arm64_sys_setpriority"
#define SYSCALL_IOPRIO_GET  "__arm64_sys_ioprio_get"
#define SYSCALL_IOPRIO_SET  "__arm64_sys_ioprio_set"
#else
#define SYSCALL_GETPRIORITY "__x64_sys_getpriority"
#define SYSCALL_SETPRIORITY "__x64_sys_setpriority"
#define SYSCALL_IOPRIO_GET  "__x64_sys_ioprio_get"
#define SYSCALL_IOPRIO_SET  "__x64_sys_ioprio_set"
#endif

static int pre_handler_getpriority(struct kprobe *p, struct pt_regs *regs) {
    regs->regs[0] = 0; // Spoofed return value (normal priority)
    return 1;          // Skip the original syscall
}

static int pre_handler_setpriority(struct kprobe *p, struct pt_regs *regs) {
    regs->regs[0] = 0; // Pretend success
    return 1;
}

static int pre_handler_ioprio_get(struct kprobe *p, struct pt_regs *regs) {
    regs->regs[0] = 0;
    return 1;
}

static int pre_handler_ioprio_set(struct kprobe *p, struct pt_regs *regs) {
    regs->regs[0] = 0;
    return 1;
}

static struct kprobe kp_getpriority = {
    .symbol_name = SYSCALL_GETPRIORITY,
    .pre_handler = pre_handler_getpriority,
};

static struct kprobe kp_setpriority = {
    .symbol_name = SYSCALL_SETPRIORITY,
    .pre_handler = pre_handler_setpriority,
};

static struct kprobe kp_ioprio_get = {
    .symbol_name = SYSCALL_IOPRIO_GET,
    .pre_handler = pre_handler_ioprio_get,
};

static struct kprobe kp_ioprio_set = {
    .symbol_name = SYSCALL_IOPRIO_SET,
    .pre_handler = pre_handler_ioprio_set,
};

static int __init syscall_shroud_init(void) {
    int ret;

    ret = register_kprobe(&kp_getpriority);
    if (ret < 0) {
        pr_err("syscall_shroud: failed to register getpriority kprobe (%d)\n", ret);
        return ret;
    }

    ret = register_kprobe(&kp_setpriority);
    if (ret < 0) {
        pr_err("syscall_shroud: failed to register setpriority kprobe (%d)\n", ret);
        unregister_kprobe(&kp_getpriority);
        return ret;
    }

    ret = register_kprobe(&kp_ioprio_get);
    if (ret < 0) {
        pr_err("syscall_shroud: failed to register ioprio_get kprobe (%d)\n", ret);
        unregister_kprobe(&kp_setpriority);
        unregister_kprobe(&kp_getpriority);
        return ret;
    }

    ret = register_kprobe(&kp_ioprio_set);
    if (ret < 0) {
        pr_err("syscall_shroud: failed to register ioprio_set kprobe (%d)\n", ret);
        unregister_kprobe(&kp_ioprio_get);
        unregister_kprobe(&kp_setpriority);
        unregister_kprobe(&kp_getpriority);
        return ret;
    }

    pr_info("syscall_shroud: loaded successfully\n");
    return 0;
}

static void __exit syscall_shroud_exit(void) {
    unregister_kprobe(&kp_ioprio_set);
    unregister_kprobe(&kp_ioprio_get);
    unregister_kprobe(&kp_setpriority);
    unregister_kprobe(&kp_getpriority);
    pr_info("syscall_shroud: unloaded\n");
}

module_init(syscall_shroud_init);
module_exit(syscall_shroud_exit);
