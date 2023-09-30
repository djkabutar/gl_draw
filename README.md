# GL DRAW

Making textures by the use of the OpenGL library. There are plenty 
of the use of the OpenGL to make the basic rendering framework or
to create a Game Engine. It can be also used for making any GUI based
applications.

This project aims towards the learning of the great API made by the 
GPU industry. It is the one of the oldest graphics API which is 
used in currently as well.

As of the standard, OpenGL is opensource but their drivers are made by 
the GPU company. So we can say that the headers are quite open and useful 
but the underlying process of the functions we don't really know about.

## Communication b/w Processor and GPU

In OpenGL, we make 2 kind of programs
    - Driver Program
    - Shader Program

### Driver Program

In here, we define the vertices and pass the positions of them so GPU 
takes this data from the CPU and renders it by itself.

### Shader Program

Shaders will be the program for the GPU, from which GPU defines which 
pixel would be of which colored.

We will move towards making a Game Engine by ourself.
