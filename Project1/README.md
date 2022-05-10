# Eight Puzzle Solver
This program is written in C++, in order to compile and run it, please use the following command:

g++ -std=c++11 eightPuzzle.cpp && ./a.out

It is important that C++ 11 be used, as the program will give an error code if not using this version.

Once compiled, the program will prompt the user to choose whether to use the default puzzle, or enter a custom puzzle.
Please make sure that you enter either "1" for default puzzle or "2" for custom puzzle.
The default puzzle is as follows:

        1 2 3
        4 8 0
        7 6 5

Note that the 0 represents the empty or blank tile.

If you choose to enter your own puzzle, please enter it one row at a time. The acceptable values are single digits
ranging from 0 - 8. Each value must be unique and seperated by a space. Enter one row at a time, with each row consisting of three values. Upon finishing entering a row, press enter to start the next row.

When finished, you will be prompted to choose which algorithm to use. Please enter only the values 1, 2, or 3, depending on your choice and press enter. 

Once this is done, the program will begin executing. It will start printing trace of the algorithm, which shows the expanded nodes and the g(n) and h(n) values. Once the goal state is reached, the program will print out the number of nodes expanded and the maximum frontier size.

Finally, below the section label solution, shows the operations and puzzle grids taken for the optimal solution.
