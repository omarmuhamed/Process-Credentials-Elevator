![](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
) ![](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
# Process Credentials Elevator
This is kernel module used to give root credentials to any running process using PID

## Build
### cmake
Run this command in the source folder
```
cmake . && make
```
### make
Create Makefile and run `make`
```
obj-m+=main.o
all:
  make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
  make: -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
```
## Installation
After successfully building, `main.ko` file will be generated. To install it you have to run this command.
```
sudo insmod main.ko
```
## Uninstallation
```
sudo rmmod main.ko
```
