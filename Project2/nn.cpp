#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <set>

using namespace std;

set<set<int>> feature_set;

struct Node{
    set<int> features;
    double values;

    Node(){
        values = 0;
    }
};

vector<vector<double> > parseData(const string &fileName)
{
    int rows = 0, cols = 0;
    string line;
    double item;
    vector<vector<double> > data;

    ifstream file(fileName);

    while (getline(file, line))
    {
        cols = 0;
        rows++;
        vector<double> row;

        stringstream ss(line); // Set up up a stream from this line
        while (ss >> item)
        {
            cols++; // Each item delineated by spaces adds one to cols
            row.push_back(item);
        }
        data.push_back(row);
    }
    file.close();

    cout << "This dataset has " << cols - 1 << " features (not including the class attribute), with " << rows << " instances.\n";
    return data;
}

int main(){
    cout.precision(4);

    cout << "Welcome to Celvin Lizama Pena's Feature Selection Algorithm.\nType in the name of the file to test:\n";
    string filename;
    cin >> filename;

    cout << "Type the number of the algorithm you want to run.\n1)\tForward Selection\n2)\tBackward Elimination\n";
    int choice;
    cin >> choice;

    vector<vector<double>> data = parseData(filename);

    Node* initState = new Node();
    int numFeatures = 0;
    for(int i = 1; i < data[0].size(); i++){
        initState->features.insert(i);
        numFeatures++;
    }

    if(choice == 1){
        feature_set.insert(initState->features);
        cout << "Running nearest neighbor with all " << numFeatures << " features, using \"leave-one-out\" evaluation, I get an accuracy of \n";
    }
}