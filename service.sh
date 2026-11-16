#!/system/bin/sh
MODDIR=${0%/*}
KO="$MODDIR/syscall_shroud.ko"

if lsmod | grep -q syscall_shroud; then
    rmmod syscall_shroud
fi

insmod "$KO"
