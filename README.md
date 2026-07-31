<div align="center">

# MegaOS 

**MegaOS is an bare-metal x86 64 bit kernel.**

[![Build Docker Image and compile binaries CI](https://github.com/eu888/MegaOS/actions/workflows/docker-image.yml/badge.svg)](https://github.com/eu888/MegaOS/actions/workflows/docker-image.yml)

[![CodeQL Advanced](https://github.com/eu888/MegaOS/actions/workflows/codeql.yml/badge.svg)](https://github.com/eu888/MegaOS/actions/workflows/codeql.yml)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](License)
</div>

## Features

1. **Custom bootloader with long mode**

    * Sets up GDT and pageing.
    * GRUB multiboot support.

2. **Display**

    * VGA text mode driver with 16 colors.
    * Cursor tracking.
    * `\t`, `\n` and `\b` handling.
    * Hex output.

3. **Interrupts**

    * Full 256 entry IDT.
    * Master and slave PIC remap.
    * PIT seted up at 100Hz(for now).
    * CPU exception handlers for `0x00` (division by zero), `0x08` (double fault), `0x0D` (GPF) and `0x0E` (page fault) and a general one to prevent triple faults, upon triggering it will do a kernel panic.
    * Seted up `IRQ 0` and `IRQ 1`.

4. **Inputs**

    * PS/2 keyboard handler with scancode to ASCII converion
    * Shift and Caps lock support.

5. **Shell**

    * Command console at boot with some commands: `echo`, `color`, `clear` and `int`.
        * `echo <text>`: This command prints the text after it.
        * `color <background> <foreground>`: changes the background and foreground color from the 16 available colors:
            * BLACK = 0&emsp;&emsp;&emsp;DARK_GREY = 8
            * BLUE = 1&emsp;&emsp;&emsp;LIGHT_BLUE = 9
            * GREEN = 2&emsp;&emsp;&emsp;LIGHT_GREEN = 10
            * CYAN = 3&emsp;&emsp;&emsp;LIGHT_CYAN = 11
            * RED = 4&emsp;&emsp;&emsp;LIGHT_RED = 12
            * MAGENTA = 5&emsp;&emsp;&emsp;LIGHT_MAGENTA = 13
            * BROWN = 6&emsp;&emsp;&emsp;LIGHT_BROWN = 14
            * LIGHT_GREY = 7&emsp;&emsp;&emsp;WHITE = 15
        * `clear`: clears the console scree.
        * `int <interrupt>`: triggers the interrupt if its available.

6. **Memory management**

    * Bitmap page allocator.
    * Heap allocator `kmalloc` and `kfree` with block splitting, coalescing and double-free detection.

## Installation and build

1. Clone the repository:

    ```bash
    git clone https://github.com/eu888/MegaOS.git
    ```

2. Navigate to the project directory:

    ```bash
    cd MegaOS
    ```

3. Create docker container and install dependencies:

    ```bash
    docker build buildenv -t myos-buildenv
    ```

4. Navigate to the source code folder

    ```bash
    cd kernel
    ```

5. Run the environment build command:

    * For Linux

    ```bash
    docker run --rm -it -v "$(pwd):/env" myos-buildenv
    ```  

    This will open an interactive shell in the container with the kernel source mounted at `/env`.

    * For Windows

    ```bash
    docker run --rm -it -v "$(pwd):/MegaOS" -w /MegaOS myos-buildenv
    ```

    This will open an interactive shell in the container with the kernel source mounted at `/MegaOS`.

6. Inside the container, build the project:

    ```bash
    make build-x86_64
    ```

## Usage

After compilation, you can run the `kernel.iso` found in `dist\x86_64` in:

### VirtualBox

1. Open VirtualBox and create a new virtual machine.
2. Select "Other" as the operating system type.
    Note: that the system must be 64 bits to work
3. Attach the `kernel.iso` file as the bootable disk.
4. Start the virtual machine.

### QEMU

1. Run this command while in the environment:

    ```bash
    make run
    ```

## Contributing

Contributions are welcome! Please follow the [contribution guidelines](CONTRIBUTING.md) to get started.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
