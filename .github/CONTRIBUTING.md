# Contributing to embARC Applications

:+1::tada: First off, thanks for taking the time to contribute! :tada::+1:

For general guideline, **see the [embARC Contributing Guide](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/blob/master/.github/CONTRIBUTING.md).**

## embARC Application Directory Structure
If you want to contribute your embARC application to this repository, you need to follow the guideline.
Each embARC application exists in this repository as a folder, such as *aws_iot_smarthome*.
embARC application contains a number of folders and files. Each folder has a specified purpose, such as
sources, documentation, readme, license, etc.

Here take the *asw_iot_smarthome* application as example.

### Source Code
The source code files resides in the *src* folder. For example:
If this application just consists of one application, the structure will look like below.
```
  aws_iot_smarthome/src/
  aws_iot_smarthome/src/main.c
  aws_iot_smarthome/src/makefile
  ...
```
If this application consists of several standalone applications, such as the *aws_iot_smarthome_multinode*.
The structure will look like below, each application has a subfolder under *src* folder.
```
  aws_iot_smarthome_multinode/src/
  aws_iot_smarthome_multinode/src/frontDoor/
  aws_iot_smarthome_multinode/src/frontDoor/main.c
  aws_iot_smarthome_multinode/src/frontDoor/makefile
  aws_iot_smarthome_multinode/src/kitken/
  aws_iot_smarthome_multinode/src/kitken/main.c
  aws_iot_smarthome_multinode/src/kitken/makefile
  ...
```

### Documentation
All the documentation related to the application resides in the *doc* folder.
Screenshots of the application and application note can be put in this folder.

### Readme
Readme document usually named as *README.md* right in the application folder, such as *asw_iot_smarthome/README.md*.

Readme is used to describe the following sections:
- **Description**: Describe the application usage.
- **Hardware and Software Setup**: Describe what hardware and external peripheral are needed, and also the software needed.
- **User Manual**: Describe how to use this application, application running screenshots and description are welcomed.
- **Note**: Necessary notes of this application.

### License
License file usually named as *LICENSE* right in the application folder, such as *aws_iot_smarthome/LICENSE*.
