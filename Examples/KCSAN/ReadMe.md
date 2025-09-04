# Instructions

## Start the writer and timer:

```bash
echo start | sudo tee /sys/kernel/debug/kcsan_race_demo/thread >/dev/null
echo start | sudo tee /sys/kernel/debug/kcsan_race_demo/timer  >/dev/null
```

## Stress with a one-shot write burst:

```bash
echo 50000 | sudo tee /sys/kernel/debug/kcsan_race_demo/blast >/dev/null
```

## Watch stats:

watch -n 0.2 cat /sys/kernel/debug/kcsan_race_demo/stats

Note: You should see mismatch_count rising as the timer observes broken invariants.

## Optional Tunings

```bash
echo 25   | sudo tee /sys/kernel/debug/kcsan_race_demo/write_delay_us >/dev/null
echo 0    | sudo tee /sys/kernel/debug/kcsan_race_demo/blast_delay_us >/dev/null
echo 50   | sudo tee /sys/kernel/debug/kcsan_race_demo/timer_spin_us  >/dev/null
```

## Stop or reset:

```bash
echo stop | sudo tee /sys/kernel/debug/kcsan_race_demo/timer  >/dev/null
echo stop | sudo tee /sys/kernel/debug/kcsan_race_demo/thread >/dev/null
echo 1    | sudo tee /sys/kernel/debug/kcsan_race_demo/reset  >/dev/null
```
