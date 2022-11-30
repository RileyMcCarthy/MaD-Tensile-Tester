# First time setup

## Install flexprop

use these instructions or see build instructions at https://github.com/totalspectrum/flexprop

```
cd $HOME
mkdir -p src
cd src
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install bison git tk8.6-dev
sudo apt-get install texlive-latex-recommended pandoc
git clone --recursive https://github.com/totalspectrum/flexprop --branch v5.9.19
cd flexprop
make install INSTALL=~/flexprop
```

## Enable Serial

`sudo raspi-config`
Go to Interface Options and enable SPI and Serial

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

## Create pyhon venv

`make guideps'

# Run web GUI

`make gui # this will run the flask-socketio webpage that we will use to interact with the RPI`

# Build and upload to the RPI

`make flexpropc # builds a clean image that will be uploaded to a connected propeller 2, will also run a debug terminal`

# Usage

The RPI must be connected to the MaDP2 using serial port 0 and SPI port 0 using RPI hardware interfaces.

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
`chromium-browser --noerrdialogs --disable-infobars --incognito --kiosk http://localhost:8080/`
