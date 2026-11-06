# PA3 VM verification

Run these in your Linux VM after taking a snapshot. Commands assume a terminal
in the repository root. Run each section separately and stop on unexpected errors.
These are proposed tests, not a record of successful final-module execution.

## 1. Build and run integration tests

```bash
cd helper_files
make clean
make -j2 W=1
lsmod | grep '^kmlab'
```

If the last command shows an old loaded module, run `sudo rmmod kmlab` before
continuing. No match is normal when it is already unloaded.

```bash
sudo insmod kmlab.ko
python3 verify_pa3.py
sudo rmmod kmlab
test ! -e /proc/kmlab && echo 'PASS: proc entries removed'
sudo dmesg | tail -n 60
```

The Python test takes roughly 65–90 seconds. It checks permissions, empty reads,
malformed writes, concurrent duplicate registration, two running applications,
increasing CPU times across timer intervals, stable short-read snapshots, EOF,
userspace output, and removal after process termination and reaping. It uses
60-second applications to allow slow VMs time to schedule work. Failures raise
an exception; preserve that output. Kernel warnings, faults, or lock reports
must be investigated, even if Python prints PASS.

The test deliberately leaves its own registered PID for the worker to remove
after Python exits, or for module cleanup to free. A zombie task may remain
until its parent reaps it; this follows the supplied helper's lookup contract.
CPU values are raw helper values, not elapsed seconds. Do not expect exact
values or perfectly timed updates on a busy VM.

## 2. One-process behavior

From `helper_files/`, with the module unloaded:

```bash
sudo insmod kmlab.ko
./userapp 15 &
pa3_one=$!
sleep 6
cat /proc/kmlab/status
sleep 5
cat /proc/kmlab/status
wait "$pa3_one"
sleep 6
cat /proc/kmlab/status
sudo rmmod kmlab
```

The running samples should show one PID and its CPU time. Compare the two samples;
the value should grow. The final read should be empty after the worker removes
the reaped application. Allow another interval if scheduling is delayed.

## 3. Two-process behavior

```bash
sudo insmod kmlab.ko
./userapp 18 &
pa3_first=$!
./userapp 18 &
pa3_second=$!
sleep 6
cat /proc/kmlab/status
sleep 5
cat /proc/kmlab/status
wait "$pa3_first"
wait "$pa3_second"
sleep 6
cat /proc/kmlab/status
sudo rmmod kmlab
```

While both applications are running, the output should show two PIDs and their
CPU times. Both records should disappear after termination,
reaping, and a worker update.

## 4. Supplied test script

The unchanged supplied script begins with `rmmod`, rebuilds, loads the module,
starts applications for 5 and 35 seconds, and reads at approximately 5, 10, and
25 seconds. It does not wait for the longer application or unload afterward.
It also does not stop after failed commands; inspect every error.
With a module loaded before starting it:

```bash
sudo insmod kmlab.ko
sh kmlab_test.sh
sleep 16
cat /proc/kmlab/status
sudo rmmod kmlab
sudo dmesg | tail -n 60
```

The short-lived PID should eventually disappear; the longer-lived PID should
remain while it runs. Exact numbers and the first-read timing can differ from
the sample output.

## 5. Repeated unload, including pending work and open descriptors

Run with no module currently loaded. Each cycle registers the shell PID and
keeps a read descriptor open while unloading. A delay straddles timer expiry.

```bash
for pa3_delay in 1 5 6 5 1; do
    sudo insmod kmlab.ko || break
    printf '%s\n' "$$" > /proc/kmlab/status
    exec 3</proc/kmlab/status
    sleep "$pa3_delay"
    sudo rmmod kmlab || { exec 3<&-; break; }
    exec 3<&-
    test ! -e /proc/kmlab || break
    echo "PASS: unload cycle after $pa3_delay seconds"
done
sudo dmesg | tail -n 100
```

All five cycles should unload without faults or hanging. This exercises cleanup
but is not proof of absence of all leaks or races. If the VM kernel already has
kmemleak/lock debugging enabled, inspect those reports as an additional check;
ordinary successful runs do not substitute for such instrumentation.
