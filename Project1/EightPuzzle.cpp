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

int heuristic = 0;          // Choice of heuristic to use
int numStatesExpanded = 0;  // Keeps track of the number of expanded states
int maxQueueSize = 0;       // Keeps track of the maximum size of the queue

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

// Compare struct used so that the priority queue knows how to order the Nodes
struct compareF{
    bool operator()(Node* n1, Node* n2){
        return n1->f > n2->f;
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

// Compares a given state to the goal and returns true if they are the same
bool goalTest(Grid grid){
    Grid goalState = {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(grid[i][j] != goalState[i][j])
                return false;
        }
    }
    return true;
}

Grid makeGrid(const Grid& grid, Pair blankTile, Pair newPosition){
    int temp;
    Grid newGrid = grid;

    temp = newGrid[newPosition.first][newPosition.second];
    newGrid[newPosition.first][newPosition.second] = 0;
    newGrid[blankTile.first][blankTile.second] = temp;

    return newGrid;
}

void moveBlankTile(Node* node, int coord, int x, int y, bool isNegative, string direction){
    Node* child = new Node();
    Pair blankTile = node->blankTile;
    Pair newPosition;   // The position of the balnk tile once it is moved
    int newDir = isNegative ? coord - 1: coord + 1;

    if(newDir >= 0 && newDir < 3){
        newPosition = make_pair(blankTile.first + x, blankTile.second + y);
        cout << "blank tile: " << newPosition.first << " " << newPosition.second << endl;

        child->parent = node;
        child->grid = makeGrid(node->grid, blankTile, newPosition);
        cout << "New grid:\n";

        switch (heuristic)
        {
            // Heuristic is Uniform Cost Search, so h(n) = 0
        case 1:
            cout << "Uniform Cost Search\n";
            break;
            // Missing Tile Heuristic
        case 2:
            cout << "Missing Tile Heuristic\n";
            break;
            // Euclidean Distance Heuristic
        case 3:
            cout << "Euclidean Distance Heuristic\n";
            break;
        default:
            break;
        }
        child->op = "Moving blank tile " + direction + "\n";
        cout << child->op;
        printGrid(child->grid);

    }

}

void expandState(Node* node){
    Node* child = new Node();
    Pair blankTile;

    // Move blank tile up
    moveBlankTile(node, node->blankTile.first, -1, 0, true, "up");
    // Move blank tile down
    moveBlankTile(node, node->blankTile.first, 1, 0, false, "down");
    // Move blank tile right
    moveBlankTile(node, node->blankTile.first, 0, 1, false, "to the right");
    // Move blank tile left
    moveBlankTile(node, node->blankTile.first, 0, -1, true, "to the left");

}

void generalSearch(Node* initState){
    set<Grid> exploredSet;

    priority_queue<Node*, vector<Node*>, compareF> nodes; // Queueing function meant to keep the nodes in the frontier
    nodes.push(initState);   // Start the queue by inserting the initial state

    // bool isInitState = true;

    while(!nodes.empty()){
        if(nodes.size() > maxQueueSize)
            maxQueueSize = nodes.size();
        
        Node* node = nodes.top();
        Grid grid = node->grid;
        nodes.pop();

        // printGrid(frontNode->grid);

        // Check if the goal state has been reached
        bool isGoalState = goalTest(node->grid);

        if(isGoalState){
            cout << "Goal!\n";
            printGrid(node->grid);
            return;
        }

        cout << "The best state to expand with g(n) = " << node->g << " and h(n) = " << node->h << endl;
        expandState(node);

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

    // printGrid(grid);
    // cout << "blank tile: " << initNode->blankTile.first << ", " << initNode->blankTile.second << endl;

    cout << "Select the heuristic to be used:\n";
    cout << "1) Uniform Cost Search\n";
    cout << "2) A* with the Misplaced Tile Heuristic\n";
    cout << "3) A* with the Eulcidean Distance Heuristic\n\n";

    cin >> heuristic;
    cout << endl;

    generalSearch(initNode);

    return 0;
}