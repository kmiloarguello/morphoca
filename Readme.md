# Mathematical morphology applied on graphs

This repository contains the implementation for the project "Exploration in techniaques of mathematical morphology applied on graphs". 

## Contents

- Technologies
- How to run the code
- Binary filters 
- Grayscale filters

# Technologies

The project was mainly implemented on `C`, however the implementation for irregular graphs and the test with common libraries was made in `Python`. It is important to have already installed the basic libraries of `Python` In order to execute the program in `C` it is better to do that in Linux.

# How to run the code

Go to the terminal and type:

```
make
```
This command will compile all the operators such as Dilation, Erosion, etc. Then if you want to execute each program you have to type:

```
./Dilation Images/cell.pgm ESs/Cross3x3.pgm output.pgm
```
That command will take an image as input (First parameter), also an structuring element (Second parameter) and Ouput image (Third parameter). If you wish you can plot you image by using the command `display output.pgm`