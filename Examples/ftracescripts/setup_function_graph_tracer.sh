# Navigate to the ftrace directory
cd /sys/kernel/debug/tracing

# Set the current tracer to function_graph
echo function_graph > current_tracer

# Enable tracing for TCP functions
echo 'tcp*' > set_ftrace_filter

# Start tracing
echo 1 > tracing_on


# Perform some TCP network activity (e.g., use curl to fetch a webpage)
curl http://example.com

# Stop tracing
echo 0 > tracing_on

# View the trace
cat trace
