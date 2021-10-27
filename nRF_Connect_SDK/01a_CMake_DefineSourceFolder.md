# CMake:  defining a folder where all *.c files will automated be added to your project

## Introduction

In previous chapter we have seen how to add *.c files to your project. But instead of adding each single c file manually, we can also define a folder and all c files we copy into this folder will automatically be added to our project.
This chapter describes how to do this. 

## 

1) make a copy of one of the previous projects. 
2) Add in the copied project folder the file src/function.c. 
3) Open Visual Studio Code and add this project to your APPLICATIONS. (Reminder:  Welcome / "Add an existing application")
4) Check which C files were added to the project. (take a look into the project / Source files / Application / src folder)
5) Now replace in the CMakeLists.txt file the __target_sources(app PRIVATE src/main.c)__ instruction by the following one:

       FILE(GLOB app_sources src/*.c)
          target_sources(app PRIVATE
                         ${app_sources}
                        )
                     
3) After changing the CMakeLists.txt file do a "build". Can you see the "function.c" file in your Application folder now?
4) Click on "Pristine build" and check again the src file list. 
