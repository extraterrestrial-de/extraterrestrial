# CoSpaceChallenge - Extraterrestrial

## This repository includes all important source files.

## A quick overview about the folders

>code
>>
>>  ╠ CoSpaceFunctions.cpp & CoSpaceFunctions.hpp
>>
>>  ╠ ColorRecognition.cpp & ColorRecognition.hpp
>>
>>  ╠ CommonFunctions.cpp & CommonFunctions.hpp
>>
>>  ╠ MapData.cpp & MapData.hpp
>>
>>  ╠ Navigation.cpp & Navigation.hpp
>>
>>  ╠ Pathfinding.cpp & Pathfinding.hpp
>>
>>  ╚ main.cpp
>--------------
> map_interpreter
>>
>>  ╠ mapDataCPP
>>
>>  ╚ map_interpreter.py
>--------------
>  extraterrestrial.dll
>  
>  watch_for_update.py

## code

As you can see all important program files are included in this directory.
These are the files that will be compiled into a .dll file and control the robot. 
(I won't go into further detail on how the program exactly works.)

## map_interpreter

This directory includes a python-script which will create the MapData.cpp file. 
This file will then contain a varity of infos about the first and second world such as "traps, walls, deposit-areas" and "points".
The mapDataCPP is a normal textfile with some of the text that will later go into the MapData.cpp file.

## extraterrestrial.dll

This is simply a compiled version of the c++ code from above.


## watch_for_update.py

This python-script automattically compiles the code in the "code" folder. 
It will do so after it detected any changes in that directory.


#### DISCLAIMER: To run the python-scripts you need python and the libaries [opencv](https://pypi.org/project/opencv-python/) and [watchdog](https://pypi.org/project/watchdog/).
