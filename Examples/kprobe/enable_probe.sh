echo "Enable probes"
echo 1 > /sys/kernel/debug/tracing/events/kprobes/enable
echo 1 > /sys/kernel/debug/tracing/events/kprobes/kernelclone/enable
echo 1 > /sys/kernel/debug/tracing/events/kprobes/cloneretprobe/enable
echo "Checking that probes have been enabled"
cat /sys/kernel/tracing/set_event
