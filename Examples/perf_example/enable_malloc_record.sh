PERF=/mnt/500GB/linux-6.5/tools/perf/perf
echo "Add the probe point"
${PERF} probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --add=malloc
${PERF} record -e probe_libc:malloc -a
sleep 100
${PERF} report
echo "Delete the malloc probe"
${PERF} probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --del malloc
