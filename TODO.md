# TO DO LIST FOR AOC-2020

## Environment and general delaying tactics

- Add more compilers to GitHub CI (TinyCC, Intel C Compiler, GCC Windows and Mac, Clang Ubuntu)
- Re-write ci.yml to be more modular and update packages (apt install build-essential)
- Double check ci.yml structure with output from Copilot Windows app
- Get deployment working from within VS Code
- Get Actions working from within VS Code
- Update token permissions / token to allow ci.yml push
- One generic makefile or makefile content sync ?
- Check compiler flags in Makefile
- Check + add compiler flags in GitHub CI

## Refactoring

- Can we make the opening of files using access() into a generic function?
- Do something with the 2020 magic number in 01 - define somewhere or make into command line param?
- Check all instances of hard coded defines (MAX_LINE_LENGTH) and try to make dynamic
- Check all instances of functions in algorithms and files to see if we return -1 on failure and
    handle in main
- Check all instances of functions in algorithms and files and remove printf spew and debug and let
    main handle
- Check all the names in enums and functions and rename to be clearer. i.e READ this WRITE this etc
- Alphabetical ordering of functions, variable declerations
- Double check no decleration of variables in for, while loops etc
- Should we be declaring and NOT intialising for C89? Best practice

## Features

- Add Unity tests
- Decide on console UI and menus long term (ASCII?)
- Sort out the time / benchmarking approach and decide how to display multiple problems

## Basics

- Fix formatting in LICENSE.txt (see how it renders on GitHub page)
- Add SPDX-License-Identifier: MIT (see unity.c for an example)
- Handle Microsoft header files and unity files (seperate LICENSE.txt in folders?)
- Keep reviewing and updating README.md
- Investigate AI copyright statement
