PERF=/mnt/500GB/linux-6.5/tools/perf/perf
echo "Delete the malloc probe"
${PERF} probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --del malloc
