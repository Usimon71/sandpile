# Sandpile

## Description

Implemented a simplified [sandpile model](https://en.wikipedia.org/wiki/Abelian_sandpile_model), which allows you to save your states into a picture in **BMP** format .

The initial state is specified by the input file.

The grid size can change while the program is running.

### Initial state

The initial state is specified by a file with the value of the number of grains of sand in each cell, except empty ones. The grid size should be calculated based on this data - the minimum rectangle that all cells fall into.

File format:
Each line contains information about one cell, in the form (x-coordinates, y-coordinates, number of grains of sand), separated by a tab character.


The program must recalculate the state of the model according to the initial data, and also save intermediate states with a given frequency in the form of a picture in bmp format.

The picture for the current state is formed according to the following rules:

1. The size of the picture is equal to the size of the field.

2. Each pixel corresponds to a field cell.

3. The color of a pixel depends on the number of grains of sand in the cell.

   + 0 - white
   + 1 - green
   + 2 - yellow
   + 3 - purple
   + \> 3 - black

## Implementation 

Console application that supports the following command line arguments:

  **-i, --input** - [tsv file]

  **-o, --output** - path to the directory for saving pictures

  **-m, --max-iter** - maximum number of model iterations

  **-f, --freq** - frequency with which pictures should be saved (if 0, then only the last state is saved)

## Initial state

The initial state is specified by a file with the value of the number of grains of sand in each cell, except empty ones. The grid size should be calculated based on this data - the minimum rectangle that all cells fall into.

File format:
Each line contains information about one cell, in the form (x-coordinates, y-coordinates, number of grains of sand), separated by a tab character.


The program must recalculate the state of the model according to the initial data, and also save intermediate states with a given frequency in the form of a picture in bmp format.

The picture for the current state is formed according to the following rules:

1. The size of the picture is equal to the size of the field.

2. Each pixel corresponds to a field cell.

3. The color of a pixel depends on the number of grains of sand in the cell.

    + 0 - white
    + 1 - green
    + 2 - yellow
    + 3 - purple
    + \> 3 - black

4. Encoding 1 pixel takes no more than 4 bits.

The program should finish its work if the model has reached a stable state, or the number of the initially specified iteration.