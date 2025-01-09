#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    //test if the number of arguments is correct. If not, return error.
    if(argc != 2) {
        cerr << "USAGE: caprot [rotation]\n";
        exit(1);
    }

    //save rotation
    long long int rotation = stoi(argv[1]);

    //otherwise, read the next line
    string input;
    getline(cin, input);

    //create a vector of indices of capitalized letters
    //also puts all characters to lower-case after checking
    vector<int> capitalIndices;
    for(unsigned int i=0; i<input.size(); i++) {
        if(isupper(input[i])) {
            capitalIndices.push_back(i);
            input[i] = tolower(input[i]);
        }
    }

    //Add rotation to indices; loops around if necessary
    for(unsigned int i=0; i<capitalIndices.size(); i++) {
        //must cast .size() as an int to % correctly
        int newIndex = (capitalIndices[i] + rotation) % int(input.size());
        if(newIndex < 0) {
            newIndex = int(input.size()) + newIndex;
        }
        capitalIndices[i] = newIndex;
    }

    //Assigns the capitalizations if possible
    for(int index : capitalIndices) {
        input[index] = toupper(input[index]);
    }
    
    //prints result
    cout << input << "\n";
    return 0;
}