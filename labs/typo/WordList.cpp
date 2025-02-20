#include <cmath>
#include <iostream>
#include "WordList.h"
#include "Heap.h"

WordList::WordList(std::istream& stream) {
    while(!stream.eof()) {
        std::string line;
        std::getline(stream, line);
        if(line[0] == '#' || line == "") { continue; }
        bool anyNonLower = false;
        for(size_t i=0; i<line.size(); i++) {
            if(!std::islower(line[i])) {
                anyNonLower = true;
                break;
            }
        }
        if(anyNonLower) { continue; }

        mWords.push_back(line);
    }
}

Heap WordList::correct(const std::vector<Point>& points, size_t maxcount, float cutoff) const {
    Heap similarWords = Heap(maxcount);
    for(size_t i=0; i<mWords.size(); i++) {
        //continue if the word is not the right length
        if(mWords[i].size() != points.size()) { continue; }

        double scoreSum = 0;
        std::string word = mWords[i];
        for(size_t j=0; j<word.size(); j++) {
            
            Point pointPosition = points[j];
            Point wordPosition = QWERTY[word[j]-97]; //ASCII - 97 = index at QUERTY; 97 = ASCII of 'a'
            //std::cout << "umm " << word[j]-97 << "\n";
            double distance = std::sqrt(std::pow(wordPosition.x-pointPosition.x, 2)+std::pow(wordPosition.y-pointPosition.y, 2));
            //std::string wordPosStr = "["+std::to_string(wordPosition.x)+", "+std::to_string(wordPosition.y)+"]";
            //std::string otherPosStr = "["+std::to_string(pointPosition.x)+", "+std::to_string(pointPosition.y)+"]";
            //std::cout << "distance between " << word[j] << "'s position: " << wordPosStr << "and other position: " << otherPosStr << "is " << distance << "\n";
            scoreSum += 1/(10*std::pow(distance, 2) + 1);
        }

        const float finalScore = scoreSum/points.size();

        if(finalScore < cutoff) { continue; }
        if(similarWords.count() == similarWords.capacity() && similarWords.top().score < finalScore) {
            similarWords.pushpop(word, finalScore);
        }
        else if(similarWords.count() < similarWords.capacity()) {
            similarWords.push(word, finalScore);
        }
    }
    return similarWords;
}