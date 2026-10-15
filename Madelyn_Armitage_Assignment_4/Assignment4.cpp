// EECS 348 Assignment 4 - Sudoku Solver

 // Description: This program uses object-oriented programming and recursive backtracking
 // to solve five Sudoku puzzles. It reads each puzzle from a text file,
 // checks possible numbers against Sudoku rules, and prints all solutions
 // or indicates when no solution exists.

 // Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt

 // Outputs: Original puzzle, all valid solutions, or "No solution found"

 // Source: Modified from AI-generated code by ChatGPT

 // Author: Madelyn Armitage

 // Creation Date: 10/12/2026

 // Collaborators/Outside Sources: ChatGPT and Claude for code comparison

 //Throughout the code, chunks of comments prior to blocks were authored by me.


//The following code block includes several different operations to be used throughout the code.
//It includes fstream for file input operations, iostream for console output operations, sstream for row validation
//string for filenames, and vector for lists from the user
//following code block authored by ChatGPT
 #include <fstream>   // Provide file input operations.
#include <iostream>  // Provide console output operations.
#include <sstream>   // Provide string streams for validating each row.
#include <string>    // Provide strings for filenames, lines, and tokens.
#include <vector>    // Provide a list of filenames supplied by the user.

//following code block authored by ChatGPT
using namespace std; // Allow standard-library names without the std:: prefix.

//the sudoku class is used to group data about puzzles and their operations.
//the first block of code assigns variables of 9 and 3 to BOARD_SIZE and BLOCK_SIZE respectively
//in order to create uniformity and ease throughout the code
//an empty char is made to represent unfilled cells
//this code also initializes a solution count variable to keep track of how many solutions a puzzle has
//following code block authored by ChatGPT
class Sudoku { // Group puzzle data and solving operations inside one object.
private: // Keep internal puzzle state and helper methods inaccessible to callers.
    static constexpr int BOARD_SIZE = 9; // Define the number of rows, columns, and allowed digits.
    static constexpr int BLOCK_SIZE = 3; // Define the width and height of each smaller block.
    static constexpr unsigned char EMPTY = 0; // Represent an unfilled cell using zero.
    unsigned char board[BOARD_SIZE][BOARD_SIZE]{}; // Store each value in one byte instead of a typical four-byte int.
    int solutionCount = 0; // Track how many complete solutions have been printed.


    //The following block of code is a function that evaluates if a digit can be placed in a specific cell
    //It checks all positions in the adjacent rows and columns of the cell to see which digits 1-9 can be placed
    //following code block authored by ChatGPT
    bool canPlace(int row, int col, int number) const { // Determine whether a digit is legal in one cell.
        for (int i = 0; i < BOARD_SIZE; i++) { // Check all positions along the cell's row and column.
            if (i != col && board[row][i] == number) { // Detect the proposed number elsewhere in the row.
                return false; // Reject the number if it repeats in the row.
            } // Finish checking for a row conflict.
            if (i != row && board[i][col] == number) { // Detect the proposed number elsewhere in the column.
                return false; // Reject the number if it repeats in the column.
            } // Finish checking for a column conflict.
        } // Finish inspecting the row and column.


        //The following block of code checks what numbers can be placed in each cell based on the numbers present in the block
        //It starts by checking which 3x3 block the cell is present withing
        //Then, it checks every row and column sequence in that block
        //the number is rejected if it is already in the block of cells
        //block returns true when the sudoku row, column, and block rules are satisfied
        //following code block authored by ChatGPT
        const int startRow = (row / BLOCK_SIZE) * BLOCK_SIZE; // Locate the first row of the cell's 3x3 block.
        const int startCol = (col / BLOCK_SIZE) * BLOCK_SIZE; // Locate the first column of the cell's 3x3 block.
        for (int r = startRow; r < startRow + BLOCK_SIZE; r++) { // Visit every row in the same block.
            for (int c = startCol; c < startCol + BLOCK_SIZE; c++) { // Visit every column in the same block.
                if ((r != row || c != col) && board[r][c] == number) { // Check for a duplicate outside the chosen cell.
                    return false; // Reject a number already present in this block.
                } // Finish checking the current block position.
            } // Finish examining the columns in this block row.
        } // Finish examining every position in the block.
        return true; // Accept the number when all three Sudoku rules are satisfied.
    } // End the candidate-validation method.

    // The following block of code is a function to ensure that the given starting board follows all sudoku rules
    // It does this by iterating through every cell in the board
    // If a cell is not empty, it checks that the value in that cell is valid by comparing it to the 
    // result of that cell being called by the canPlace function
    //  false is returned if the numbers conflict
    // true is returned if starting values are valid
    //following code block authored by ChatGPT
    bool startingBoardIsValid() const { // Ensure the numbers provided in the file do not already conflict.
        for (int row = 0; row < BOARD_SIZE; row++) { // Visit every row in the original puzzle.
            for (int col = 0; col < BOARD_SIZE; col++) { // Visit every column in the original puzzle.
                if (board[row][col] != EMPTY && !canPlace(row, col, board[row][col])) { // Test each preset digit against the rules.
                    return false; // Reject a puzzle with conflicting given numbers.
                } // Finish examining whether the current clue conflicts.
            } // Finish checking a row's cells.
        } // Finish checking every row of the original puzzle.
        return true; // Confirm that the starting clues are consistent.
    } // End the original-board validation method.


    //following block of code is a function for recursive solving
    // This acts as storage for information on what row or column the recusion is currently on
    // It will store best row, best column, and what cell has the fewest possible candidates
    //following code block authored by ChatGPT
    void solveRecursive() { // Search recursively for every valid completion of the board.
        int bestRow = -1; // Record the selected empty cell's row, or -1 if none exists.
        int bestCol = -1; // Record the selected empty cell's column, or -1 if none exists.
        int fewestCandidates = BOARD_SIZE + 1; // Begin above the maximum possible candidate count.

        //The following code searches all cells to find what has the fewest candidates, by looking at 
        //unfilled cells. It counts all candidates by calling canPlace on all digits 1-9. 
        //If there are no valid candidates, it has to back track and abandon this path.
        //if it finds a cell with few candidates than the previous best, it records row col and number of candidates for the new cell
        //if candidates is 1, it breaks the loop and fills that cell
        //following code block authored by ChatGPT
        for (int row = 0; row < BOARD_SIZE; row++) { // Search all rows for an empty cell with few candidates.
            for (int col = 0; col < BOARD_SIZE; col++) { // Search every column in the current row.
                if (board[row][col] == EMPTY) { // Examine only cells not yet filled.
                    int candidates = 0; // Count legal numbers for this empty cell.
                    for (int number = 1; number <= BOARD_SIZE; number++) { // Try all Sudoku digits as possible candidates.
                        if (canPlace(row, col, number)) { // Check if this number obeys all Sudoku restrictions.
                            candidates++; // Count the number if it could be placed here.
                        } // Finish checking the proposed candidate.
                    } // Finish counting possible digits for this cell.
                    if (candidates == 0) { // Detect a cell that cannot accept any digit.
                        return; // Abandon this path so the earlier recursive call can backtrack.
                    } // Finish detecting a dead-end puzzle state.
                    if (candidates < fewestCandidates) { // Check if this cell is more constrained than the current choice.
                        fewestCandidates = candidates; // Remember its smaller number of possible digits.
                        bestRow = row; // Save this empty cell's row.
                        bestCol = col; // Save this empty cell's column.
                    } // Finish updating the selected empty cell.
                    if (fewestCandidates == 1) { // Recognize that no empty cell can have fewer than one legal digit.
                        break; // Stop searching the current row because the ideal cell has been found.
                    } // Finish checking the early-exit condition.
                } // Finish processing the current cell if it was empty.
            } // Finish this row or stop early after finding a one-candidate cell.
            if (fewestCandidates == 1) { // Determine whether the ideal next cell is already selected.
                break; // Stop scanning additional rows to avoid unnecessary checks.
            } // Finish checking whether to stop the outer search loop.
        } // Finish searching for the next empty cell.

        //the following block of code recognizes that if best row is -1, there are no empty cells left
        //the program will then increase the number of solutions
        //the code after finding a solution, prints the solution
        //a newline is printed so that there is a space between any following solutions
        //following code block authored by ChatGPT
        if (bestRow == -1) { // Recognize a solution when no empty cells remain.
            solutionCount++; // Increase the count of completed solutions.
            cout << "Solution " << solutionCount << ":\n"; // Label the newly discovered solution.
            printBoard(); // Print the completed board immediately without storing a copy.
            cout << '\n'; // Leave a blank line before another possible solution.
            return; // Return so earlier calls can try other valid candidates.
        } // Finish handling a completed Sudoku puzzle.

        //The following code places iterates through numbers that can be placed in empty cells
        //if the number can be placed in the cell, the program will place the value in the cell
        //The program will recursively keep trying to place values in empty cells until it finds a soltion or 
        //cannot place any other value
        //if a guess was incorrect, the cell tested is placed back to empty and a new digit is attempted
        //following code block authored by ChatGPT
        for (int number = 1; number <= BOARD_SIZE; number++) { // Try every digit in the selected empty cell.
            if (canPlace(bestRow, bestCol, number)) { // Only explore digits that currently obey Sudoku rules.
                board[bestRow][bestCol] = static_cast<unsigned char>(number); // Tentatively fill the selected cell.
                solveRecursive(); // Recursively search for every solution that follows this choice.
                board[bestRow][bestCol] = EMPTY; // Undo the choice to backtrack and test alternatives.
            } // Finish trying the current digit if it was legal.
        } // Finish all candidates for the selected cell.
    } // End the recursive backtracking method.

    //The following code functions primarily as a way to access the puzzles from the given files
    //It opens the file if possible
    //If the file cannot be opened, then function returns false
    //when a new file is opened, solution count is reset to 0
    //The program will reject files that have fewer than 9 rows or columns because it is not a complete puzzle
    //This code also takes in "_" as empty and stores them as 0
    //this code checks if any characters are outside of the expected character input of 1-9 or "-",
    //If characters are incorrect, function returns False
    //following code block authored by ChatGPT
public: // Make puzzle loading, printing, and solving available to the main program.
    bool loadPuzzle(const string& filename) { // Read and fully validate a Sudoku file.
        ifstream input(filename); // Open the requested puzzle file for reading.
        if (!input) { // Check whether the file could be opened successfully.
            return false; // Report failure if the file does not exist or cannot be read.
        } // Finish checking whether the file opened.
        solutionCount = 0; // Reset the solution total for the new file.
        for (int row = 0; row < BOARD_SIZE; row++) { // Require exactly nine puzzle rows.
            string line; // Hold the raw text of one puzzle row.
            if (!getline(input, line)) { // Attempt to read the next required row.
                return false; // Reject a file with fewer than nine rows.
            } // Finish checking the required row's presence.
            istringstream rowInput(line); // Read space-separated tokens from this row.
            for (int col = 0; col < BOARD_SIZE; col++) { // Require exactly nine cells in each row.
                string token; // Hold one cell's symbol so multi-character entries can be detected.
                if (!(rowInput >> token) || token.size() != 1) { // Reject missing or multi-character cell entries.
                    return false; // Report malformed row data instead of silently accepting it.
                } // Finish validating the token's presence and length.
                if (token[0] == '_') { // Interpret an underscore as an empty square.
                    board[row][col] = EMPTY; // Store a zero for the unfilled square.
                } else if (token[0] >= '1' && token[0] <= '9') { // Accept only digits one through nine.
                    board[row][col] = static_cast<unsigned char>(token[0] - '0'); // Convert the digit character to a small integer.
                } else { // Detect any symbol outside the permitted puzzle format.
                    return false; // Reject incorrect characters such as letters, periods, or zero.
                } // Finish converting the current cell token.
            } // Finish reading all nine expected tokens in this row.
            string extraToken; // Store anything unexpectedly remaining on the current row.
            if (rowInput >> extraToken) { // Check whether this row contains a tenth cell or other data.
                return false; // Reject rows containing more than nine cells.
            } // Finish checking for extra tokens on this row.
        } // Finish reading all nine required rows.
        string remainingLine; // Hold any additional lines beyond the required nine.
        while (getline(input, remainingLine)) { // Examine the rest of the file after the puzzle.
            if (remainingLine.find_first_not_of(" \t\r") != string::npos) { // Allow trailing blank lines but no additional puzzle data.
                return false; // Reject extra rows or unexpected characters after the grid.
            } // Finish checking the current trailing line.
        } // Finish confirming that no extra puzzle data exists.
        return true; // Report that the file contains exactly one properly formatted puzzle.
    } // End the file-loading and input-validation method.

    // print board is a function that iterates through a puzzle and prints the numbers to show the finished board
    //if the value is empty, an underscore gets displayed
    //
    //following code block authored by ChatGPT
    void printBoard() const { // Print the current 9x9 puzzle, showing blanks as underscores.
        for (int row = 0; row < BOARD_SIZE; row++) { // Visit each row of the grid.
            for (int col = 0; col < BOARD_SIZE; col++) { // Visit each cell in the current row.
                if (board[row][col] == EMPTY) { // Check whether this cell is blank.
                    cout << '_'; // Display an underscore for an empty cell.
                } else { // Handle a cell that already contains a digit.
                    cout << static_cast<int>(board[row][col]); // Print its numeric value rather than a character code.
                } // Finish printing the current cell's value.
                if (col != BOARD_SIZE - 1) { // Avoid adding a space after the final cell in a row.
                    cout << ' '; // Separate adjacent cells for readability.
                } // Finish checking whether a cell needs a following space.
            } // Finish printing all cells in this row.
            cout << '\n'; // Begin a new output line after each Sudoku row.
        } // Finish printing every row in the grid.
    } // End the board-display method.

    //the following block of code is a function to solve the puzzles
    //It starts by setting solution count to 0 for each new puzzle
    //It then calls starting board is valid, to ensure the input is good
    //if solution count is 0, the program prints "No solution found"
    //Otherwise, the program will print the number of solutions found
    //following code block authored by ChatGPT
    void solve() { // Validate the given clues and search for every solution.
        solutionCount = 0; // Start a fresh count for the current puzzle.
        if (startingBoardIsValid()) { // Avoid exploring puzzles with contradictory starting clues.
            solveRecursive(); // Solve the valid starting board by recursive backtracking.
        } // Finish conditionally searching the current puzzle.
        if (solutionCount == 0) { // Check whether the search found no completed grids.
            cout << "No solution found\n"; // Print the required message for an unsolvable puzzle.
        } else { // Handle puzzles that have one or more valid completions.
            cout << "Solutions found: " << solutionCount << '\n'; // Report the number of solutions printed.
        } // Finish displaying the solution-count result.
    } // End the public solving method.
}; // Finish defining the Sudoku class.

//the following block of code is the main function
//it takes in user specified file names if the user gave them in the command line
//it takes the file names and adds them to the list of file names that need puzzle solutions to
//if no file names are given, the program uses the five given puzzle files hardcoded into the program
//following code block authored by ChatGPT
int main(int argc, char* argv[]) { // Process requested puzzle files and run the Sudoku solver.
    vector<string> filenames; // Store the filenames that need to be solved.
    if (argc > 1) { // Check whether the user specified any filenames in the command line.
        for (int i = 1; i < argc; i++) { // Visit each filename argument after the program name.
            filenames.push_back(argv[i]); // Add the filename to the list of puzzles to solve.
        } // Finish collecting command-line filenames.
    } else { // Provide the assignment's five filenames when no arguments are supplied.
        for (int i = 1; i <= 5; i++) { // Build the five default puzzle filenames in order.
            filenames.push_back("puzzle" + to_string(i) + ".txt"); // Add a default puzzle name to the list.
        } // Finish constructing the default puzzle list.
    } // Finish choosing between the user-supplied and default filenames.

    //The following program connects the sudoku logic to the files
    //The code iterates through each given file
    //It validates the format
    //This program prints the original board
    //Then, the solve function gets called to find the solutions of each puzzle
    //this block also prints lines that seperate each files' solution sets
    //following code block authored by ChatGPT
    for (const string& filename : filenames) { // Solve each requested file in sequence.
        Sudoku puzzle; // Create a new puzzle object with a clean board and solution count.
        cout << "File: " << filename << '\n'; // Display which input file is being processed.
        if (!puzzle.loadPuzzle(filename)) { // Attempt to read and validate the puzzle's file format.
            cout << "Unable to read a valid puzzle file.\n\n"; // Report missing or malformed input.
            continue; // Skip this file and continue with the next puzzle.
        } // Finish handling errors while loading the current puzzle.
        cout << "Original puzzle:\n"; // Label the original unsolved grid.
        puzzle.printBoard(); // Display the puzzle as it appeared in the input file.
        cout << '\n'; // Separate the original grid from the solution output.
        puzzle.solve(); // Print every solution or the required no-solution message.
        cout << "\n-----------------------------\n\n"; // Visually separate this file's results from the next one.
    } // Finish processing the requested puzzle files.
    return 0; // Exit successfully after processing all files.
} // End the main program.
