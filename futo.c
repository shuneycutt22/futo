/* futo - solve a futoshiki puzzle up to 9x9
 *
 * usage - futo
 *  (1) prompts user for puzzle filename
 *  (2) loads and parses puzzle
 *  (3) prints puzzle and solution to console (if solvable)
 *
 * NC State University
 * ECE 209 (001) Spring 2020
 * Program 2
 *
 * Sam Huneycutt, March 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "futo.h"


//checkSize
//find size of formatted futoshiki puzzle from .txt file
//input:
//  const char* filename: filename of puzzle
//return:
//  size of puzzle (int)
int checkSize(const char* filename) {
    int size;                           //size of futoshiki puzzle

    FILE* in;                           //create IO stream
    in = fopen(filename, "r");      //open file to IO stream

    if(!in)                             //if file not successfully opened, error
        return -1;

    fscanf(in, "%d", &size);            //get first character in file, this is size

    fclose(in);                         //close IO stream
    return size;                        //return size
}


//allocateArray
//dynamically allocates array of input size
//input:
//  const int N: size of array
//return:
//  pointer to allocated array (int*)
int* allocateArray(const int N) {
    return malloc(N * sizeof(int));      //allocate array, return pointer
}


//clearArray
//sets all values in an array to 0
//input:
//  int array[]: array to clear
//  const int N: size of array
void clearArray(int array[], const int N) {
    int i;
    for(i = 0; i < N; i++) {                //set each value of array to zero
        array[i] = 0;                       //from 0 to max index
    }
}


//storePuzzle
//parses futoshiki puzzle into puzzle and constraints arrays
//input:
//  const char* filename: filename of puzzle
//  int puzzle[]: puzzle array
//  int constraints[]: constraints array
//  const int size: size of puzzle
void storePuzzle(const char* filename, int puzzle[], int constraints[], const int size) {
    char tempchar, tempconst;               //hold temporary characters from IO stream
    int puzzleIndex = 0;                    //index for puzzle array
    int constraintIndex;                    //index for constraints array
    int count;                              //iteration count
    int pipeCount;                          //counts number of '|' characters

    FILE* in;                               //create IO stream
    in = fopen(filename, "r");          //open file

    //count '|' characters
    //each line has '|' at the beginning and the end
    //when pipeCount reaches (4 * size) - 2 , loop will exit
    for(pipeCount = 0; pipeCount < (4 * size) - 2; ) {
        //read characters until '|'
        while(tempchar != '|') {
            fscanf(in, "%c", &tempchar);
        }
        pipeCount++;                        //increment pipe counter
        //next character on IO stream first number in line

        //read 2 characters at a time
        //on puzzle lines -> first is number, second is horizontal constraint
        //on constraint lines -> first is vertical constraint, second is ' '
        //there are (size) sets of data on each line
        for(count = 0; count < size; count++) {
            fscanf(in, "%c%c", &tempchar, &tempconst);          //read characters from IO stream

            //tempchar contains puzzle data on puzzle lines
            //  contains vertical constraints on constraint lines
            //on puzzle lines -> switch statement stores integer into puzzle array based on character value
            //on constraint lines -> switch statement stores constraint data
            switch(tempchar) {
                case '-':
                    puzzle[puzzleIndex] = 0;
                    puzzleIndex++;
                    break;
                case '1':
                    puzzle[puzzleIndex] = 1;
                    puzzleIndex++;
                    break;
                case '2':
                    puzzle[puzzleIndex] = 2;
                    puzzleIndex++;
                    break;
                case '3':
                    puzzle[puzzleIndex] = 3;
                    puzzleIndex++;
                    break;
                case '4':
                    puzzle[puzzleIndex] = 4;
                    puzzleIndex++;
                    break;
                case '5':
                    puzzle[puzzleIndex] = 5;
                    puzzleIndex++;
                    break;
                case '6':
                    puzzle[puzzleIndex] = 6;
                    puzzleIndex++;
                    break;
                case '7':
                    puzzle[puzzleIndex] = 7;
                    puzzleIndex++;
                    break;
                case '8':
                    puzzle[puzzleIndex] = 8;
                    puzzleIndex++;
                    break;
                case '9':
                    puzzle[puzzleIndex] = 9;
                    puzzleIndex++;
                    break;
                case '^':
                    //calculate constraint index based on current position
                    //OR constraints[index] with 4 to set bit 2 to 1
                    //don't change other bits
                    constraintIndex = (((puzzleIndex / size) - 1) * size) + count;
                    constraints[constraintIndex] = constraints[constraintIndex] | 4;
                    break;
                case 'v':
                    //calculate constraint index based on current position
                    //OR constraints[index] with 8 to set bit 3 to 1
                    //don't change other bits
                    constraintIndex = (((puzzleIndex / size) - 1) * size) + count;
                    constraints[constraintIndex] = constraints[constraintIndex] | 8;
                    break;
                default:
                    break;
            }

            //tempconst contains horizontal constraint on puzzle lines
            //  contains ' ' on empty lines
            switch(tempconst) {
                case '<':
                    //OR constraints[index] with 1 to set bit 0 to 1
                    //don't change other bits
                    constraints[puzzleIndex-1] = constraints[puzzleIndex-1] | 1;
                    break;
                case '>':
                    //OR constraints[index] with 2 to set bit 1 to 1
                    //don't change other bits
                    constraints[puzzleIndex-1] = constraints[puzzleIndex-1] | 2;
                    break;
                case '|':
                    pipeCount++;            //increment pipe counter
                    break;
                default:
                    break;
            }
        }
    }
    fclose(in);                 //close IO stream
}


//readPuzzle
//given puzzle filename.
//  calculates puzzle size
//  allocates puzzle and constraints arrays
//  populates puzzle and constraints arrays
//input:
//  const char* filename: filename of puzzle
//  int** puzzle: pointer to pointer to puzzle array
//  int** constraints: pointer to pointer to constraints array
//  int* size: size of puzzle
//return:
//  1 if successfully read and parse puzzle
//  0 if failure
int readPuzzle(const char* filename, int** puzzle, int** constraints, int* size) {
    *size = checkSize(filename);        //calculate puzzle size

    if(*size == -1)                     //if file not successfully opened, error
        return 0;                       //return error code

    *puzzle = allocateArray(*size * *size);             //dynamically allocate puzzle array
    *constraints = allocateArray(*size * *size);        //dynamically allocate constraints array

    clearArray(*puzzle, *size * *size);                 //clear puzzle array
    clearArray(*constraints, *size * *size);            //clear constraints array

    storePuzzle(filename, *puzzle, *constraints, *size);     //parse and populate puzzle data into arrays

    return 1;                                                //return success code
}


//printOne
//used for puzzle printing
//prints character following digit
//  space or horizontal constraint or pipe
//input:
//  const int puzzle[]: puzzle array
//  const int constraints[]: constraints array
//  int index: array index of values to print
//  int count: number of sets printed so far in current line
//  const int size: size of puzzle
void printOne(const int puzzle[], const int constraints[], int index, int count, const int size) {
    //check constraints bit 0 and 1
    //if no constraints, set, print ' '
    // or '|' if end of line
    if ((constraints[index] & 3) == 0) {        //check constraints bit 0 and 1 == 0
        if(count < size - 1) {                  //check if end of line
            printf(" ");                        //if not end of line print ' '
        }
        else printf("|\n");                     //if end of line print '|'
    }

    //check constraints bit 0
    //if bit set, print <
    else if(constraints[index] & 1) {
        printf("<");
    }

    //check constraints bit 1
    //if bit set, print >
    else if(constraints[index] & 2) {
        printf(">");
    }
}


//printPuzzle
//prints puzzle to console in format outlined in program spec
//input:
//  const int puzzle[]: puzzle array
//  const int constraints: constraints array
//  const int size: size of puzzle
void printPuzzle(const int puzzle[], const int constraints[], int size) {
    int puzzleIndex = 0;            //index for puzzle array
    int constraintIndex = 0;        //index for constraints array
    int line;                       //line counter
    int count;                      //counter

    //complete puzzle is (2 * size) - 1 lines long
    //each iteration of for loop prints one complete line
    for (line = 1; line <= (2 * size) - 1; line++) {

        //odd lines contain puzzle numbers and horizontal constraint data
        if (line % 2 == 1) {                //check if line index odd
            printf("|");                    //start line with '|'

            //there are (size) numbers in a line
            //each iteration of for loop prints one number and the character following it
            for (count = 0; count < size; count++) {
                //read puzzle data from array
                // switch statement prints numerals or '-' depending on integer in puzzle array at index
                //  printOne function prints ' ' or '<' or '>' or '|' depending on constraints at index
                switch (puzzle[puzzleIndex]) {
                    case 0:
                        printf("-");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 1:
                        printf("1");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 2:
                        printf("2");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 3:
                        printf("3");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 4:
                        printf("4");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 5:
                        printf("5");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 6:
                        printf("6");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 7:
                        printf("7");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 8:
                        printf("8");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    case 9:
                        printf("9");
                        printOne(puzzle, constraints, puzzleIndex, count, size);
                        puzzleIndex++;
                        break;
                    default:
                        break;
                }
            }
        }

        //even lines contain vertical constraint data only
        else {
            printf("|");                                    //start line with '|'
            constraintIndex = (size * (line/2)) - size;     //set constraintIndex using line counter

            //there are up to (size) vertical constraints in a line
            for (count = 0; count < size; count++) {

                //check bit 2, if bit 2 set, print '^'
                if(constraints[constraintIndex] & 4) {
                    printf("^");            //print
                    constraintIndex++;      //increment constraintIndex
                    if(count + 1 < size)    //check if end of line
                        printf(" ");        //if not end of line, print ' '
                }

                //check bit 3. if bit 3 set print 'v'
                else if(constraints[constraintIndex] & 8) {
                    printf("v");            //print
                    constraintIndex++;      //increment constraintIndex
                    if(count + 1 < size)    //check if end of line
                        printf(" ");        //if not end of line, print ' '
                }

                //if no condition bits set, print ' '
                else {
                    printf(" ");            //print
                    constraintIndex++;      //increment constraintIndex
                    if(count + 1 < size)    //check if end of line
                        printf(" ");        //if not end of line, print ' '
                }
            }
            printf("|\n");                  //end line by printing '|' and newline
        }
    }
}


//checkFreeSpace
//finds the first empty space in array
// (i.e. index of first space in array == 0)
//input:
//  const int puzzle[]: puzzle array
//  const in size: size of puzzle
//return:
//  index of first empty space (int)
//  -1 if no empty space
int checkFreeSpace(const int puzzle[], const int size) {
    int index;

    //step through all indexes in array
    for(index = 0; index < size * size; index++) {
        if (puzzle[index] == 0)         //if puzzle at index == 0
            return index;               //return current index
        else continue;                  //else, try again
    }
    return -1;                          //if no empty space, return error code
}


//solve
//solves futoshiki puzzle, writing solution into input array
//uses recursive backtracking -- solve is called from within solve
//input:
//  int puzzle[]: puzzle array
//  int constraints[]: constraints array
//  int size: size of puzzle
//return:
//  1 if successfully placed number in puzzle
//  0 if could not place legal number in puzzle
int solve(int puzzle[], const int constraints[], int size) {
    int index;                      //index of puzzle array
    int row;                        //current row in puzzle
    int col;                        //current column in puzzle
    int num;                        //counter

    index = checkFreeSpace(puzzle, size);       //find index of first empty space in puzzle
    if(index == -1)                             //if no empty space
        return 1;                               //return 1 (solution successfully found)

    row = (index / size);           //calculate current row using index (1st row is 0)
    col = index - (row * size);     //calculate current column using index (1st column is 0)

    //index is set to first empty space in puzzle
    //try digits 1 to (size)
    // if illegal, try next digit
    // if legal, call solve (to solve next empty space)
    // if no legal value, loop exits. return 0 (no solution with current values)
    //  the solve function will backtrack and try more possible combinations at lower index
    for (num = 1; num <= size; num++) {
        if (!isLegal(row, col, num, puzzle, constraints, size)) {         //check if move is illegal
            continue;                                                     //if move illegal, continue to next value
        }
        else {      //move is legal
            puzzle[index] = num;                    //store number to puzzle array
            if(solve(puzzle, constraints, size))    //call solve recursively to solve next free space
                return 1;                           //if recursive solve succeeded, return 1 (success)
            else puzzle[index] = 0;                 //if recursive solve failed, reset puzzle at index to 0
        }
    }

    //if execution reaches this point, there is no solution with current setup
    //return 0 (error) to backtrack to a point where there could be a solution
    return 0;
}


//isLegal
//checks if a value is allowed at a point in a futoshiki puzzle
//input:
//  int row: current row
//  int col: current column
//  int num: number to check
//  const int puzzle[]: puzzle array
//  const in constraints[]: constraints array
//  int size: size of puzzle
//return:
//  1 if move legal
//  0 if move illegal
int isLegal(int row, int col, int num, const int puzzle[], const int constraints[], int size) {
    int index;              //index for puzzle array
    int startRow;           //index of beginning of row
    int endRow;             //index of beginning of next row
    int startCol;           //index of top of column
    int endCol;             //index after bottom of column
    int i;                  //counter

    //calculate index given row and col
    index = (row * size) + col;

    //calculate indexes for beginning/end of row/column
    startRow = row * size;
    endRow = startRow + size;
    startCol = (index % size);
    endCol = startCol + (size * size);

    //no number can appear twice in same row
    for(i = startRow; i < endRow; i++) {            //check every number in row
        if(i == index)                              //skip check if i == index
            continue;
        if(puzzle[i] == num)                        //if num appears in row
            return 0;                               //return 0 (value illegal)
    }

    //no number can appear twice in same column
    for(i = startCol; i < endCol; i = i + size) {   //check every number in column
        if(i == index)                              //skip check if i == index
            continue;
        if(puzzle[i] == num)                        //if num appears in column
            return 0;                               //return 0 (value illegal)
    }

    //if < to the right, num cannot be greater than number to the right
    if(constraints[index] & 1) {                    //check if bit 0 set
        if(puzzle[index + 1] != 0) {                //check if number to the right is not 0
            if(num >= puzzle[index + 1])            //check if num >= number to the right
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }
    }

    //if > to the right, num cannot be less than number to the right
    if(constraints[index] & 2) {                    //check if bit 1 set
        if(puzzle[index + 1] != 0) {                //check if number to the right is not 0
            if(num <= puzzle[index + 1])            //check if num <= number to the right
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }
    }

    //if ^ below, num cannot be greater than number below
    if(constraints[index] & 4) {                    //check if bit 2 set
        if(puzzle[index + size] != 0){              //check if number below is not 0
            if(num >= puzzle[index+size])           //check if num >= number below
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }
    }

    //if v below, num cannot be less than number below
    if(constraints[index] & 8) {                    //check if bit 3 set
        if(puzzle[index + size] != 0){              //check if number below is not 0
            if(num <= puzzle[index+size])           //check if num <= number below
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }
    }


    //if < to the right, can't put largest number
    if(constraints[index] & 1) {                    //check if bit 0 set
        if (num == size)                            //check if num == largest number
            return 0;                               //if all conditions evaluate true, return 0 (value illegal)
    }

    //if > to the right, can't put 1
    if(constraints[index] & 2) {                    //check if bit 1 set
        if(num == 1)                                //check if num == 1
            return 0;                               //if all conditions evaluate true, return 0 (value illegal)
    }

    //if ^ below, can't put largest number
    if (constraints[index] & 4) {                   //check if bit 2 set
        if (num == size)                            //check if num == largest number
            return 0;                               //if all conditions evaluate true, return 0 (value illegal)
    }

    //if v below, can't put 1
    if (constraints[index] & 8) {                   //check if bit 3 set
        if (num == 1)                               //check if num == 1
            return 0;                               //if all conditions evaluate true, return 0 (value illegal)
    }

/* For the remaining conditions, the (&& puzzle[index - x] != 0) condition is not necessary for my implementation.
 * The solve function solves the puzzle in ascending order, so the spaces to the left and above will always be filled.
 * However, the ZyBooks lab checks isLegal with an empty puzzle, so this check is necessary to pass the ZyBooks tests
 * despite the fact that it is unnecessary to correctly solve the puzzle. Therefore, it has been included.
 */

    //check constraint to the left
    if(index > 0) {     //there is no constraint to the left of the first space
        //if < to the left, num cannot be less than number to the left
        if (constraints[index - 1] & 1) {           //check if bit 0 set
            if (num <= puzzle[index - 1] && puzzle[index - 1] != 0)           //check if num <= number to the left
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }

        //if > to the left, num cannot be greater than number to the left
        if (constraints[index - 1] & 2) {           //check if bit 1 set
            if (num >= puzzle[index - 1] && puzzle[index - 1] != 0)     //check if num >= to the right
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }

    }
    //check constraint above
    if(index > size) {  //there is no constraint above the first row
        //if ^ above, num cannot be less than number above
        if (constraints[index - size] & 4) {        //check if bit 2 set
            if (num <= puzzle[index - size] && puzzle[index - size] != 0)        //check if num <= number above
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }

        //if v below, num cannot be greater than number above
        if (constraints[index - size] & 8) {        //check if bit 3 set
            if (puzzle[index - size] <= num && puzzle[index - size] != 0)        //check if num >= number above
                return 0;                           //if all conditions evaluate true, return 0 (value illegal)
        }
    }

    return 1;
}
