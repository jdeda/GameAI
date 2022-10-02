# Game AI

<img src="https://github.com/jdeda/GameAI/blob/master/assets/banner_gameAI_01.png" alt="drawing" width="800"/>


# Welcome to GameAI!
This program visualizes many fundamental algorithms of Game AI. Game AI is broken into three categories: movement, pathfinding, and decision making. This includes cannonical algorithms such as Arrive, Align, A*, and Behavior Trees. All algorithms are based upon the [AI for Games III Edition.](https://www.amazon.com/AI-Games-Third-Ian-Millington/dp/1138483974/ref=pd_lpo_1?pd_rd_i=1138483974&psc=1) Each and everyone of these algorithms have been implemented from scratch and visualized using [SFML.](https://www.sfml-dev.org/)

Table
- [Game AI](#game-ai)
- [Welcome to GameAI!](#welcome-to-gameai-)
  * [📦 Environment and Dependencies](#---environment-and-dependencies)
  * [🛠 Setting up Environment](#---setting-up-environment)
    + [1. Creating the VM](#1-creating-the-vm)
    + [2. Connecting to the VM](#2-connecting-to-the-vm)
    + [3. Setting up XQuartz](#3-setting-up-xquartz)
    + [4. Setting up OpenGL](#4-setting-up-opengl)
    + [5. Setting up SFML](#5-setting-up-sfml)
  * [🔨Building the Programs](#--building-the-programs)
  * [&#129302; Running the Programs](#--129302--running-the-programs)
  * [🕹 Interacting with the Program](#---interacting-with-the-program)
  * [🔧 Status of Project](#---status-of-project)


## 📦 Environment and Dependencies
This program was built in Linux Ubuntu OS, and uses the following dependencies:
- [Ubuntu](https://ubuntu.com)
- [C++](https://cplusplus.com)
- [SFML](https://www.sfml-dev.org)
- [OpenGL](https://www.opengl.org)
- [X11](https://www.x.org/wiki)
- [XQuartz](https://www.xquartz.org)

<hr>

## 🛠 Setting up Environment
Unfournantely, this one is a bit lengthy, so I'll try to keep it short. I could have created a script, but one may run into issues so I decided to write a guide instead.

### 1. Creating the VM
Install multipass, init and launch a VM:
1. `brew install multipass`
2. `multipass launch -n gameai -c 4 -m 8G -d 20G`
3. `multipass list`

You may also alter the parameters of the second command, which creates a VM
with usage of 4 CPU cores, 8G memory, 20G disk.

After running the third commmand you should see your VM running. We'll use that IP address to connect to the VM later.

### 2. Connecting to the VM
We first must create an SSH key pair. You need to copy your ssh public key into the VM and add it to the authorized_keys file.
```
multipass transfer ~/.ssh/id_rsa.pub gameai:.ssh/id_rsa.pub
multipass shell gameai
cd .ssh/
cat id_rsa.pub >> authorized_keys
rm id_rsa.pub
exit
```
Now you should be able to connect to your VM via something like `ssh -Y ubuntu@192.168.64.2`, with that those numbers being the IP of your VM. 
### 3. Setting up XQuartz
Make sure you download XQuartz and launch it. This will allow X11 forwarding to your VM. Run the following commands in your VM:
```
sudo apt update && sudo apt -y upgrade;
sudo apt install x11-apps;
xeyes;
```

### 4. Setting up OpenGL
SFML, the graphics library used for this prorgam uses OpenGL. Out the box there can be some issues getting it to work. We can fix those easily.
1. OpenGL Profile Error
    1. add `export LIBGL_ALWAYS_INDIRECT=1` on your host machine to your `~/bashrc` file
    2. reload `~/bashrc` with `source ~/bashrc`

2. XQuartz GLX Forwarding
    1. must use versons 2.8.1 or newer
    2. run `defaults write org.xquartz.X11 enable_iglx -bool true` to fix this

3. Restart XQuartz and things should be working
4. Go into VM and run `glxgears` and you should get 3 colored gears rotating

### 5. Setting up SFML
Simply run `sudo apt -y install build-essential libsfml-dev` in your VM. By this point, everything should be running.

<hr>

## 🔨Building the Programs
As previously explained, there are three directories, each which visualize different categories of AI algorithms. Each directory however follows the same build process. Simply run `./build.sh` to build the program and run it. Alternatively you can call `make` to build it. Note, if you change a header file you must call `make clean` as "make" will not pickup the dependency change!

## &#129302; Running the Programs
Once any of the programs are  built, run `./main` to run the program. `main` is the exectuable for each program. The program takes user input, and even user control, so read below for details.

## 🕹 Interacting with the Program
This is the fun part. Everything is pretty intuitive.

## 🔧 Status of Project
This project is permanently archived.
