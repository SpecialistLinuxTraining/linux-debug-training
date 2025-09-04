PERF=/mnt/500GB/linux-6.5/tools/perf/perf
echo "Disabling tracing"
echo 0 > /sys/kernel/tracing/tracing_on
echo "Disabling the malloc probe"
echo 0 > /sys/kernel/tracing/events/probe_libc/malloc/enable
echo "Delete the malloc probe"
${PERF} probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --del malloc
