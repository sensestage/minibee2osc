# Installation

## Dependencies

* libxbee: communication with the xbee
  
Website: https://github.com/attie/libxbee3

To install on your system:
```
    git clone https://github.com/attie/libxbee3.git
    cd libxbee3
    make configure
    sudo make install
```

* liblo: communication via osc. Probably available through your
  package manager.

* tinyxml2: for reading the XML configuration files. Probably available through your
  package manager.

* CMake: for building

## Arch Linux: 
```bash
sudo pacman -S liblo
```

* tinyxml2: xml parsing of the configuration files

```bash
sudo pacman -S tinyxml2
```

* qt5: gui wrapper for the commandline program

```bash 
sudo pacman -S qt5-base
```

## Build Instructions

```
    git clone https://github.com/sensestage/minibee2osc.git
    cd minibee2osc
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
```

# Usage

There are various options that can be passed onto minibee2osc:

- configfile (-c): the XML configuration file to use (default `example_hiveconfig.xml`)
- serialport (-s): the serial port to use (default `/dev/ttysUSB1`)
- listenport (-l): the UDP port to listen on for OSC messages (default `57600`)
- targetport (-p): the UDP port to send OSC messages to (default `57120`)
- targetip (-i): the IP address to send OSC message to (default `127.0.0.1` (localhost))
- loglevel (-v): how much output the program should generate for debugging (default `0`)
- help (-h): get help info

## Documentation

General documentation on the Sense/Stage MiniBee can be found here: https://docs.sensestage.eu/sensestage-v1/

This software package is a replacement for [pydon](https://github.com/sensestage/ssdn_python). To use it you need to reconfigure your coordinator node to use API mode 1. You can use [X-CTU for this](https://docs.sensestage.eu/sensestage-v1/adding-new-minibees-to-a-network-with-xctu/).
