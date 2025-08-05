# Advent of Code: 2020 in C

We are attempting to solve all of the 2020 Advent of Code problems in C.

What is Advent of Code?

https://adventofcode.com/

Advent of Code is an annual programming event that runs from December 1st to 25th. It features a series of daily coding puzzles, each with two parts, designed to be solved using any programming language. 

Each of the problems/solutions is organised into folders i.e 01/ corresponds to the Day 01 problem. Generally there are two sets of input data, which are uniquely generated for each AOC user. sample.txt is a short set of data to help test implementation and finaldata.txt is a large data set used in both problems posed each day.

## Setup and Running

To set up you can clone the repo and then navigate to a particular day's problem 
i.e 01, 02 etc. You can of course use the C compiler of your choice to manually compile the code, then run the executable and specify an input file i.e 
`./main finaldata.txt`

Otherwise you can use the Makefiles and run `make`.

Currently the Makefiles depend on:

Make:   https://www.gnu.org/software/make/
GCC:    https://gcc.gnu.org/ 
Clang:  https://clang.llvm.org/

For now this will spew a lot of debug and testing information to the console but it's automatically taking the input files (sample.txt and finaldata.txt) and reading them into the program to generate the solutions. Eventually we will refactor the Makefiles to not create test and debug output by default and create an optimised version of the binary.

## Thinking and Goals

1. Use the C89 C standard: https://en.wikipedia.org/wiki/ANSI_C

2. Allow for cross platform compilation for Windows, MacOS and Linux.

3. Create one project that contains all the solutions. Maybe we have one main.c per solution and multiple Makefiles, or we try and incorporate it all together. To be determined as things evolve.

4. As we solve problems, creature re-usable functions that can be placed into     library files for use in other problems and projects.

5. Do our best to minimise memory allocation and usage. For example, try and use dynamic sizes rather than hardcoded static values.

6. Take the opportunity to learn maths and algorithms from the problems. For example if, we need to sort an array, implement several different sorts and learn the differences. 

7. Have fun and learn more C.

## Development Environment

We develop using Visual Studio Code on Debian Linux and most testing and debugging is done on that OS. We use Makefiles to help automate compilation and build tasks and GitHub Actions to test compilation on MacOS and Windows.

## Automated Testing

We would like to implement at least partial test coverage using the Unity framework: https://www.throwtheswitch.org/unity

## License

All code is licensed under the MIT License. See LICENSE.txt and blurbs at the top of each code file.

## Contributing

We welcome:

1. Code that resolves errors and bugs.
2. Code that is more efficient (completes tasks faster, or uses less memory)
3. Proposed approaches that facilitate a greater number of re-usable functions and make the code more readable and maintainable.
4. Code that improves cross platform compatibility or resolves issues with specific compilers and operating systems.

Please be aware that both the code style and design may still be changing as we solve more problems. Don't let that put you off.