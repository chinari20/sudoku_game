#include <stdio.h>

// Function prototypes for various parts of the Sudoku solver
int input_grid();           // Reads and stores the input grid
int validate_grid();        // Validates if the current grid is a valid Sudoku puzzle
int solve_cell(int row, int column);  // Solves the puzzle using recursion (backtracking)
int is_valid(int row, int column, int value);  // Checks if a value is valid at a specific cell
void print_grid();          // Prints the Sudoku grid

// Global variables to store the original and current grid
int original[9][9], grid[9][9];

int main()
{
    // Input the grid from the user
    if (! input_grid()) {
        printf("error: unable to input grid\n");
        return 0;  // Exit if the grid input fails
    }

    // Validate the input grid (check if it's a valid Sudoku puzzle)
    printf("\nvalidating puzzle... ");
    if (! validate_grid()) {
        printf("invalid!\n");
        return 0;  // Exit if the grid is invalid
    }

    printf("valid.\n\nsolving following puzzle:\n");
    print_grid();  // Display the puzzle to the user

    // Try to solve the Sudoku puzzle using backtracking
    if (! solve_cell(0, 0)) {
        printf("\nunfortunately, your puzzle is unsolvable\n");
        return 0;  // Exit if the puzzle can't be solved
    }

    // Print the solved grid if the puzzle is successfully solved
    printf("\npuzzle solved:\n");
    print_grid();
    return 0;  // Successfully solved
}

// Function to input the Sudoku grid from the user
int input_grid()
{
    int character, row, column;
    row = column = 0;

    // Prompt user for each row of the grid
    printf("enter rows line by line. use numbers for known cells, ");
    printf("zero or dot for missing cells.\nrow 1: ");

    while (row < 9) {
        character = getchar();  // Read each character from input

        // Check if the character is a valid Sudoku number or placeholder (dot)
        if ('0' <= character && character <= '9' || character == '.') {
            if (column > 8) {
                printf("error: each row has maximum 9 cells\n");
                return 0;  // Exit if there are too many columns
            }

            // Store the input value in the grid and original array
            if (character != '.')
                original[row][column] = grid[row][column] = character - '0';

            column++;  // Move to the next column

        } else if (character == '\n') {
            column = 0;  // Start a new row after entering a complete line
            row++;  // Move to the next row
            printf("row %i: ", row + 1);
        }
    }

    return 1;  // Successfully input the grid
}

// Function to validate the grid (check if the current grid is valid)
int validate_grid()
{
    int i, j;

    // Check all filled cells to ensure they are valid according to Sudoku rules
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            if (grid[i][j] && !is_valid(i, j, grid[i][j]))  // Skip empty cells
                return 0;  // Return false if any cell violates Sudoku rules

    return 1;  // Return true if the grid is valid
}

// Function to solve the Sudoku puzzle using backtracking
int solve_cell(int row, int column)
{
    int number = 1;

    // Move to the next column or row if necessary
    if (column == 9) {
        column = 0;
        row++;
    }

    if (row == 9)
        return 1;  // Puzzle is solved if we have filled all cells

    // Try all numbers (1-9) for the current cell
    while (number < 10) {
        // If the number is valid for this cell, place it and move to the next cell
        if (is_valid(row, column, number)) {
            grid[row][column] = number;

            // Recursively solve the next cell
            if (solve_cell(row, column + 1))
                return 1;
        }

        grid[row][column] = 0;  // Backtrack if the number doesn't work
        number++;  // Try the next number
    }

    return 0;  // Return false if no solution was found for this cell
}

// Function to check if a value is valid at a specific position
int is_valid(int row, int column, int value)
{
    int i, j, r, c;

    // Check if the cell is predefined in the original grid
    if (original[row][column] != 0)
        if (original[row][column] != value)
            return 0;

    // Check for the value in the same row
    for (i = 0; i < 9; i++)
        if (i != column && grid[row][i] == value)
            return 0;  // Return false if the value is already in the row

    // Check for the value in the same column
    for (i = 0; i < 9; i++)
        if (i != row && grid[i][column] == value)
            return 0;  // Return false if the value is already in the column

    // Check the 3x3 sub-grid (box)
    r = (row / 3) * 3;
    c = (column / 3) * 3;
    for (i = r; i < r + 3; i++)
        for (j = c; j < c + 3; j++)
            if (i != row || j != column)
                if (grid[i][j] == value)
                    return 0;  // Return false if the value is in the 3x3 box

    return 1;  // Return true if the value is valid
}

// Function to print the Sudoku grid
void print_grid()
{
    int i, j;

    // Print the grid with borders between rows and columns
    for (i = 0; i < 10; i++) {
        if (i % 3 == 0)
            printf("+-------+-------+-------+\n");

        if (i == 9)
            return;

        for (j = 0; j < 9; j++) {
            if (j % 3 == 0)
                printf("| ");

            // Print the number or a dot for empty cells
            grid[i][j] != 0 ? printf("%d ", grid[i][j]) : printf(". ");
        }

        printf("|\n");
    }
}
