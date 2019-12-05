# ChocAn - Group Software Project
[![CircleCI](https://circleci.com/gh/AlexanderJDupree/ChocAn.svg?style=svg)](https://circleci.com/gh/AlexanderJDupree/ChocAn)
[![codecov](https://codecov.io/gh/AlexanderJDupree/ChocAn/branch/master/graph/badge.svg)](https://codecov.io/gh/AlexanderJDupree/ChocAn)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/af9edf02e21b4343b34b8b64ff534b10)](https://www.codacy.com/manual/AlexanderJDupree/ChocAn?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AlexanderJDupree/ChocAn&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/AlexanderJDupree/ChocAn/blob/master/LICENSE)

![chocan](docs/ChocAn_Logo.png)

## Introduction

Chocoholics Anonymous is an organization commited to treating those afflicted with addictions to chocolate in all of its glorious forms. The software contained in this repository assists in the delivery and accountability of ChocAn services to Members of the ChocAn network. 

( It's a group software project for CS300 - Elements of Software Engineering, at Portland State University )

## Project Goals

For a detailed analysis of the ChocAn project please see the following documents. 

* [Requirements](docs/ChocAn-Requirements.pdf)
* [Design](docs/ChocAn-Design.pdf)
* [Test Plan](docs/ChocAn-TestPlan.pdf)

## Prerequisites

This project uses C++17 language features and requires a compiler that supports those features. The ChocAn project is built and tested with GCC 7.0 and Clang 4.0. For a list of C++17 supported compilers see [here](https://en.cppreference.com/w/cpp/compiler_support). 

This project also utilizes the [Catch2](https://github.com/catchorg/Catch2) unit test library and the [Clara](https://github.com/catchorg/Clara) command line parser. You do not need to download these libraries, they are included in the `third_party` folder of the project repository. 

**Note**: Currently there is a bug that requires the executable to be ran from the projects root directory, since the `Terminal_State_Viewer` class uses relative paths to find the view files. This is noted in issue [#18](https://github.com/AlexanderJDupree/ChocAn/issues/18). If you attempt run the application anywhere other than the ChocAn/ folder, you will be met with a `View Not Found` message. 

## Getting Started

First clone this repository. 

```bash
git clone https://github.com/AlexanderJDupree/ChocAn.git
```

Next, build the project!

```bash
./build.sh
```

That's it!

The provided build script will build the project in debug and release configurations and runs their respective unit tests. The build script uses the provided GNU Makefiles in the `gmake/` folder and will store the compiled binaries in the `bin/` folder. If you need specific project files (Visual Studio, Xcode) or need to change the compiler, I have provided a premake5 script that can generate this files for you. If you don't have premake5 you can download it [here](https://premake.github.io/download.html).

The build script will also attempt to download the [Lib-bash](https://github.com/pioneerworks/lib-bash) utility to prettify the output. ~~Currently, if this download fails the build scrip aborts, we are working on making the process never fail in [issue #13](https://github.com/AlexanderJDupree/ChocAn/issues/13)~~. Fixed in [PR #44](https://github.com/AlexanderJDupree/ChocAn/pull/44). If the process fails you can still build the project manually like this:

To build using Make. 
```bash
cd gmake
make 
```

## Running the Application

There two executables for the application. The debug configuration is compiled with symbols on and defaults to using the `sqlite3` database in memory. The release configuration with optimization flags and defaults to using the `chocan.db` sqlite3 database and data created during runtime will persist in that database. 

Run Debug build:
```
./bin/debug/ChocAn_debug
```

Run Release build:
```
./bin/release/ChocAn_release 
```

By default, the application will read from `STDIN` and clear the screen after each input. However, you can change these options like this:

```
./bin/release/ChocAn_release -i tests/system_tests.txt --compact-output --in-memory
```

This will run the database in memory, and compact the output (No new lines). You can run the app with the `--help` flag to see a list of options. 

When you start the application you will be greeted with a login screen:

```
======================== Login Service =========================

Welcome, 

Please enter your ChocAn Provider or Manager ID. 

( Entering 'exit' will close the application )

> 
```

To login you must enter either a Provider ID or a Manager ID. To start, `123456789` is a manager ID and `987654321` is a provider ID. The rest of the ID's (and all starting data) can be found in the `chocan_schema.sql` file, or by examing the `chocan.db` with the sqlite3 command line. 

## What's in this Repo?

```
.
├── .circleci/    <-- Continuous Integration configuration
|
├── bin/          <-- Executable binaries stored here.
│   ├── debug/
│   ├── release/
│   └── tests/
|
├── docs/         <-- Project documentation goes here
|
├── gmake/        <-- Holds GNU Makefiles used to build the project
|
├── include/      <-- Public header files go here
│   └── ChocAn/
│       ├── app/  <-- Application layer files
|       |   
│       ├── core/ <-- Domain layer files
|       |
│       ├── data/ <-- Data Access Layer files
|       |
│       └── view/ <-- Presentation Layer files
|   
├── lib/          <-- Project shared libraries are stored here
│   ├── debug/
│   └── release/
|
├── src/          <-- Source files go here, structured in the same way as include/
│   └── main.cpp
|
├── tests/        <-- Test source files, structured in the same way as include/
│   └── test_config_main.cpp
|
├── third_party/  <-- Third-party libraries
│   ├── clara.hpp
│   └── catch.hpp
|
├── views/         <-- All UI text files go here
|
├── build.sh      <-- Fancy Build script
├── codecov.yml   <-- config file for code coverage reports
├── LICENSE
├── premake5.lua  <-- Build script for premake5
├── README.md     <-- This document
├── chocan.db     <-- sqlite3 database
└── chocan_schema.sql <-- Defines database schema for ChocAn


```

## Contributing

I wrote a basic contributing guide that explains how to code within the application architecture and what basic git commands you should be using. Check it out [here](https://github.com/AlexanderJDupree/ChocAn/blob/master/docs/contributing.md).

## Built With

* [Catch2](https://github.com/catchorg/Catch2) - Unit Testing framework used
* [Clara](https://github.com/catchorg/Clara) - Command Line Parser
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

