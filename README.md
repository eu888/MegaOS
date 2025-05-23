# MegaOS Overview

MegaOS is an operating system designed from scratch.

## Installation

1. Clone the repository:
```bash
git clone https://github.com/eu888/MegaOS.git
```

2. Navigate to the project directory:
```bash
cd MegaOS
```

3. Install dependencies:
```bash
docker build buildenv -t myos-buildenv
```

4. Run the following command based on your operating system:

- **Linux/Mac**:
```bash
docker run --rm -it -v "$(pwd)/root/env:/env" myos-buildenv
```

- **Windows (PowerShell)**:
```powershell
docker run --rm -it -v "${PWD}/root/env:/env" myos-buildenv
```

Note: Use `${PWD}` for PowerShell on Windows and `$(pwd)` for Linux/Mac.

After completing step 4, you can build the project with the following command:

5. Compile  
```bash
make build-x86_64
```

There is already a build with the version you downloaded.

## Usage

After compilation, you can run the `kernel.iso` found in `dist\x86_64` in VirtualBox:
1. Open VirtualBox and create a new virtual machine.
2. Select "Other" as the operating system type.
3. Attach the `kernel.iso` file as the bootable disk.
4. Start the virtual machine.

## Features

There are many features, here are arranged by the library they came from:

1. `print.h`:
    - **void print_clear();**  
        Clears the screen or terminal output.
    - **void print_char(char character);**  
        Prints a single character to the screen or terminal.
    - **void print_str(char\* str);**  
        Prints a null-terminated string to the screen or terminal.
    - **void print_set_color(uint8_t foreground, uint8_t background);**  
        Sets the foreground and background colors for subsequent text output.
        - **Parameters:**
            - `foreground`: Color code for the text.
            - `background`: Color code for the background.
    - **void print_hex(uint64_t value);**  
        Prints a 64-bit hexadecimal value.
    - **void hexdump(uint8_t\* buffer, size_t length);**  
        Prints a hex dump of the given buffer.
        - **Parameters:**
            - `buffer`: Pointer to the data buffer.
            - `length`: Length of the buffer.
    - **void wait_for_keypress();**  
        Waits for a key press from the user.
    - **void sleep(uint64_t count);**  
        Pauses execution for a specified duration.
        - **Parameter:**
            - `count`: Duration to sleep.
    
2. `memory.h`:
    - **void init_memory_manager(size_t mem_size);**  
        Initializes the memory manager with the given memory size.
        - **Parameter:**
            - `mem_size`: Total size of memory to manage.

    - **void\* alloc_page();**  
        Allocates a single memory page and returns a pointer to it.

    - **void free_page(void\* addr);**  
        Frees a previously allocated memory page.
        - **Parameter:**
            - `addr`: Pointer to the page to free.

    - **void init_heap();**  
        Initializes the kernel heap for dynamic memory allocation.

    - **void\* kmalloc(size_t size);**  
        Allocates a block of memory of the specified size from the kernel heap.
        - **Parameter:**
            - `size`: Number of bytes to allocate.
        - **Returns:** Pointer to the allocated memory.

    - **void kfree(void\* ptr);**  
        Frees a previously allocated block of memory from the kernel heap.
        - **Parameter:**
            - `ptr`: Pointer to the memory block to free.

    - **void check_heap_integrity();**  
        Checks the integrity of the kernel heap for consistency and errors.

3. `strings.h`: Includes some functions for `string.h`

4. `io.h`:
    - **void outb(uint16_t port, uint8_t val);**
        The out instruction, only one byte.
        - **Parameter:**
            - `port`: The I/O port.
            - `val`: The value to be outputed in the I/O port port.
    - **uint8_t inb(uint16_t port);**
        The in instruction, only one byte.
        - **Parameter:**
            - `port`: The I/O port wich will get the value.
    - **void outw(uint16_t port, uint8_t val);**
        The out instruction, 16 bytes.
        - **Parameter:**
            - `port`: The I/O port.
            - `val`: The value to be outputed in the I/O port port.
    - **uint8_t inb(uint16_t port);**
        The in instruction 16 bytes.
        - **Parameter:**
            - `port`: The I/O port wich will get the value.

## Contributing

Contributions are welcome! Please follow the [contribution guidelines](CONTRIBUTING.md) to get started.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
