# Exos II GoTo Telescope Mount Driver for libindi

---

## Disclaimer
You get the driver free of charge and, also may modify it to your needs.

However the software is distributed AS IS.

**I'M NOT RESPONSIBLE FOR ANY DAMAGES OR INJURIES, CAUSED BY THIS SOFTWARE!**

---

## Introduction
This is a basic driver for the Bresser Exos II GoTo telescope mount controller, allowing the connection to Indi clients/software.
The driver is intended for remote control on a Raspberry Pi, running Astroberry with libindi, but may run on any Indi running platform.
Its current state is experimental, but hopefully gradually improves.
Since its the initial release, feedback for improvement is appreciated.

If your have an improvements, features to add or a bug to report, please fell free to write a mail, a ticket in the issues section or a pull request.

### About the Mount
The Bresser Exos II GoTo Mount has a relabled JOC SkyViewer Handbox (PCB Rev. 1.09 2012_08).
It runs the Firmware Version 2.3 distributed by Bresser.
The mount is highly autonomous, in terms motion controls, when initialized properly I did not notice any jams or crashes.
On the serial protocol side however, this device is quite primitive. 
The data exchange is established using a 13 Byte message frame, with a 4 Byte preamble, leaving 1 byte for a command and 8 bytes for command parameter data.
The protocol only accepts, a few commands for goto tracking, parking, motion stop and Location/Time/Date setting.

I reverse engineered the the most useful parts of the serial protocol using serial port sniffing tools, developping this driver as a result. 

---

## Requirements
***This driver is intended for Bresser Exos II GoTo System not the Explore Scientific Exos II which may share some similiarities. The drivers for these system and its derivitives are already included in the INDI Environment.***

- Raspberry Pi with Astroberry (AB, Version 2.0.3 or higher) with Libindi 1.8.7 (https://www.astroberry.io/, https://www.indilib.org/), In fact: any platform running indi will do, but I developped this with AB in mind,
- latest Version of cmake installed (at least Version 3.13)
- Astronomy Software (KStars, for Windows download see: https://edu.kde.org/kstars/#download, or use the package manager of your linux distribution.)
- A COM Port or working USB to Serial Adapter (any device supporting the change of Baud Rates will do!)
- The Bresser Serial Adapter for the Handbox (https://www.bresser.de/Astronomie/Zubehoer/Motoren-Steuerungen/BRESSER-Computer-Kabel-zur-Fernsteuerung-von-MCX-Goto-Teleskopen-und-EXOS-II-EQ-Goto-Montierungen.html)
- The Bresser Exos II GoTo Mount (or the Upgrade Kit) (https://www.bresser.de/Astronomie/BRESSER-Messier-EXOS-2-EQ-GoTo-Montierung.html, https://www.bresser.de/Astronomie/Zubehoer/Motoren-Steuerungen/BRESSER-StarTracker-GoTo-Kit.html)
- Firmware Version 2.3 installed on the Handbox (https://www.bresser.de/Astronomie/Zubehoer/Motoren-Steuerungen/BRESSER-Computer-Kabel-zur-Fernsteuerung-von-MCX-Goto-Teleskopen-und-EXOS-II-EQ-Goto-Montierungen.html, under Manual)

## Features of the Driver
- Works with KStars/Stellarium using Indi Connection
- GoTo Coordinates and Track commands (Sidereal Tracking)
- Park and Abort commands
- Sync commands for alignment
- Get/Set Site Location
- Set Date/Time
- Adjust Pointing while Tracking
- Software Autoguiding (highly experimental)

---
## Install / Update / Removal

### Prerequisites
Make sure you have software in this section installed before the build attempt.

#### Libindi
Check if you have the libindi software suite installed:

>``sudo apt-get install libindi1 libindidriver1 libindi-data libindi-dev libindi-plugins libindialignmentdriver1``

This should be done on Astroberry, but may be necessary on a different distribution.

#### Development Tools and Build System
Please check if cmake is installed, since its the primary build system and throw in a build-essential for good measure.

>``sudo apt-get install cmake build-essential``

Apart from cmake, this should already be installed on Astroberry, but may be necessary on a different distribution.

Wait until everything is installed, and continue with building the driver.

### Building and Installing the Driver
1. Clone the Repository into a directory of the Pi (eg. your Home directory):
> ``git clone https://github.com/kneo/BresserExosIIDriverForIndi.git``

2. Change Directory to BresserExosIIDriverForIndi:
> ``cd BresserExosIIDriverForIndi``

3. Create a directory "build" in the current directory:
> ``mkdir build``

4. Change to the build directory:
> ``cd build``

5. Run ``cmake ..`` (and wait for completion):
	- provide``CMAKE_INSTALL_PREFIX`` to adjust the install location of the driver binary if necessary.
	- provide ``XML_INSTALL_DIR`` to adjust the location of the xml file for indi if necessary.

6. Run build process (and wait for the conclusion):
> ``cmake --build .``

**Optional Test before Installing:** before installing, you may make sure everything works by commanding ``indiserver -v ./BresserExosIIGoToDriver`` in the build directory.
This way you don't "taint" your system.
Simply create an new Profile "Testing" in your KStars EKOS dialog, and set server settings to your pi.
Make sure the indi server does not run, on astroberry this should be default!
This mode also gives valuable insight on the software workings via some text outputs, this may be helpful when reporing a bug.

7. Install the driver (if everything seems working):
> ``sudo make install``

8. Restart the indi Service or simply restart the Pi
	- This is necessary for the driver to be visible in the Web Manager

9. Go To the Astroberry Manager Web Page:
	- http://ip-of-your-pi/desktop

10. Remain here do not "Connect" to the desktop, rather Go To INDI Webmanager:
	- Click the little scope on the side bar

11. Look throught the Driver list for "Exos II GoTo"
	- If it appears, the installation is completed.
	
### Update The Driver
Its recommended to keep the repository stored on your device for easy updating. 

1. Make sure the indi software does not run, it should not on a freshly rebooted system. Use:
``fuser /bin/BresserExosIIGoToDriver``
if it returns a number a process is still "using" the file, 
kill this process using: ``kill -9 PID``, where **PID** is the number the ``fuser`` command returned.

2. open the root directory of the driver source code repository in a terminal.
	- The context menu of your prefered file manager may have an appropriate context menu entry.
3. unless you changed any of the contents of the source directory the command should download all remote changes/updates. To get the changes use:
``git pull``

4. go into the ``build`` directory you created in step 3. of the installation guide using:
``cd build``

5. again run the ``cmake ..`` command.

6. when concluded run the ``cmake --build .`` again to start the build process.

7. once successfully finished, run:
``sudo make install`` to replace your existing install files.

8. the update is done, continue using the driver. Although is test is recommended.
The ``/usr/share/indi/BresserExosIIGoToDriver.xml`` file should contain the version number of the latest version. Also the Driver should show the correct version number in the kstars EKOS panel of the driver.

### Remove the Driver
If you want to remove your driver, you can do so by just deleting the files installed by ``sudo make install``. The default installation paths are:
- ``/bin/BresserExosIIGoToDriver``
- ``/usr/share/indi/BresserExosIIGoToDriver.xml``
If you changed the paths, by eg. providing a ``CMAKE_INSTALL_PREFIX`` you have to adjust you path accordingly.

Use a ``rm /bin/BresserExosIIGoToDriver`` and ``rm /usr/share/indi/BresserExosIIGoToDriver.xml`` to delete the files.

If you want to find out if you removed everything or don't know what to delete, use the ``find / -name "Bresser*" 2>/dev/null`` command to find any "Bresser*" related file on your file system. Check through the output for any remaining binary file installed on your system This command may also find them in your home directory, but these can be considered inactive.

---

## Important Note before Further Setup or Observation
It is **important** that you put the scope in the Home position, Polar and Star Align in accordance to the Bresser manual provided with the telescope.

**Its vital in order to avoid damage to your Equipment. This Driver can not handle this for your!**

---

## Client Software Configuration
Hock up your Serial Cable to the Handbox and the USB-To-Serial Adapter and connect the USB-To-Serial Adapter to the Pi/Computer.

Enter the Command:
> ``ls -l /dev/ttyUSB*``

To see if a Serial device has appeared.
This device name is required to configure the driver.
If not, use:

> ``dmesg``

to see if your adapter has a different name.

### KStars
I happend to use kstars for configuration, it provides a wizard to set up the connection.
In this Wizzard enter the Address of your Astroberry Pi, and KStars will setup a profile for you.
You have to enter "Exos II GoTo" manually in the Mount Driver Combobox, since kstars does not list this device (at least not in my case).
Usually everything else falls into places here. When connecting to the Indiservice in kstars, it should already have selected your serial port.
But depending on how much equipment you have hocked up, you may have to choose the right port for your mount profile.

If you want to use an Autoguider, you can set up the autoguiding options, set the "Via" to "Bresser Exos II GoTo Driver (for Firmware V2.3)" set "Guider" to your autoguider.
The Mount does not support setting the autoguiding rate via serial protocol, you have to change these manually in the settings menu of the Handbox (Center Key -> Setup -> Tracking Rate -> Guiding Speed).

### Stellarium
You can also use Stellarium, but you need to set up an Indi instead of the Ascom device (assuming you previously used it).
The Telescope Control plug in is quite simple. You just need to select the Indi connector, and enter the IP-Address of your Rasperry PI in the "Indi Settings" Groupbox.
Click "Refresh Devices" to get a list with available devices in the Combo-Box below. Select the "Bresser Exos II GoTo Driver (for...2.3)".

In Stellarium commands to for GoTo are supported, no parking or stopping, but it updates the telescope pointing coordinates in the Sky View.

### Test Utility
The repository also contains a little test utility accessing the serial device directly.
It can be used to try out the driver functionallty without a lot of configuration, or without messing up existing configurations.
Building the driver should also build the tool, to use it just call:

> ``./BresserExosIIGoToDriverForIndiTest /dev/ttyUSB0``

*(your serial device may be different)*

It requires exclusive access to the interface, so make sure nothing else is using it.
You will be introduced to a simple menu system allowing to send the commands implemented in the serial protocol.

This tool is kind of fragil, it may crash on empty inputs, in such a case just restart it.

I Intend to replace this with a proper testing method, since its not that appropriate for this purpose.

### Testing your Connection
Once you have a set up profile in e.g. KStars issue a "park" command to see if everything, is working. 
The hand controller should sound a single beep from the handbox.
With this you can start doing your observation, the go to mount should now work with the astro software of your choice.

## Known Issues
- Tracking modes can not be set, only Sidereal Tracking is working right now.
- KStars only updates location but not the time, everything is implemented, but the function is not called by the EKOS Software.
- More a Hint than an issue: Sync only works when tracking an object. This behaviour is implemented on the handbox and can not be changed.
- Newer versions of indi (Version 1.8.8) may break the build, since the driver interface has changed.
- you can not perform the meridian flip from afar, since the handbox does not allow it.
- While testing Autoguiding with the simulator it appeared, that the guide star drifts away. Since I lack an autoguider I can not test real world circumstances.
## Thanks
- Thanks to spitzbube for his effort in reverse engineering the handbox (https://github.com/Spitzbube/EXOS-2_GoTo_HandController) for revealing valuable insights!
- Thanks to SimonLilie from https://forum.astronomie.de for feedback and testing!