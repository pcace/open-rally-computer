# Open Rally Computer

An open source tripmaster for navigation rallies

![ORC](./img/orc.png?raw=true "Inside")

(after 2000km abuse...)


## Description

This is a Fork of the Open Rally Computer project by Matías Godoy. The original project can be found [https://github.com/mattogodoy/open-rally-computer](https://github.com/mattogodoy/open-rally-computer)

The Open Rally Computer (previously known as [Baja Pro](https://baja.matto.io/)) is a complete tripmaster for navigation rallies.

Some of its features are:

- Based on GPS technology
- Adjustable partial distance odometer
- Total distance odometer
- Speedometer
- Average speed
- Max speed
- Trip time
- Compass (CAP heading)
- Multiple languages and unit systems
- Firmware updates over-the-air
- GPX Track Logging to SD Card
- POI Saving to SD Card
- Display 12V input voltage

The main goal of this project is to achieve a widely used open-source device that can be improved over time by ourselves, the pilots behind the roadbook.

Note: contrary to the original project, this fork does not use FRAM to store the data, but saves it to the SD Card. This makes it MUCH more UNreliable! So if you are planning to use this in a real rally, I would recommend to use the original project.

## Schematics and PCB

For a more sophisticated version of the ORC with Schematics and PCB, please go to the [Open Rally Computer](https://github.com/mattogodoy/open-rally-compute) repository of Matías Godoy.

this fork here is a simplified version wireing the components directly to the used  [ESP32 DevKit V1](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/user-guide-devkitm-1.html). The mess of wires can be seen in the image below:

![Inside](./img/inside.png?raw=true "Inside")

This includes: 
- [SD Card Reader](https://de.aliexpress.com/item/1005004916894706.html?spm=a2g0o.productlist.main.5.3343518fkF5v0y&algo_pvid=5eb2e93c-0f10-4eba-950c-feb942e4dbdb&algo_exp_id=5eb2e93c-0f10-4eba-950c-feb942e4dbdb-2&pdp_npi=4%40dis%21EUR%212.62%211.84%21%21%212.80%211.96%21%40211b61b017229652803674429e49cb%2112000031008839838%21sea%21DE%212616847731%21X&curPageLogUid=NaunbedfPsp2&utparam-url=scene%3Asearch%7Cquery_from%3A) 
- [OLED Display](https://www.amazon.de/dp/B07QJ4HPV9/ref=pe_27091401_487024491_TE_item)
- [GPS Module LC86G](https://de.aliexpress.com/i/1005005605284683.html?gatewayAdapt=glo2deu)
- [3 Buttons](https://www.amazon.de/Gebildet-wasserdichte-Drucktaster-Momentanen-Hervorstehend/dp/B08L49F7DV)
- [Voltage Divider](https://www.ebay.de/itm/272641348205) 

the pins are listet in `pins.h` (when changing them, make sure to take into account that two SPI ports are used (Display and SD Card Reader) and that the GPS Module uses the Serial Port 2)

## 3D Printed Case

a very basic case can be found in the docs/stl folder. It is designed to be printed in two parts and stacked together.

## User manual

You can find the User Manual in the [manual](docs/manual) folder.

## Getting Started

These instructions will allow you to compile the project in you local computer. In order to run it you will need an actual device.

### Prerequisites

This project has been developed using VSCode with the PlatformIO plugin, which is is the recommended setup.

- Install [VSCode](https://code.visualstudio.com/)
- Install the [PlatformIO plugin](https://platformio.org/install/ide?install=vscode)
- Clone this repository

```bash
git clone https://github.com/pcace/open-rally-computer.git
```

### Building

To build this project you can open the directory of the repository you just downloaded in VSCode.

Once open, go to the sidebar at the left, PlatformIO -> esp32dev -> General -> Build

The required libraries are already bundled in the `lib` directory of this repo. PlatformIO will automatically try to use them.
If for some reason you are getting compilation errors because of missing libraries, you can always install them manually going to PlatformIO -> PIO Home -> Libraries.

You can find the list of necessary libraries in the file [platformio.ini](platformio.ini).

## Flashing into device

Once the project compiles correctly in your computer, you are ready to flash it into the rally computer.

1. Connect your ESP32 (DevKit or programmer) to your computer via USB.
2. Go to PlatformIO -> esp32dev -> General -> Upload
3. PlatformIO should automatically detect the serial port where your ESP32 is connected, buld the project and flash it for you.

## Flashing over-the-air

Once the project is flashed into the device, you can update the firmware over-the-air. After the first flash the device will open up a wifi network called "ORC" (see webserver.cpp for PW). After connecting to this network you can use the [webinterface](http://192.168.4.1/update) to upload a new firmware.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on the process for submitting pull requests.

Also read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for details on the code of conduct for this project.

## Versioning

We use [SemVer](http://semver.org/) for versioning.

For a list of available versions, see the [tags on this repository](https://github.com/pcace/open-rally-computer/tags).

## Authors

- Original Repository:  **Matías Godoy** - [matto.io](https://matto.io/)
- this Fork: **pcace** - [pcace](https://johannes-froelich.de)

See also the list of [contributors](https://github.com/pcace/open-rally-computer/graphs/contributors) who participated in this project.

## TO-DO

There are a few things I'd like to do in order to improve this project and its repository:

- [x] Build, upload and tag Version 1.0.0 binary as a release
- [x] Translate user manual to english

## License

This project is licensed under the GPL v3 License - see the [LICENSE](LICENSE) file for details.

This is what this means:

1. Anyone can copy, modify and distribute this software.
2. If you do, you have to include the license and copyright notice with each and every distribution.
3. You can use this software privately.
4. You can use this software for commercial purposes.
5. If you build your business based on this code, you have to open-source your whole code base.
6. Any modifications of this code base MUST be distributed with the same license (GPL v3).
7. Any changes made to this code MUST be indicated on distribution.
8. This software is provided without warranty.
9. The software author or license can not be held liable for any damages inflicted by the software.

## Changelog

- **1.0.0**: Initial version
  - The complete project was open sourced, including source code, bill of materials, PCB desings and user manual.

## Contributors

- [turtle3333](https://github.com/turtle3333): User manual translation
