#include<iostream>
#include<vector>
#include<utility>
#include<queue>
#include<set>
#include<stack>
#include<math.h>

using namespace std;

#define ROWS 3
#define COLS 3

typedef vector<vector<int> > Grid; // A 2-d vector is used as the puzzle grid
typedef pair<int, int> Pair;

int choice = 0;
int numStatesExpanded = 0;
int maxFrontierSize = 0;

struct Node{
    Grid grid;
    Node* parent; // For backtracking, points to the parent of the curr node
    Pair blankTile; // Keeps track of the location of the blank tile for this node
    double f,g,h;
    string op;

    Node(){
        parent = nullptr;
        blankTile = make_pair(-1,-1);
        f = 0, g=0, h=0;
        op = "";
    }
};

void printGrid(const Grid& grid){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){
    cout << "Welcome to my 8-Puzzle solver, Type '1' to use a default puzzle, or '2' to create your own.\n";
    int puzzleChoice;
    cin >> puzzleChoice;

    Grid grid;
    Pair blankTile;

    if(puzzleChoice == 1){
        grid = {
            {1,2,3},
            {4,8,0},
            {7,6,5}
        };
        blankTile = make_pair(1,2);
    }

    else if(puzzleChoice == 2){
        cout << "Enter your puzzle, using a zero to represent the blank. Please only enter valid 8-puzzles, delimiting the numbers with a space. Enter RETURN only when finished.\n";

        for(int i = 0; i < ROWS; i++){
            string rowName = "";

            if(i == 0)
                rowName = "first";
            else if(i == 1)
                rowName = "second";
            else
                rowName = "third";

            cout << "Enter the " << rowName << " row:\t";

            vector<int> row;

            for(int j = 0; j < COLS; j++){
                int tile;
                cin >> tile;

                if(tile == 0){
                    blankTile = make_pair(i,j);
                }
                
                row.push_back(tile);
            }
            grid.push_back(row);
        }
    }

    Node* initNode = new Node();
    initNode->grid = grid;
    initNode->blankTile = blankTile;

    printGrid(grid);
    cout << "blank tile: " << initNode->blankTile.first << ", " << initNode->blankTile.second << endl;

    return 0;
}