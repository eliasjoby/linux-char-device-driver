# Operating Systems - Assignment 1
## Character Device Driver Implementation

### Author
Name: Elias Joby
Roll No: B240175CS

## Description

This project implements a Linux character device driver that:

- Accepts `kernel_version_0`, `kernel_version_1`, and `time` as module parameters
- Loads only if the provided kernel major/minor version matches the running kernel version
- Registers a character device dynamically
- Requires operations in this order:
  1. Read from the device
  2. Write username to the device
- Validates on module removal whether:
  - Both operations were completed in order
  - Execution finished within the provided time limit

## Requirements

- Linux system (Debian/Ubuntu recommended)
- Kernel headers installed
- `gcc` and `make`

## Build

```bash
make
```

This generates `mymodule.ko`.

## Execution Steps

### 1. Check running kernel version

```bash
uname -r
```

Use the major and minor values from this output for `kernel_version_0` and `kernel_version_1`.

### 2. Insert module

Example:

```bash
sudo insmod mymodule.ko kernel_version_0=6 kernel_version_1=12 time=30
```

Check logs:

```bash
sudo dmesg | tail -n 20
```

### 3. Create device node

Get the major number from `dmesg` (`Major=<value> Minor=0 ...`), then run:

```bash
sudo mknod /dev/mychardev c <major> 0
sudo chmod 666 /dev/mychardev
```

### 4. Perform required operations

Read first:

```bash
cat /dev/mychardev
```

Write username next:

```bash
echo -n "elias" > /dev/mychardev
```

### 5. Remove module and verify result

```bash
sudo rmmod mymodule
sudo dmesg | tail -n 20
```

## Expected Success Output

If all conditions are met:

```text
Successfully completed the actions within time
Username: elias
```

## Failure Cases

### 1. Timeout

```bash
sudo insmod mymodule.ko kernel_version_0=6 kernel_version_1=12 time=5
sleep 10
cat /dev/mychardev
echo -n "elias" > /dev/mychardev
sudo rmmod mymodule
```

### 2. Wrong order (write before read)

```bash
sudo insmod mymodule.ko kernel_version_0=6 kernel_version_1=12 time=30
echo -n "elias" > /dev/mychardev
sudo rmmod mymodule
```

### 3. Kernel version mismatch

```bash
sudo insmod mymodule.ko kernel_version_0=5 kernel_version_1=2 time=30
```

## Files Included

- `mymodule.c`: kernel module source code
- `Makefile`: build instructions
- `README.md`: execution guide

## Notes

- Kernel major/minor version must match the running kernel.
- Always check logs using `dmesg`.
- Device must be read before write.
- Complete operations within the provided `time` limit.

## Conclusion

This assignment demonstrates:

- Linux kernel module programming
- Character device driver implementation
- User-kernel interaction through file operations
- Time/order-based validation logic in kernel space


