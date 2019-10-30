# ChocAn - Group Software Project
[![CircleCI](https://circleci.com/gh/AlexanderJDupree/ChocAn.svg?style=svg)](https://circleci.com/gh/AlexanderJDupree/ChocAn)
[![codecov](https://codecov.io/gh/AlexanderJDupree/ChocAn/branch/master/graph/badge.svg)](https://codecov.io/gh/AlexanderJDupree/ChocAn)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/af9edf02e21b4343b34b8b64ff534b10)](https://www.codacy.com/manual/AlexanderJDupree/ChocAn?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AlexanderJDupree/ChocAn&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/AlexanderJDupree/ChocAn/blob/master/LICENSE)

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
Now you can run the applciaiton with:

```bash
./bin/release/ChocAn_release
```

Also, the test binary is located in `bin/tests/` if you run `./bin/tests/debug_tests --help` you will see a list of testing options that can be performed.

## What's in this Repo?

```bash
.
├── bin         <-- Executable binaries stored here.
│   ├── debug
│   ├── release
│   └── tests
|
├── build.sh    <-- Fancy Build script
|
├── codecov.yml <-- config file for code coverage reports
|
├── docs        <-- Project documentation goes here
|
├── gmake       <-- Holds GNU Makefiles used to build the project
|
├── include     <-- Public header files go here
│   └── ChocAn
│       ├── app <-- Application layer files
|       |   |
│       │   ├── states  <-- Specific Application states go here
|       |   
│       ├── core <-- Domain layer files
|       |
│       ├── data <-- Data Access Layer files
|       |
│       └── view <-- Presentation Layer files
|   
├── lib          <-- Project static libraries are stored here
│   ├── debug
│   └── release
|
├── LICENSE
├── premake5.lua <-- Build script for premake5
├── README.md    <-- This document\
|
├── src          <-- Source files go here, structured in the same way as include/
│   ├── app
│   ├── core
│   ├── data
│   ├── view
│   └── main.cpp
|
├── tests       <-- Test source files, structured in the same way as include/
│   ├── app
│   ├── core
│   ├── data
│   └── test_config_main.cpp
|
├── third_party <-- Third-party libraries
│   └── catch.hpp
|
└── views       <-- All UI text files go here

```

## Contributing

I wrote a basic contributing guide that explains the application architecture and how to code within it. Check it out [here](https://github.com/AlexanderJDupree/ChocAn/blob/master/docs/contributing.md).

## Built With

* [Catch2](https://github.com/catchorg/Catch2) - Unit Testing framework used
* [Premake5](https://premake.github.io/download.html) - Build system

## Authors

* **Alex Salazar**  - [AlexSalazarJ](https://github.com/AlexSalazarJ)
* **Daniel Mendez** - [dannymendez90](https://github.com/dannymendez90)
* **Kyle Zalewski** - [kzalewski11](https://github.com/kzalewski11)
* **Dominique Moore** - [Dom26](https://github.com/Dom26)
* **Arman Alauizadeh** - [keyumar](https://github.com/Keyumar)
* **Alexander DuPree** - [AlexanderJDupree](https://github.com/AlexanderJDupree)

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://raw.githubusercontent.com/AlexanderJDupree/BigInt/master/LICENSE) file for details

