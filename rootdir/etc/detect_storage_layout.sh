#!/sbin/static/busybox
export PATH=/sbin/static:/sbin

# detect storage layout for find7

# try to get the size of the userdata partition with multiple retries
userdata_size=""
retries=10
while [ -z "$userdata_size" ] && [ $retries -gt 0 ]; do
  userdata_size=`busybox blockdev --getsize64 /dev/block/platform/msm_sdcc.1/by-name/userdata | busybox sed -e 's/\([0-9]*\)[0-9]\{6\}/\1/'`
  if [ -z "$userdata_size" ]  && [ $retries -gt 1 ]; then
    busybox echo "$TAG: retrying to detect size of userdata partition in 2s" > /dev/kmsg
    busybox sleep 2
  fi
  retries=$(($retries-1))
done

if [ -z "$userdata_size" ]; then
  busybox echo "$TAG: error: could not detected size of userdata partition" > /dev/kmsg
else
  busybox echo "$TAG: detected size of userdata partition: $userdata_size MB" > /dev/kmsg
  # if the userdata partition is larger then 7000 MB we probably have Coldbird's unified layout
  if [ $userdata_size -gt 7000 ]; then
    UNIFIED=1
    busybox echo "$TAG: Coldbird's unified storage layout detected" > /dev/kmsg
  else
    busybox echo "$TAG: did not detect Coldbird unification" > /dev/kmsg
  fi
fi

if [ -e /dev/lvpool/userdata ]; then
   LVM=1
   busybox echo "$TAG: LVM storage layout detected" > /dev/kmsg
fi
