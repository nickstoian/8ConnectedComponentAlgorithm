//Nicolas Stoian
//This program needs 3 command line arguments
//argv[1] "input" for text file representing the binary image
//argv[2] "output1" for pretty print of the result of the first, second, and third pass with eqArray and eqArray after management
//argv[3] "output2" for a binary image from the result of the third pass with header

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class ImageProcessing{
private:
    int** imgArray;
    int** zeroFramedArray;
    int* eqArray;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newLabel;

public:
    ImageProcessing();
    ImageProcessing(ifstream& inFile);
    ~ImageProcessing();
    void loadImage(ifstream& inFile);
    void zeroFramed();
    void prettyPrint(ofstream& outFile);
    void printEqArray(ofstream& outFile);
    void printBinaryImage(ofstream& outFile);
    static bool arrIsAllZero(int* arr, int arrSize);
    static bool arrContainsElement(int* arr, int arrSize, int element);
    static int arrMax(int* arr, int arrSize);
    static int arrMin(int* arr, int arrSize);
    void firstPass8Conn();
    void secondPass8Conn();
    void thirdPass8Conn();
    void updateEqArray(int index, int label);
    void manageEqArray();

};

int main(int argc, char* argv[]){
    ifstream inFile1;
    inFile1.open(argv[1]);
    ImageProcessing* image = new ImageProcessing(inFile1);
    inFile1.close();
    image->zeroFramed();
    image->firstPass8Conn();
    ofstream outFile1;
    outFile1.open(argv[2]);
    outFile1 << "Pretty print of Pass-1 and the eqArray" << endl;
    image->prettyPrint(outFile1);
    image->printEqArray(outFile1);
    image->secondPass8Conn();
    outFile1 << endl << endl << endl << "Pretty print of Pass-2 and the eqArray" << endl;
    image->prettyPrint(outFile1);
    image->printEqArray(outFile1);
    image->manageEqArray();
    outFile1 << endl << endl << endl << "Print of the eqArray after management of the eqArray" << endl << endl;
    image->printEqArray(outFile1);
    image->thirdPass8Conn();
    outFile1 << endl << endl << endl << "Pretty print of Pass-3 and the eqArray" << endl;
    image->prettyPrint(outFile1);
    image->printEqArray(outFile1);
    outFile1.close();
    ofstream outFile2;
    outFile2.open(argv[3]);
    image->printBinaryImage(outFile2);
    outFile2.close();
}

ImageProcessing::ImageProcessing(): imgArray(NULL), zeroFramedArray(NULL), eqArray(NULL), numRows(0), numCols(0), minVal(0), maxVal(0), newLabel(0){
}

ImageProcessing::ImageProcessing(ifstream& inFile): zeroFramedArray(NULL), newLabel(0){
    loadImage(inFile);
}

ImageProcessing::~ImageProcessing(){
    if(imgArray != NULL){
        for(int i = 0; i < numRows; i++){
            delete imgArray[i];
        }
    }
    if(zeroFramedArray != NULL){
        for(int i = 0; i < numRows; i++){
            delete zeroFramedArray[i];
        }
    }
    delete imgArray;
    delete zeroFramedArray;
    delete eqArray;
}

void ImageProcessing::loadImage(ifstream& inFile){
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    eqArray = new int [(numRows * numCols) / 4];
    for(int i = 0; i < (numRows * numCols) / 4; i++){
        eqArray[i] = i;
    }
    imgArray = new int* [numRows];
    for(int i = 0; i < numRows; i++){
        imgArray[i] = new int [numCols];
    }
    for(int row = 0; row < numRows; row++){
        for(int col = 0; col < numCols; col++){
            imgArray[row][col] = 0;
        }
    }
    for(int row = 0; row < numRows; row++){
        for(int col = 0; col < numCols; col++){
            int value;
            inFile >> value;
            imgArray[row][col] = value;
        }
    }
}

void ImageProcessing::zeroFramed(){
    zeroFramedArray = new int* [numRows + 2];
    for(int i = 0; i < numRows + 2; i++){
        zeroFramedArray[i] = new int [numCols + 2];
    }
    for(int row = 0; row < numRows + 2; row++){
        for(int col = 0; col < numCols + 2; col++){
            zeroFramedArray[row][col] = 0;
        }
    }
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            zeroFramedArray[row][col] = imgArray[row - 1][col - 1];
        }
    }
}

void ImageProcessing::prettyPrint(ofstream& outFile){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(zeroFramedArray[row][col] <= 0){
                outFile << " " << " ";
            }
            else{
               outFile << zeroFramedArray[row][col] << " ";
            }
        }
        outFile << endl;
    }
}

void ImageProcessing::printBinaryImage(ofstream& outFile){
    outFile << numRows << " " << numCols << " " << "0" << " " << "1" << endl;
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(zeroFramedArray[row][col] <= 0){
                outFile << "0" << " ";
            }
            else{
               outFile << "1" << " ";
            }
        }
        outFile << endl;
    }
}

void ImageProcessing::printEqArray(ofstream& outFile){
    outFile << setw(10) << "eqArray:" << endl;
    outFile << setw(10) << "Index:";
    for(int i = 0; i <= newLabel; i++){
        outFile << setw(2) << i << " ";
    }
    outFile << endl;
    outFile << setw(10) << "Component:";
    for(int i = 0; i <= newLabel; i++){
        outFile << setw(2) << eqArray[i] << " ";
    }
}

bool ImageProcessing::arrIsAllZero(int* arr, int arrSize){
    for(int i = 0; i < arrSize; i++){
        if(arr[i] != 0){
            return false;
        }
    }
    return true;
}

bool ImageProcessing::arrContainsElement(int* arr, int arrSize, int element){
    for(int i = 0; i < arrSize; i++){
        if(arr[i] == element){
            return true;
        }
    }
    return false;
}

int ImageProcessing::arrMax(int* arr, int arrSize){
    int maximum = 0;
    for(int i = 0; i < arrSize; i++){
        if(arr[i] > maximum){
            maximum = arr[i];
        }
    }
    return maximum;
}

int ImageProcessing::arrMin(int* arr, int arrSize){
    int minimum = arrMax(arr, arrSize);
    for(int i = 0; i < arrSize; i++){
        if(arr[i] != 0 && arr[i] < minimum){
            minimum = arr[i];
        }
    }
    return minimum;
}

void ImageProcessing::firstPass8Conn(){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(zeroFramedArray[row][col] > 0){
                int* neighbors = new int [4];
                neighbors[0] = zeroFramedArray[row - 1][col - 1]; //a
                neighbors[1] = zeroFramedArray[row - 1][col];     //b
                neighbors[2] = zeroFramedArray[row - 1][col + 1]; //c
                neighbors[3] = zeroFramedArray[row][col - 1];     //d
                if(arrIsAllZero(neighbors, 4)){ //Case 1
                    newLabel++;
                    zeroFramedArray[row][col] = newLabel;
                }
                else if(arrMin(neighbors, 4) == arrMax(neighbors, 4)){ //Case 2
                    zeroFramedArray[row][col] = arrMax(neighbors, 4);
                }
                else if(arrMin(neighbors, 4) != arrMax(neighbors, 4)){ //Case 3
                    zeroFramedArray[row][col] = arrMin(neighbors, 4);
                    updateEqArray(arrMax(neighbors, 4), arrMin(neighbors, 4));
                }
                delete neighbors;
            }
        }
    }
}

void ImageProcessing::secondPass8Conn(){
    for(int row = numRows + 1; row > 0; row--){
        for(int col = numCols + 1; col > 0; col--){
            if(zeroFramedArray[row][col] > 0){
                int* neighbors = new int [4];
                neighbors[0] = zeroFramedArray[row][col + 1];     //e
                neighbors[1] = zeroFramedArray[row + 1][col - 1]; //f
                neighbors[2] = zeroFramedArray[row + 1][col];     //g
                neighbors[3] = zeroFramedArray[row + 1][col + 1]; //h
                if(arrIsAllZero(neighbors, 4)){ //Case 1
                    //do nothing
                }
                else if(arrMin(neighbors, 4) == arrMax(neighbors, 4) && zeroFramedArray[row][col] == arrMax(neighbors, 4)){ //Case 2
                    //do nothing
                }
                else if(arrMin(neighbors, 4) != arrMax(neighbors, 4) ||
                       (arrMin(neighbors, 4) == arrMax(neighbors, 4) && zeroFramedArray[row][col] != arrMax(neighbors, 4))){ //Case 3
                    zeroFramedArray[row][col] = arrMin(neighbors, 4);
                    updateEqArray(arrMax(neighbors, 4), arrMin(neighbors, 4));
                }
                delete neighbors;
            }
        }
    }
}

void ImageProcessing::thirdPass8Conn(){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(zeroFramedArray[row][col] > 0){
                zeroFramedArray[row][col] = eqArray[zeroFramedArray[row][col]];
            }
        }
    }
}

void ImageProcessing::updateEqArray(int index, int label){
    eqArray[index] = label;
}

void ImageProcessing::manageEqArray(){
    for(int i = newLabel; i > 0; i--){
        if(eqArray[i] != i){
            int label = eqArray[i];
            while(eqArray[label] != label){
                label = eqArray[label];
            }
            eqArray[i] = label;
        }
    }
    int numTrueLabels = 1;
    int* labelsUsed = new int [newLabel + 1];
    for(int i = 0; i <= newLabel; i++){
        labelsUsed[i] = 0;
    }
    for(int i = 1; i <= newLabel; i++){
        if(!arrContainsElement(labelsUsed, numTrueLabels, eqArray[i])){
            labelsUsed[numTrueLabels++] = eqArray[i];
        }
    }
    for(int i = 1; i <= newLabel; i++){
        for(int j = 1; j <= numTrueLabels; j++){
            if(eqArray[i] == labelsUsed[j]){
                eqArray[i] = j;
            }
        }
    }
    delete labelsUsed;
}
