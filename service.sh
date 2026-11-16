#!/system/bin/sh
MODDIR=${0%/*}
KO="$MODDIR/ko/syscall_shroud.ko"

if lsmod | grep -q syscall_shroud; then
    rmmod syscall_shroud
fi

insmod "$KO"
