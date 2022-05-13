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

double manhattanDistanceHeuristic(Grid grid){
    double h = 0;

    Grid goal = {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            int tile = grid[i][j];
            if(tile != goal[i][j]){
                switch(tile){
                    case 1:
                        h += abs(i-0) + abs(j-0);
                        break;
                    case 2:
                        h += abs(i-0) + abs(j-1);
                        break;
                    case 3:
                        h += abs(i-0) + abs(j-2);
                        break;
                    case 4:
                        h += abs(i-1) + abs(j-0);
                        break;
                    case 5:
                        h += abs(i-1) + abs(j-1);
                        break;
                    case 6:
                        h += abs(i-1) + abs(j-2);
                        break;
                    case 7:
                        h += abs(i-2) + abs(j-0);
                        break;
                    case 8:
                        h += abs(i-2) + abs(j-1);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return h;
}

double missingTileHeuristic(Grid grid){
    double h = 0;

    Grid goal = {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(grid[i][j] != goal[i][j])
                h++;
        }
    }

    return h;
}

Grid makeNode(const Grid& grid, Pair blankTile, Pair newPosition){
    int temp;
    Grid newGrid = grid;

    temp = newGrid[newPosition.first][newPosition.second];
    newGrid[newPosition.first][newPosition.second] = 0;
    newGrid[blankTile.first][blankTile.second] = temp;

    return newGrid;
}

void moveBlankTile(Node* node, int coord, int x, int y, bool isNegative, string direction, vector<Node*>& children){
    Node* child = new Node();
    Pair blankTile = node->blankTile;
    Pair newPosition;   // The position of the balnk tile once it is moved
    int newDir = isNegative ? coord - 1: coord + 1;

    if(newDir >= 0 && newDir < 3){
        newPosition = make_pair(blankTile.first + x, blankTile.second + y); // Calculate the new position of the blank tile

        child->parent = node;
        child->grid = makeNode(node->grid, blankTile, newPosition);
        child->blankTile = newPosition; // Update the blank tile
        child->g = node->g + 1;

        Grid grid = child->grid;

        switch (heuristic)
        {
            // Heuristic is Uniform Cost Search, so h(n) = 0
        case 1:
            break;
            // Missing Tile Heuristic
        case 2:
            child->h = missingTileHeuristic(grid);
            break;
            // Manhattan Distance Heuristic
        case 3:
            child->h = manhattanDistanceHeuristic(grid);
            break;
        default:
            break;
        }

        child->f = child->h + child->g;
        child->op = "Moving blank tile " + direction + "\n";
        children.push_back(child);
    }

}

void Expand(Node* node, vector<Node*>& children){
    Node* child = new Node();
    Pair blankTile;

    // Move blank tile up
    moveBlankTile(node, node->blankTile.first, -1, 0, true, "up", children);
    // Move blank tile down
    moveBlankTile(node, node->blankTile.first, 1, 0, false, "down", children);
    // Move blank tile right
    moveBlankTile(node, node->blankTile.second, 0, 1, false, "to the right", children);
    // Move blank tile left
    moveBlankTile(node, node->blankTile.second, 0, -1, true, "to the left", children);

    numStatesExpanded++;
}

void printTrace(Node* node){ 
    Node* curr = node;
    stack<Node*> sol;

    while(curr != nullptr){
        sol.push(curr);
        curr = curr->parent;
    }

    while(!sol.empty()){
        Node* top = sol.top();
        cout << top->op << endl;
        printGrid(top->grid);
        cout << endl;
        sol.pop();
    }
}

void generalSearch(Node* initState){
    set<Grid> visited;  // Keeps track of the nodes that we have already visited
    priority_queue<Node*, vector<Node*>, compareF> nodes; // Queueing function meant to keep the nodes in the frontier
    nodes.push(initState);   // Start the queue by inserting the initial state

    while(!nodes.empty()){
        if(nodes.size() > maxQueueSize)
            maxQueueSize = nodes.size();
        
        Node* node = nodes.top();
        Grid grid = node->grid;
        nodes.pop();

        // Check if the goal state has been reached
        bool isGoalState = goalTest(grid);

        if(isGoalState){
            cout << "Goal state!\n";
            printGrid(grid);

            cout << "----------- SOLUTION TRACE -------------\n";
            cout << "The solution depth was " << node->g << endl;
            cout << "\nTo solve this problem, the search algorithm expanded a total of " << numStatesExpanded << " nodes\n";
            cout << "The maximum number of nodes in the queue at any one time: " << maxQueueSize << endl << endl;
            printTrace(node);
            return;
        }

        cout << "The best state to expand with g(n) = " << node->g << " and h(n) = " << node->h << endl;
        printGrid(grid);
        cout << endl;

        visited.insert(node->grid); // Mark the node as visited

        vector<Node*> children; // Keeps track of the children that are found by expanding the node
        Expand(node, children);

        for(auto child: children){
            set<Grid>::iterator it1 = visited.find(child->grid);

            if(it1 == visited.end())
                nodes.push(child);
        }
    }
    cout << "failure\n";
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

    cout << "Select the heuristic to be used:\n";
    cout << "1) Uniform Cost Search\n";
    cout << "2) A* with the Misplaced Tile Heuristic\n";
    cout << "3) A* with the Manhattan Distance Heuristic\n\n";

    cin >> heuristic;
    cout << endl;

    generalSearch(initNode);

    return 0;
}