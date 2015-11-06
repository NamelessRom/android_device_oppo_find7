#!/sbin/static/busybox
export PATH=/sbin/static:/sbin
export TAG=`busybox basename "$0"`

source /detect_storage_layout.sh

if [ ! -z "$LVM" ]; then
    busybox cp /fstab.qcom.lvm /fstab.qcom
    /system/bin/setprop ro.crypto.fuse_sdcard true
elif [ ! -z "$UNIFIED" ]; then
    busybox cp /fstab.qcom.ufd /fstab.qcom
    /system/bin/setprop ro.crypto.fuse_sdcard true
else
    busybox cp /fstab.qcom.std /fstab.qcom
    /system/bin/setprop ro.vold.primary_physical 1
fi
