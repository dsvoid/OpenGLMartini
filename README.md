# OpenGL Martini
![OpenGLMartini](https://i.imgur.com/6yTqKoQ.png)
I made a little martini glass with OpenGL, GLEW, GLFW, and GLM for my graphics course! I'm just saving it here for a bit of posterity, the fragment shader is cute.

Rotate the camera around the model with your mouse, and scale it with the scroll wheel. 

Try `Q` `W` `E` `A` `S` `D` to rotate the model. 

Pressing `P` renders the model as points, `L` as polylines, and `F` as filled polygons.

## Compilation
The repo comes with a precompiled executable for Linux systems. If you want to compile yourself, make sure you have the appropriate libraries linked!

Here are my compilation steps for Linux as a reference:

```
g++ -std=c++11 -c main.cpp
g++ main.o -o main.exec -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lGLEW
```

Finally, the program can be run using by executing the `main.exec` file.


