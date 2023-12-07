#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>

// productions (a) ,  attractions (b)
// i = productions ,j = attractions
namespace fs = std::filesystem;
using namespace std;

void Applying() {
    cout << "Applying correction";
    for (int i = 0; i < 10; ++i) {
        cout << ".";
        cout.flush();  
        this_thread::sleep_for(chrono::milliseconds(10));  
    }
    cout << endl;
}
void printGreen(const string& text) {
    cout << "\033[1;32m" << text << "\033[0m";
}
void printRed(const string& text) {
    cout << "\033[1;31m" << text << "\033[0m";
}
double calculateError(double oldVal, double newVal) {
    return fabs((newVal - oldVal) / oldVal);
}


void calculateFactors(const vector<vector<double>>& matrix, const vector<double>& productionSums, const vector<double>& attractionSums,
    vector<double>& productionFactors, vector<double>& attractionFactors, double tolerance) {
    int size = matrix.size();

    productionFactors.assign(size, 1.0);
    attractionFactors.assign(size, 1.0);

    int maxIterations = 1000;  
    int currentIteration = 1;

    double productionError = 0.0;
    double attractionError = 0.0;
    vector<double> oldattractionFactors(size, 1.0);
    vector<double> oldproductionFactors(size, 1.0);



    while (currentIteration < maxIterations) {
        vector<double> calculatedproductionSums(size, 0);
        vector<double> calculatedattractionSums(size, 0);

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                calculatedproductionSums[i] += matrix[i][j] * attractionFactors[j];
            }
        }

        for (int i = 0; i < size; ++i) {
            productionFactors[i] = (calculatedproductionSums[i] != 0) ? (productionSums[i] / calculatedproductionSums[i]) : 1.0;  
        }

        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                calculatedattractionSums[j] += matrix[i][j] * productionFactors[i];
            }
        }

        for (int j = 0; j < size; ++j) {
            attractionFactors[j] = (calculatedattractionSums[j] != 0) ? (attractionSums[j] / calculatedattractionSums[j]) : 1.0;  

        }

        for (int j = 0; j < size; ++j) {
            double currentError = calculateError(oldattractionFactors[j], attractionFactors[j]);
            attractionError = currentError;
        }
        for (int i = 1; i < size; ++i) {
            double currentError = calculateError(oldproductionFactors[i], productionFactors[i]);
            productionError = currentError;
        }

        if (attractionError <= tolerance && productionError <= tolerance) {
            break;
        }
        oldattractionFactors = attractionFactors;
        oldproductionFactors = productionFactors;

        ++currentIteration;
    }

    if (currentIteration == maxIterations) {
        printRed("Maximum iterations reached.The result may not be accurate.");

        cout << endl;
    }

    printGreen("production Factors (a):"), cout << endl;

    for (int i = 0; i < productionFactors.size();++i) {
        cout << "a" << i+1 << ": " << productionFactors[i] << endl;
    }
    cout << endl;

    
    printGreen("attraction Factors (b):"), cout << endl;
    for (int j = 0; j< attractionFactors.size();++j) {
        cout << "b" << j + 1 << ": " << attractionFactors[j] << endl;
    }

    cout << endl;
    
    printGreen("Production growth factor Error (a%): "), cout << productionError * 100 << "%" << endl;
    printGreen("Attraction growth factor error (b%): "), cout << attractionError * 100 << "%" << endl;
    cout << endl;


    if (currentIteration == maxIterations) {
        printRed("Number of trails: "), cout << currentIteration << endl; 
        cout << endl;
    }
    else {
        printGreen("Number of trails: "), cout << ++currentIteration << endl;
        cout << endl;
    }

}


vector<vector<double>> calculateFutureMatrix(const vector<vector<double>>& matrix, const vector<double>& productionSums, const vector<double>& attractionSums,double error) {
    int size = matrix.size();
    vector<double> productionFactors(size, 0.0);
    vector<double> attractionFactors(size, 0.0);

    calculateFactors(matrix, productionSums, attractionSums, productionFactors, attractionFactors, error);

    vector<vector<double>> resultMatrix(size, vector<double>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            resultMatrix[i][j] = matrix[i][j] * productionFactors[i] * attractionFactors[j];
        }
    }


    printGreen("Factors Matrix (a*b): "), cout << endl;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << productionFactors[i] * attractionFactors[j] << "    ";
        }
        cout << endl;
    }
    cout << endl;


    return resultMatrix;
}

vector<vector<double>> loadinputs(int& size, vector<double>& productionSums, vector<double>& attractionSums) {
    vector<vector<double>> loadedMatrix;

    string filePath = fs::current_path().string() + "\\log.txt"; 

    ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        inputFile >> size; 

        for (int i = 0; i < size; ++i) {
            vector<double> rowData;
            for (int j = 0; j < size; ++j) {
                double value;
                inputFile >> value;
                rowData.push_back(value);
            }
            loadedMatrix.push_back(rowData);
        }

        productionSums.resize(size);
        for (int i = 0; i < size; ++i) {
            inputFile >> productionSums[i];
        }

        attractionSums.resize(size);
        for (int i = 0; i < size; ++i) {
            inputFile >> attractionSums[i];
        }

        inputFile.close();
    }

    return loadedMatrix;
}

void saveInputs(const vector<vector<double>>& matrix, int size, const vector<double>& productionSums, const vector<double>& attractionSums) {
    string filePath = fs::current_path().string() + "\\log.txt"; 

    ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << size << endl; 

        for (const auto& row : matrix) {
            for (double value : row) {
                outputFile << value << " ";
            }
            outputFile << endl;
        }

        for (double value : productionSums) {
            outputFile << value << " ";
        }
        outputFile << endl;

        for (double value : attractionSums) {
            outputFile << value << " ";
        }
        outputFile << endl;

        outputFile.close();
    }
}

int main() {
    int size;
    char choice;
    double error;
    while (true) {

        cout << "MADE BY: Anas Akram" << endl;
        cout << "ID: 9180327" << endl;
        cout << "SEC.: 2" << endl;
        cout << "B.N.: 28" << endl;
        cout << "--------------------------------" << endl;

        cout << "Hint:You can use the log.txt to add Ur own example." << endl;
        cout << "Do you want to load log file? (y/n): ";
        cin >> choice;

        vector<vector<double>> matrix;
        vector<double> productionSums;
        vector<double> attractionSums;

        if (choice == 'y' || choice == 'Y') {
            matrix = loadinputs(size, productionSums, attractionSums);
            cout << "size: " << size << endl;

            cout << "matrix:" << endl;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    cout << matrix[i][j] << "    ";
                }
                cout << endl;
            }
            cout << endl;

            cout << "sum of production:" << endl;
            for (int i = 0; i < size; ++i) {
                cout << productionSums[i] << endl;
            }
            cout << endl;

            cout << "sum of attraction:" << endl;
            for (int i = 0; i < size; ++i) {
                cout << attractionSums[i] << "      ";
            }
            cout << endl << endl;


        }

        else {

            cout << "Enter the size of the matrix: ";
            cin >> size;

            matrix = vector<vector<double>>(size, vector<double>(size));

            cout << "Enter the matrix elements:" << endl;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    cout << "Enter element at (" << i + 1 << ", " << j + 1 << "): ";
                    cin >> matrix[i][j];
                }
            }

            productionSums.resize(size);

            cout << "Enter the sum of production:" << endl;
            for (int i = 0; i < size; ++i) {
                cout << "Enter sum for production " << i + 1 << ": ";
                cin >> productionSums[i];
            }

            attractionSums.resize(size);

            cout << "Enter the sum of attraction:" << endl;
            for (int i = 0; i < size; ++i) {
                cout << "Enter sum for attraction " << i + 1 << ": ";
                cin >> attractionSums[i];
            }

            saveInputs(matrix, size, productionSums, attractionSums);
        }

        //Check balance
        cout << "Checking production & attraction balancing:" << endl;

        double sumOfproductions = accumulate(productionSums.begin(), productionSums.end(), 0);
        double sumOfattractions = accumulate(attractionSums.begin(), attractionSums.end(), 0);

        if (sumOfproductions != sumOfattractions) {
            cerr << "Productions and attractions are not balance. Do you want to correct it? (y/n)" << endl;
            cin >> choice;

            if (choice == 'n' || choice == 'N') {

                cout << "no correction will applied,The result may not be accurate." << endl;
            }

            else {

                Applying();
                cout << endl;

                double correctionFactor = static_cast<double>(sumOfproductions) / sumOfattractions;

                for (int i = 0; i < size; ++i) {
                    attractionSums[i] *= correctionFactor;
                }

                printGreen("Correction applied.");
                cout << endl<<endl;

                printGreen("Corrected values: ");
                cout << endl;
                for (int i = 0; i < size; ++i) {
                    cout << attractionSums[i] << "      ";
                }
                cout << endl<<endl;



            }
        }

        cout << "Acceptable error % should be: ";
        cin >> error, cout << endl;
        error = error / 100;

        vector<vector<double>> resultMatrix = calculateFutureMatrix(matrix, productionSums, attractionSums, error);

        printGreen("The result Matrix:"), cout << endl;

        for (const auto& row : resultMatrix) {
            for (double element : row) {
                cout << element << "    ";
            }
            cout << endl;
        }

        cout << "Press 'E' to exit or 'R' to reload the program...  ";
        cin >> choice;

        if (choice == 'e' || choice == 'E') {
            cout << "Exiting the program..." << endl;
            break;
        }
        else if (choice == 'r' || choice == 'R') {
            cout << "Reloading the program..." << endl;
            continue;
        }
        else {
            cout << "Invalid choice. Please enter 'e' to exit or 'r' to reload." << endl;
        }
    }
    
    return 0;
}
