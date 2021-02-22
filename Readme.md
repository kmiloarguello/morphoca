# Mathematical morphology applied on graphs

This repository contains the implementation for the project "Exploration in techniaques of mathematical morphology applied on graphs". 

## Contents

- Technologies
- How to run the code
- Binary filters 
- Grayscale filters
- Adjacency Regions

## Technologies

The project was mainly implemented on `C`, however the implementation for irregular graphs and the test with common libraries was made in `Python`. It is important to have already installed the basic libraries of `Python` In order to execute the program in `C` it is better to do that in Linux.

## How to run the code

Go to the terminal and type:

```
make
```
This command will compile all the operators such as Dilation, Erosion, etc. Then if you want to execute each program you have to type:

```
./Dilation Images/cell.pgm ESs/Cross3x3.pgm output.pgm
```
That command will take an image as input (First parameter), also an structuring element (Second parameter) and Ouput image (Third parameter). If you wish you can plot you image by using the command `display output.pgm`

## Binary filters 

This project contains different files for each operator. For example for the operator Dilation in a binary level you should run the file `Dilatation.c`, the next list show the whole list of files and its purpose.

- `Dilatation.c` It performs a dilation in binary level. Inside of this file there is a funcion called **dilation** which is in charge to apply the filter.
- `Erosion.c` It performs an erosion in binary level. Inside of this file there is a funcion called **erosion** which is in charge to apply the filter.
- `Opening.c` It performs an opening in binary level. Inside of this file there are two funcions called **dilation** and **erosion** which is in charge to apply the filter.
- `Closing.c` It performs a closing in binary level. Inside of this file there are two funcions called **dilation** and **erosion** which is in charge to apply the filter.

## Grayscale filters

The following files are related to the implementation of morphological filters but on grayscale.

- `GrayDilation.c` It performs a dilation in a gray level. Inside of this file there is a funcion called **dilation** which is in charge to apply the filter.
- `GrayErosion.c` It performs an erosion in a gray level. Inside of this file there is a funcion called **erosion** which is in charge to apply the filter.
- `GrayOpening.c` It performs an opening in a gray level. Inside of this file there are two funcions called **dilation** and **erosion** which is in charge to apply the filter.
- `GrayClosing.c` It performs a closing in a gray level. Inside of this file there are two funcions called **dilation** and **erosion** which is in charge to apply the filter.

## Adjacency Regions

This implementation was made in Python. It can be found in the file `scripts.ipynb`. The file also contain the results of the set of tests made comparing the implemented code with the results by using a common `Python` library.

To execute this kind of file you have two options

1. Upload it into a Google Colab notebook
2. Install Anaconda and try to run it in you computer

## Contributions

This project can be updated by anyone who wants to improve it. There is not a general rule for the contribution. Just clone the project, try by yourself and try to do a Pull-Request.

## Acknowledgment

To the professor Isabelle, who help me to understand and keep going with the project.