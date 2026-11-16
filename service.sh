#!/system/bin/sh
MODDIR=${0%/*}
KO="$MODIR/ko/syscall_shroud.ko"

# Wait for the system to fully boot before loading the module
sleep 60

if lsmod | grep -q syscall_shroud; then
    rmmod syscall_shroud
fi

insmod "$KO"
