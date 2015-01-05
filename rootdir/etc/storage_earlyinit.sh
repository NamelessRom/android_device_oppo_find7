#!/sbin/static/busybox
export PATH=/sbin/static:/sbin

if [ -e /dev/lvpool/userdata ]; then
    busybox cp /init.fs.rc.emu /init.fs.rc
    busybox cp /fstab.qcom.lvm /fstab.qcom
# if the userdata partition is larger then 7000 MB we probably have Coldbird's unified layout
elif [ `busybox blockdev --getsize64 /dev/block/platform/msm_sdcc.1/by-name/userdata | busybox sed -e 's/\([0-9]*\)[0-9]\{6\}/\1/'` -gt 7000 ]; then
    busybox cp /init.fs.rc.emu /init.fs.rc
    busybox cp /fstab.qcom.ufd /fstab.qcom
else
    busybox cp /init.fs.rc.std /init.fs.rc
    busybox cp /fstab.qcom.std /fstab.qcom
fi

