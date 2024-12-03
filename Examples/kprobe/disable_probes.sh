echo "Disable probes"
echo 0 > /sys/kernel/debug/tracing/events/kprobes/kernelclone/enable
echo 0 > /sys/kernel/debug/tracing/events/kprobes/cloneretprobe/enable
echo "Checking that probes have been disabled"
cat /sys/kernel/tracing/set_event
