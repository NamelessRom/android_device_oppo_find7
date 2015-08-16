#!/sbin/static/busybox
export PATH=/sbin/static:/sbin
export TAG=`busybox basename "$0"`

source /detect_storage_layout.sh

if [ ! -z "$LVM" ] || [ ! -z "$UNIFIED" ]; then
    busybox ln -s /storage/emulated/legacy /sdcard
    busybox ln -s /storage/emulated/legacy /mnt/sdcard
    busybox ln -s /storage/emulated/legacy /storage/sdcard0
    busybox ln -s /mnt/shell/emulated/0 /storage/emulated/legacy
else
    busybox ln -s /storage/sdcard0 /sdcard
    busybox ln -s /storage/sdcard0 /mnt/sdcard
fi
