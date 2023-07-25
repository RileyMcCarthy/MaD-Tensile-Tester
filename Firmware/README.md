# First time setup

## Platformio

Install the flexprop platformio platform using the instructions at the link below
https://github.com/RileyMcCarthy/platform-propeller

Open the platformio.ini in the Firmware/MaD_Firmware folder and compile!

## Enable Serial

`sudo raspi-config`
Go to Interface Options and enable SPI and Serial

// This is for fast serial, not required!!!

sudo nano /boot/config.txt
add: dtoverlay = disable-bt
sudo reboot

If worked then:
ls -l /dev/serial\*
lrwxrwxrwx 1 root root 7 Nov 24 21:17 /dev/serial0 -> ttyAMA0
lrwxrwxrwx 1 root root 5 Nov 24 21:17 /dev/serial1 -> ttyS0

sudo nano /boot/cmdline.txt
Remove: console=serial0,115200
sudo reboot

Disable bluetooth:
sudo systemctl disable hciuart.service
sudo systemctl disable bluealsa.service
sudo systemctl disable bluetooth.service
sudo reboot

## Install and update packages

```
sudo apt-get update
sudo apt-get upgradesudo apt-get install -y libhdf5-dev libhdf5-serial-dev python3-pyqt5 libatlas-base-dev libjasper-dev
sudo apt-get install libmemcached-dev zlib1g-dev
```

## Install Website Dependecies

Will need to have or install NodeJS and NPM

```
curl -fsSL https://deb.nodesource.com/setup_current.x | sudo -E bash - # Enable NodeSource Repo
sudo apt install nodejs # Install nodejs
npm --version # Verify install
```

Go to Software/Website and run:

`make install'

# Run web GUI

`make run # this will run the flask-socketio webpage that we will use to interact with the RPI`

# Usage

The RPI should be connected to the MaDP2 using serial port 0 using RPI hardware interfaces.

# Running display on PI

Install:

```
sudo apt install onboard
sudo apt install at-spi2-core -y
```

Configure onboard with template and auto-open enabled

Configure chromium to run in fullscreen on startup in kiosk mode using:

```
cp /etc/xdg/lxsession/LXDE-pi/autostart ~/.config/lxsession/LXDE-pi/
nano ~/.config/lxsession/LXDE-pi/autostart
```

in nano add:
`chromium-browser --noerrdialogs --disable-infobars --incognito --kiosk http://localhost:5000/`
