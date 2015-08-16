#!/sbin/static/busybox sh
export PATH=/sbin/static:/sbin
export TAG=`busybox basename "$0"`

/sbin/lvm vgscan --mknodes --ignorelockingfailure
/sbin/lvm vgchange -aly --ignorelockingfailure

source /detect_storage_layout.sh

if [ ! -z "$LVM" ]; then
    busybox cp /init.fs.rc.emu /init.fs.rc
    busybox cp /fstab.qcom.lvm /fstab.qcom
    busybox cp /etc/twrp.fstab.lvm /etc/twrp.fstab
elif [ ! -z "$UNIFIED" ]; then
    busybox cp /init.fs.rc.emu /init.fs.rc
    busybox cp /fstab.qcom.ufd /fstab.qcom
    busybox cp /etc/twrp.fstab.ufd /etc/twrp.fstab
else
    busybox cp /init.fs.rc.std /init.fs.rc
    busybox cp /fstab.qcom.std /fstab.qcom
    busybox cp /etc/twrp.fstab.std /etc/twrp.fstab
fi
