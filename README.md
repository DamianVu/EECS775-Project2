# EECS775-Project2
Project 2 for EECS775: Visualization

This is my Project 2 for EECS775: Visualization at KU. 
I am currently taking this course in Spring of 2018.

## Overview

The requirements for this project is to use OpenGL geometry shaders to implement the Marching Cubes
algorithm and produce visual output of two types:

1. Nested Contour Lines
2. Nested Solid Color Regions

From class discussions, the idea behind this project is to take elevation data and generate a contour map.
This technique can be applied to other data sets as well to find interesting trends.

## Coding Detail

To keep things brief, I had a rough week leading up to the project due date, and did not get to spend nearly as much time as I wanted to on this project. My line shader worked fine with a basic color ramp I chose. There was one bug in the solid shader which I am going to work on now, even though it is past the due date. You can find the code that I submitted by either reverting to the commit in which I commented that I had submitted the project in that state. Otherwise, I will use this as a learning experience and definitely plan better for project 3.

## *NOTE on OpenGL Version*

For this project, I used my laptop which could only use a OpenGL 3.3 rendering context. I changed the GLFW_CONTEXT_VERSION_MAJOR variable in line 53 in mvcutil/GLFWController.c++ from 4 to 3, so that I could target OpenGL 3.3 instead. I also changed the shaders in project2/shaders from version 410 core to version 330 core. You can change these back to use OpenGL 4.x if you would like. The professor stated that I could change the shaders to version 3.3 as you can use 3.3 shaders with a 4.1 rendering context.
