// Object-oriented brute-force (DFS + backtracking) Sudoku solver.
// Finds ALL solutions for each puzzle file.
//
// Build: g++ -std=c++17 -O2 -o sudoku sudoku.cpp
// Run:   ./sudoku puzzle1.txt puzzle2.txt ... (defaults to puzzle1..5.txt)

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Board: holds the 9x9 grid and knows how to read/print itself and
// answer "what digits may legally go in this cell?".
// ---------------------------------------------------------------------------
class Board {
public:
    enum { SIZE = 9, BLOCK = 3, EMPTY = 0 };

    Board() {
        for (auto &row : cells_) row.fill(EMPTY);
    }

    // Load a puzzle file. '_' (or '0' / '.') is a blank. Returns false on failure.
    bool load(const std::string &filename) {
        std::ifstream in(filename);
        if (!in) return false;
        int r = 0, c = 0;
        char ch;
        while (r < SIZE && in.get(ch)) {
            if (ch >= '1' && ch <= '9') {
                cells_[r][c] = ch - '0';
            } else if (ch == '_' || ch == '0' || ch == '.') {
                cells_[r][c] = EMPTY;
            } else {
                continue;  // whitespace / newlines
            }
            if (++c == SIZE) { c = 0; ++r; }
        }
        return r == SIZE;
    }

    int get(int r, int c) const { return cells_[r][c]; }
    void set(int r, int c, int v) { cells_[r][c] = v; }

    // Find the first empty cell (row-major). Returns false if board is full.
    bool findEmpty(int &row, int &col) const {
        for (int r = 0; r < SIZE; ++r)
            for (int c = 0; c < SIZE; ++c)
                if (cells_[r][c] == EMPTY) { row = r; col = c; return true; }
        return false;
    }

    // Candidate digits for cell (r,c): not used in its row, column, or block.
    std::vector<int> candidates(int r, int c) const {
        std::array<bool, SIZE + 1> used{};
        for (int i = 0; i < SIZE; ++i) {
            used[cells_[r][i]] = true;
            used[cells_[i][c]] = true;
        }
        int br = (r / BLOCK) * BLOCK, bc = (c / BLOCK) * BLOCK;
        for (int i = 0; i < BLOCK; ++i)
            for (int j = 0; j < BLOCK; ++j)
                used[cells_[br + i][bc + j]] = true;

        std::vector<int> result;
        for (int d = 1; d <= SIZE; ++d)
            if (!used[d]) result.push_back(d);
        return result;
    }

    // Check that the givens don't already violate a rule.
    bool isConsistent() const {
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                int v = cells_[r][c];
                if (v == EMPTY) continue;
                // Temporarily blank the cell and see if v is still a candidate.
                Board copy = *this;
                copy.cells_[r][c] = EMPTY;
                bool ok = false;
                for (int cand : copy.candidates(r, c))
                    if (cand == v) { ok = true; break; }
                if (!ok) return false;
            }
        }
        return true;
    }

    void print(std::ostream &os) const {
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (cells_[r][c] == EMPTY) os << "_ ";
                else os << cells_[r][c] << ' ';
            }
            os << '\n';
        }
    }

private:
    std::array<std::array<int, SIZE>, SIZE> cells_;
};

// ---------------------------------------------------------------------------
// Solver: recursive depth-first search with backtracking.
// Collects every solution rather than stopping at the first.
// ---------------------------------------------------------------------------
class Solver {
public:
    explicit Solver(const Board &puzzle) : board_(puzzle) {}

    // Returns all solutions (empty vector => no solution).
    const std::vector<Board> &solveAll() {
        solutions_.clear();
        if (board_.isConsistent()) search();
        return solutions_;
    }

private:
    void search() {
        int r, c;
        if (!board_.findEmpty(r, c)) {      // k == 81: complete solution
            solutions_.push_back(board_);
            return;
        }
        for (int cand : board_.candidates(r, c)) {
            board_.set(r, c, cand);         // try it
            search();                       // recurse with k+1 cells filled
            board_.set(r, c, Board::EMPTY); // backtrack
        }
        // No candidates (or all failed) => fall through, reporting nothing.
    }

    Board board_;
    std::vector<Board> solutions_;
};

// ---------------------------------------------------------------------------
// PuzzleRunner: loads one puzzle file, solves it, and reports the results.
// ---------------------------------------------------------------------------
class PuzzleRunner {
public:
    explicit PuzzleRunner(const std::string &filename) : filename_(filename) {}

    void run(std::ostream &os) {
        os << "Puzzle file: " << filename_ << "\n";
        Board puzzle;
        if (!puzzle.load(filename_)) {
            os << "Could not read a valid puzzle from this file.\n\n";
            return;
        }

        os << "Puzzle:\n";
        puzzle.print(os);
        os << '\n';

        Solver solver(puzzle);
        const std::vector<Board> &solutions = solver.solveAll();

        if (solutions.empty()) {
            os << "No solution found\n";
        } else {
            os << "Solutions found: " << solutions.size() << "\n\n";
            for (size_t i = 0; i < solutions.size(); ++i) {
                os << "Solution " << (i + 1) << ":\n";
                solutions[i].print(os);
                os << '\n';
            }
        }
        os << "----------------------------------------\n\n";
    }

private:
    std::string filename_;
};

int main(int argc, char *argv[]) {
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) files.push_back(argv[i]);
    if (files.empty())
        for (int i = 1; i <= 5; ++i)
            files.push_back("puzzle" + std::to_string(i) + ".txt");

    for (const std::string &f : files) {
        PuzzleRunner runner(f);
        runner.run(std::cout);
    }
    return 0;
}