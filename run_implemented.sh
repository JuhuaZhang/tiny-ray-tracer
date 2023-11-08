#!/bin/bash

g++ -std=c++11 -O3 -I. -g main.cpp lodepng.cpp vec.cpp -Wall -Wextra -pedantic -o program
# Read implemented.txt file line by line
while IFS= read -r line
do
    # Run the program with the current line as an argument
    ./program "mprayfiles/$line"

    # Remove the .txt extension from the line
    png_name="${line%.txt}.png"

    # Run the compare.sh script with the PNG file as an argument
    ./compare.sh "$png_name"
done < implemented.txt

