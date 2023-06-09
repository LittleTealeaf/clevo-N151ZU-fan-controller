# Clevo-N151ZU automatic fan control

> Cloned from https://gitlab.com/francois.kneib/clevo-N151ZU-fan-controller

Default (bios) fan behavior for these laptops is not optimized as it triggers fan at about 51°C, making it noizy even when doing almost nothing. However, the processor i7-85**U have a max temp of 100°C.
This project brings a smarter automatic fan control, it makes it perfectly silent for less-demanding tasks like office, web, or even video-watch, but gradually power the fan when necessary. The behavior of the program is setup to:

* check in-processor temperature every 0.5s,
* shut-down fan if temp is below 70°C,
* turn on fan linearly from 50% to 100% between 70°C and 90°C,
* fan speed peaks (local maximums) will last for at least 10s to avoid bumps.

**In most computers, there is a (bios-level) temperature cut-off that will brutally cause it to power-off before any damage occurs. However, this software comes without waranty, by using it your computer may overheat and be damaged. Although it runs very well on my own device, I am not be responsible for any consequence that may occur.**

That being said, I never reached 90°C with this controller.

## Prerequisites

```
gcc build-essential
```

On debian-based distros (Ubuntu, ...):

```
sudo apt install -y git gcc build-essential
```

## Installing

Clone the repo:

```
git clone https://gitlab.com/francois.kneib/clevo-N151ZU-fan-controller.git
```

Go into the project folder:

```
cd clevo-N151ZU-fan-controller
```

Build & install:

```
sudo make all
```

This will:

1. build the c++ file into N151ZU-fan-controller and make it executable,
2. copy N151ZU-fan-controller bin file into /usr/local/bin,
3. copy the service file N151ZU-fan-controller.service into /etc/systemd/system/,
4. enable the service at startup,
5. launch the service now.

You can now check that the service is running:

```
systemctl status -n20 N151ZU-fan-controller.service
```

should output something like:

```
● N151ZU-fan-controller.service - Clevo_N151ZU-based laptop automatic fan control                                                                                                                                                           
   Loaded: loaded (/etc/systemd/system/N151ZU-fan-controller.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2019-03-18 13:39:53 CET; 24min ago
 Main PID: 31382 (N151ZU-fan-cont)
    Tasks: 1 (limit: 4915)
   CGroup: /system.slice/N151ZU-fan-controller.service
           └─31382 /usr/local/bin/N151ZU-fan-controller

mars 18 14:04:04 lafiteIII N151ZU-fan-controller[31382]: T:50°C set fan to 0%
mars 18 14:04:04 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:05 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:05 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:06 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:06 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:07 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:07 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:08 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:08 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:09 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:09 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:10 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:10 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:11 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:11 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:12 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:12 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:13 lafiteIII N151ZU-fan-controller[31382]: T:50°C
mars 18 14:04:13 lafiteIII N151ZU-fan-controller[31382]: T:50°C
```
## Note

To watch for the temperature of your cpu (for example with a desktop widget), you can use the following command (in millidegrees):

```
cat /sys/class/thermal/thermal_zone0/temp
```

If you want the daemon to write logs in syslog, use `sudo make -VERBOSE=ON all`.

## Authors

* **François Kneib** - *Initial work*

## License

This project is licensed under the GNU General Public License v3 or any later version (GPL-3.0-or-later). See [LICENSE](LICENSE).

## Acknowledgments

This project uses some code from TuxedoFanControl program: https://github.com/tuxedocomputers/tuxedo-fan-control

 
