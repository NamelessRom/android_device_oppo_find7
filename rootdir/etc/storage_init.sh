#!/sbin/static/busybox
export PATH=/sbin/static:/sbin

if [ -e /dev/lvpool/userdata ] ||
    # if the userdata partition is larger then 7000 MB we probably have Coldbird's unified layout
    [ `busybox blockdev --getsize64 /dev/block/platform/msm_sdcc.1/by-name/userdata | busybox sed -e 's/\([0-9]*\)[0-9]\{6\}/\1/'` -gt 7000 ]; then
    busybox ln -s /storage/emulated/legacy /sdcard
    busybox ln -s /storage/emulated/legacy /mnt/sdcard
    busybox ln -s /storage/emulated/legacy /storage/sdcard0
    busybox ln -s /mnt/shell/emulated/0 /storage/emulated/legacy
else
    busybox ln -s /storage/sdcard0 /sdcard
    busybox ln -s /storage/sdcard0 /mnt/sdcard
fi
