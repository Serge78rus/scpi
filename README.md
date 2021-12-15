# scpi

Command line utility for SCPI devices

## Building and installation
```bash
make
sudo make install 
```
Last command install application binary file to /usr/local/bin. 

## Usage example

```bash
scpi -d /dev/ttyUSB0 -b 115200 -c *IDN?
```
For more examples see example directory

## Command line options

```bash
scpi --help

Usage: scpi options

options:

-h, --help                       show this help screen and exit
-V, --version                    show version information and exit
-v, --verbose          optional  show verbose information
                                 (default: false)
-d, --device=NAME      required  serial communication device
-b, --baudrate=VALUE   optional  communication baudrate
                                 (default: current setting for device)
-t, --timeout=VALUE    optional  communication timeout in milliseconds
                                 (default: 1000 ms)
-c, --command=COMMAND  required  SCPI command
                                 If COMMAND contain spaces it must be quoted
                                 May use more than one command, for examle:
                                 -c COMMAND1 -c COMMAND2 ... etc

```
