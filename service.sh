#!/system/bin/sh
MODDIR=${0%/*}
KO="$MODDIR/ko/syscall_shroud.ko"

# Wait for 30 seconds to ensure the system is fully booted
sleep 30

if lsmod | grep -q syscall_shroud; then
    rmmod syscall_shroud
fi

insmod "$KO"
