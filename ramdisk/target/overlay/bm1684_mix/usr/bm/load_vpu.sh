#!/bin/sh
module="vpu"
device="vpu"
mode="664"

# Group: since distributions do	it differently,	look for wheel or use staff
#if grep	'^staff:' /etc/group > /dev/null; then
#    group="staff"
#else
#    group="wheel"
#fi

# invoke insmod	with all arguments we got
# and use a pathname, as newer modutils	don't look in .	by default
vpu=`/sbin/lsmod | awk "\\$1==\"$module\" {print \\$1}"`
if [ "$vpu" != "$module" ]; then
  echo "insmod $module.ko"

  # invoke insmod with all arguments we got
  # and use a pathname, as newer modutils don't look in . by default
  /sbin/insmod $module.ko $* || exit -1

  major=`cat /proc/devices | awk "\\$2==\"$module\" {print \\$1}"`

  # Remove stale nodes and replace them, then give gid and perms
  # Usually the script is	shorter, it's simple that has several devices in it.

  rm -f /dev/${device}
  mknod /dev/${device} c $major 0
  chmod $mode  /dev/${device}
  chown root /dev/${device}

else
  echo "$module.ko already installed"
fi

