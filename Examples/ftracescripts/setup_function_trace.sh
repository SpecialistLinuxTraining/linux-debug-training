cd /sys/kernel/debug/tracing
# Set the current tracer to function
echo function > current_tracer

# Enable tracing for TCP functions
echo 'tcp*' > set_ftrace_filter

# Start tracing
echo 1 > tracing_on

# Generate some network activity
wget www.google.com

# Stop tracing
echo 0 > tracing_on

# View the trace
cat trace

