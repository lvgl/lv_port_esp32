# LVGL under ESP-IDF demonstration project

## Overview

The **lv_esp_idf** presents a common way of building LVGL applications on top 
of the ESP-IDF structure. This project is intended to serve as a references
for users that just wanted to dive in the application code while they
abstract the project set-up complexities.

The main application will set-up the LVGL, plus the display and touch driver(if any),
then it will execute one of the selected LVGL demos such as benchmarking or music demo.

## Supported Boards

As any ESP-IDF project, this one can support a variety of ESP32 families boards, the user can
check the out-of-the-box supported boards by referring the folder [manifests](manifests/).

## Getting started

There are currently two ways of using this repository, via the [LVGL Project Creator](https://lvgl.io/tools/project-creator), 
or configuring it as regular ESP-IDF project, for the later this documentation will
cover the command line method, however the user can set this repo using the 
[Espressif IDE](https://docs.espressif.com/projects/espressif-ide/en/latest/index.html). 

### Using the LVGL Project Creator

* Obtain the [LVGL Project Creator](https://lvgl.io/tools/project-creator).
* Launch the Project Creator
* Create an ESP IDF project 
* Follow the remaining steps given by the Project Creator
    * Set up [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html).

### Using the regular ESP-IDF project method
 
* First of all you need [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html).

* Clone this repository on your preferred folder.
* Set the IDF environment: `$ . /path/to/esp-idf/export.sh`
* Go to this project root: `$ cd /path/to/this/repo`
* Before doing anything please remove the `idf_component.yml` file on the `main` folder, **this step is valid only when not using the LVGL Project creator**. 
* If user is going to build using one standard board from espressif, add its BSP package (for example for ESP32-P4 EV Function Board):
`$ idf.py add-dependency esp32_p4_function_ev_board`
* Update the components (using IDF set target is one option): `$ idf.py set-target esp32s3` 
* Wait the project to set-up.

## Building and Flashing

After setting up this project using either the LVGL Project Creator or the Espressif tools, you
should be able to build and flash the application following the steps:

* Set the IDF environment: `$ . /path/to/esp-idf/export.sh`
* Update the project for your target if it is not already: `$ idf.py set-target <esp32xxx>`
* Issue the build: `$ idf.py build`
* For flashing use the flash command: `$ idf.py flash`
* Optionally the user can select the port: `$ idf.py -p <COM or TTY port> flash`
* After flashing you can launch the console: `$ idf.py monitor`
* Optionally the user can concatenate these commands into a single one: `idf.py -p <COM or TTY port> build flash monitor`

## More information

The LVGL documentation page has a dedicated section for the Espressif chips integration. If the user wants to get more information about the integration details or tips and tricks about LVGL
under Espressif chips visit [Espressif section of LVGL documentation](https://docs.lvgl.io/master/details/integration/chip_vendors/espressif/index.html).