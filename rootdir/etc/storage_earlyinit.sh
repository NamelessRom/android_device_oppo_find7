#!/sbin/static/busybox
export PATH=/sbin/static:/sbin
export TAG=`busybox basename "$0"`

source /detect_storage_layout.sh

if [ ! -z "$LVM" ]; then
    busybox cp /init.fs.rc.emu /init.fs.rc
    busybox cp /fstab.qcom.lvm /fstab.qcom
elif [ ! -z "$UNIFIED" ]; then
    busybox cp /init.fs.rc.emu /init.fs.rc
    busybox cp /fstab.qcom.ufd /fstab.qcom
else
    busybox cp /init.fs.rc.std /init.fs.rc
    busybox cp /fstab.qcom.std /fstab.qcom
fi
