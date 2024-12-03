echo "Add the probe point"
perf probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --add=malloc
echo "Enable the newly added probe"
echo 1 > /sys/kernel/tracing/events/probe_libc/malloc/enable
echo "Enable tracing"
echo 1 > /sys/kernel/tracing/tracing_on
echo "Record the events using record and the configured probe"
perf record -e 'probe_libc:malloc' -a

