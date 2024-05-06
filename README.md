# Snake-game
The original snake game. To run this project you only need a recent g++ version, make installed in your system, plus a modern browser like Firefox or Chrome and the version 5 of QT.

## How to build
To build the project you need to do the following steps:
```
qmake
make
./Exercici-2
```

## Implementation
A program implemented with OpenGL. The main code is in the MyGLWidget.cpp and LL2GLWidget.cpp where you can find the interactions with the models that exists. In addition of the differents funcionalities that have the keys:
```
'key_left': move left.
'key_right': move right.
'key_up': move up.
'key_down': move down.
'r': reset the game.
't': active a timer that moves the snake to the direction it's watching.
'c': alterate between a 3D and 2D view.
```
