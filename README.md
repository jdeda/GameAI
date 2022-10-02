# Game AI

<img src="https://github.com/jdeda/GameAI/blob/master/assets/banner_gameAI_01.png" alt="drawing" width="800"/>


# Welcome to GameAI!
This program visualizes many fundamental algorithms of Game AI. Game AI is broken into three categories: movement, pathfinding, and decision making. This includes cannonical algorithms such as Arrive, Align, A*, and Behavior Trees. All algorithms are based upon the [AI for Games III Edition.](https://www.amazon.com/AI-Games-Third-Ian-Millington/dp/1138483974/ref=pd_lpo_1?pd_rd_i=1138483974&psc=1) Each and everyone of these algorithms have been implemented from scratch and visualized using [SFML.](https://www.sfml-dev.org/)

## 📦 Program Environment
This program was built in Linux Ubuntu OS, and uses the following dependencies:
- [C++](https://cplusplus.com)
- [SFML](https://www.sfml-dev.org/)
- [OpenGL](https://www.opengl.org)
- [X11](https://www.x.org/wiki/)
- [XQuartz](https://www.xquartz.org)

To guarantee dependencies are setup properly, it is highly advised you run the `setup.sh` script. Otherwise, you are not guaranteed such. This script will create a multi-pass VM, create a directory in that VM, clone the code from this repo, and download all the dependencies to run properly. You may need to enable permissions to run this script, and all other ones in this repo, which may be done via `chmod u+x <script name>`.

## 🔨Building the Program
Simply run `./builder.sh` to build program and run it. Alternatively you can call `make` to build it. However, if you change a header file you must call `make clean` as "make" will not pickup the dependency change!

## &#129302; Running the Program
The `builder.sh` script will build and run the program. If you chose to manually make the program, run `./main` to launch the program. The exectuable "main" is the exectuable for the entire program. The program takes user input, and even user control, so read below for details.

## 🕹 Interacting with the Program
This is the fun part. 

## 🔧 Status of Project
This project is permanently archived.
