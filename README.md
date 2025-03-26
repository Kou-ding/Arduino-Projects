# Arduino-Projects
A library of several well documented arduino projects for educational purposes. 

### Arch setup
Install:
```bash
sudo pacman -S arduino-ide
```
If:
```
Failed uploading: uploading error: exit status 1
```
For the boards that expose a UART over USB, it is necessary to allow read/write access to the serial port to users. Edit the following file:
```bash
sudo nano /etc/udev/rules.d/01-ttyusb.rules
```
By adding the following:
```txt
SUBSYSTEMS=="usb-serial", TAG+="uaccess"
```
Reload the rules:
```bash
sudo udevadm trigger
```