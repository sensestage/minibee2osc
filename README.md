##Dependencies

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

Arch Linux: 
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
    make
    sudo make install
```
