# Console Image Printer

## Description

This is an application that prints images
to console using ANSI escape sequences.
Requires terminal supporting colored output.
Currently only 24-bit BMP format is supported.

## Installation

First of all, you need to install following dependencies:

```
make gcc [gdb]
```

After that simply run `make` which will build & test the app.

## Usage

```
./app [ -<maxwidth> ] image.bmp
```

Where `app` is the name of the executable, `<maxwidth>` is maximum width on the output, `image.bmp` is the source image

For example, try running `./build/print-pic -30 soup.bmp`
to get a tiny picture or `./build/print-pic -0 soup.bmp`
for huge image of original size.
