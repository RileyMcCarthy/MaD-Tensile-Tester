# First Time Setup

## Enable Serial

`sudo raspi-config`
Go to Interface Options:
- Enable Serial Port
- Enable VNC (Optional)
- Disable Serial Console (Important!)

!!! On Raspberry pi 4 disable Serial Console, this uses the serial port for logging into the device

## Install and update packages

```
sudo apt-get update
sudo apt-get upgradesudo apt-get install -y libhdf5-dev libhdf5-serial-dev python3-pyqt5 libatlas-base-dev libjasper-dev
sudo apt-get install libmemcached-dev zlib1g-dev
```

## Install Website Dependecies

Will need to have or install NodeJS and NPM

```
curl -fsSL https://deb.nodesource.com/setup_current.x | sudo -E bash -
sudo apt install nodejs
npm --version
```
If a version number is displayed NPM has been installed!

Next inside the repo folder run:

`make install'

This will install website dependancies for flask and javascript

# Run web GUI

`sudo make run 

# this will run the flask-socketio webpage that we will use to interact with the RPI`

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
