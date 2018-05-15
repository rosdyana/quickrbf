# Quickrbf

QuickRBF is a simple, easy-to-use, and efficient software for Radial
Basis Function Networks (RBFN) classifier. 
This document explains the use of QuickRBF package.

QuickRBF is available at 

http://csie.org/~yien/quickrbf/

Please read the [COPYRIGHT](COPYRIGHT) file before using QuickRBF package.

## Quick Start
Please refer the QuickRBF website.

[http://csie.org/~yien/quickrbf/](http://csie.org/~yien/quickrbf/)


## Installation

On Unix systems, type ***make*** to build all the programs of package. 
Run them without arguments to show the usages of them.

On other systems, consult ***Makefile*** to build them or use the 
pre-built binaries.

The format of training and testing data file is:
```
<label> <index1>:<value1> <index2>:<value2> ...
.
.
.
```

<label> is the target value of the training data. For classification,
it should be a positive integer which identifies a class (multi-class
classification is supported). 
<index> is an integer starting from 1, and <value> is a real number. 
The labels in the testing data file are only used to calculate 
accuracy or error. 


There are some other useful programs in this package.

datatrans:

	This is a tool for convert input data file to our format.

datascale:

	This is a tool for scaling input data file.

centerselect:

	This is a tool for random selecting center data from training data.


## quickrbf Usage
```
Usage: quickrbf center_data_file training_data_file testing_data_file
options:
```
***Yu-Yen Ou***

QuickRBF : An efficient package of Radial Basis Function Networks

[http://csie.org/~yien/quickrbf/](http://csie.org/~yien/quickrbf/)

