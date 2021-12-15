# scpi

Command line utility for SCPI devices

## building and installation
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
