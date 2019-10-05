# ChocAn - Group Software Project
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/52406353f564468e9e301645f02127a3)](https://www.codacy.com/app/AlexanderJDupree/File_Reader?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AlexanderJDupree/File_Reader&amp;utm_campaign=Badge_Grade)\
[![CircleCI](https://circleci.com/gh/AlexanderJDupree/ChocAn.svg?style=svg)](https://circleci.com/gh/AlexanderJDupree/ChocAn)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/AlexanderJDupree/ChocAn/blob/master/LICENSE)

TODO Update and connect badges

## Introduction

ChocAn is a group software project for CS300 at Portland State University

## Getting Started

First clone this repository. 

```bash
git clone https://github.com/AlexanderJDupree/ChocAn.git
```

Next we have to build the project. If you're on linux, GNU makefiles are provided in the gmake folder. If you need specific project files (VScode, Xcode, etc.) there is a provided premake5 script that can generate the files for you. If you don't have premake5 you can download it [here](https://premake.github.io/download.html). 

To build using Make. 
```bash
cd gmake
make
```

Alternatively, I also provided a build script that will build and execute the unit tests all at once. You can run the build script with:

```bash
./build.sh
```


### Usage 

## What's in this Repo?

```bash
├── bin <-- executables will be found in here
|
├── build.sh <-- Fancy build script
|
├── docs <-- finalized and in-progress documentation will be found here
|
├── gmake <-- directory holds all the GNU Makefiles and object binaries. 
│   ├── ChocAn-App.make
│   ├── ChocAn.make
│   ├── Makefile
│   └── Tests.make
|
├── include <-- Stores all public header files
│   └── ChocAn
│       └── datetime.hpp
|
├── LICENSE
|
├── premake5.lua <-- build script for premake5, generates the Make files
|
├── README.md <- this document
|
├── src <-- implementation files here. 
│   ├── datetime.cpp
│   └── main.cpp
|
├── tests <-- Ensure each module contains its own test file
│   ├── datetime_tests.cpp
│   └── test_config_main.cpp
|
└── third_party <-- open source libraries will be stored here, with their licences if needed. 
    └── catch.hpp
```

## Contributing

If you're contributing to this project ensure you communicate which feature exactly you will be developing, and try to develop only that feature. It is also vital that all dvelopment is done on a feature branch. For example, if you are working on the DateTime library, then you should first run:

```bash
git checkout -b DateTime
```

Then within that branch create more branches for each module created within that feature. Also, be sure to pull from the master branch regularly so your branch is continually updated. When your feature is completed and a suite of unit tests has been added create a pull request into the master branch then we can review the changes, run the tests, and merge the feature branch. This will keep our git history clean, and prevent merge difficulties. 

## Built With

* [Catch2](https://github.com/catchorg/Catch2) - Unit Testing framework used
* [Premake5](https://premake.github.io/download.html) - Build system

## Authors

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://raw.githubusercontent.com/AlexanderJDupree/BigInt/master/LICENSE) file for details

