echo " remove the return probes:"
echo  > /sys/kernel/debug/tracing/kprobe_events
echo " check that probes have been removed:"
cat /sys/kernel/tracing/set_event
