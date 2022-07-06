# POGL project

https://github.com/Paulopacads/POGL-FFT-Ocean

## Introduction

This project aims to create a realistic ocean rendering using a method called the Fast Fourier Transformation.

I'm a student and this is my project for an OpenGL course at EPITA teached by J. Fabrizio for the IMAGE 2023 promotion.

NEVER COPY PASTE A PROJECT YOU FOUND ON THE INTERNET. Even tho you're a student, you SHOULD NEVER copy paste this project for your own grading. You MAY use this to see a rendering example.

Our team is composed of two maintainers (students):
- `melanie.tcheou`
- `paul.galand`

## Example

![](https://i.imgur.com/UKIcz44.png)

*Program executed with default parameters

Demo: [click here!](https://youtu.be/w0zbNGq5Pls)

## Instructions

### Compile

You will need CMake to compile this project.
```sh
cd ./build
cmake ..
make
```

This will create an executable called `ocean`.

### Execution

You should be able to execute the previously compiled program directly without any trouble.
```sh
./ocean
```

However, if you want to personalize your experience, you may find all the informations you'll need by simply asking for help!
```sh
./ocean --help
```

### Usage

Keybinds for this program are the following.
- `w` Forward (`z` in AZERTY)
- `a` Left (`q` in AZERTY)
- `d` Right
- `s` Backward
- `p` Pause (cursor appears and moving is unavailable)

## Conclusion

Have fun! But be careful when playing around with parameters, you may need a good GPU to launch it.

## Details

This program has been tested and runs perfectly on our two different machines, on Manjaro KDE:
|      Owner     |               CPU               |           GPU           |  RAM  |
| -------------- | ------------------------------- | ----------------------- | ----- |
| melanie.tcheou | 12 * Intel Core i7-8750H 2.2GHz | GTX 1060M               | 16GiB |
| paul.galand    | 8 * Intel Core i7-10510U 1.8GHz | Mesa Intel UHD Graphics | 16GiB |
