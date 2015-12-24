#
# Regular cron jobs for the util-linux package
#
0 4	* * *	root	[ -x /usr/bin/util-linux_maintenance ] && /usr/bin/util-linux_maintenance
