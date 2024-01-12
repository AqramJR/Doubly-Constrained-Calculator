#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>
#include <thread>
#include <fstream>
#include <conio.h>
#include <filesystem>

// productions (a) ,  attractions (b)
// i = productions ,j = attractions
namespace fs = std::filesystem;
using namespace std;

int generateRandomNumber() {
    return rand() % 9901 + 100; // Generates a random number between 100 and 10000
}

void Type(const string& text, int delay) {
    for (char c : text) {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << endl;
}

void Applying(int x, string c) {
    Type(c, 10);
    cout << endl << "[";
    const int totalWidth = 100;

    for (int i = 0; i <= totalWidth; ++i) {
        float progress = static_cast<float>(i) / totalWidth;
        int barWidth = static_cast<int>(progress * totalWidth);

        cout << "\r[";

        for (int j = 0; j < barWidth; ++j) {
            cout << "=";
        }

        for (int j = barWidth; j < totalWidth; ++j) {
            cout << ".";
        }

        cout << "] " << static_cast<int>(progress * 100) << "%";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(x));
    }

    cout << "]" << endl;
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

void saveOutputs(const vector<vector<double>>& matrix) {
    string filePath = fs::current_path().string() + "\\outputs.txt";

    ofstream outputFile(filePath);
    if (outputFile.is_open()) {

        for (const auto& row : matrix) {
            for (double value : row) {
                outputFile << value << " ";
            }
            outputFile << endl;
        }
        outputFile.close();
    }
}


int main() {
    int size;
    char choice;
    double error;
    vector<vector<double>> matrix;
    vector<double> productionSums;
    vector<double> attractionSums;

    while (true) {
        string filePath = fs::current_path().string() + "\\log.txt";

        Type("MADE BY : Anas Akram", 10);
        Type("ID: 9180327", 10);
        Type("SEC.: 2", 10);
        Type("B.N.: 28", 10);

        if (fs::exists(filePath)) {
            cout << "Log file was found!" << endl;
            cout << "Hint:You can use the log.txt to add Ur own example." << endl;
            cout << "Do you want to load log file? (y/n): ";

            choice = _getch();
            cout << choice << endl;


            if (choice == 'y' || choice == 'Y') {
                Applying(5, "Loading");
                cout << endl;
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
        
            else if (choice == 'n' || choice == 'N') {

                cout << "Do You want to generate a random matrix? (y/n): ";
                choice = _getch();
                cout << endl;

                if (choice == 'n' || choice == 'N') {

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

                else if (choice == 'y' || choice == 'Y') {

                    cout << "Enter the size of the matrix: ";
                    cin >> size;

                    matrix = vector<vector<double>>(size, vector<double>(size));
                    srand(time(0));

                    for (int i = 0; i < size; ++i) {
                        for (int j = 0; j < size; ++j) {
                            matrix[i][j] = generateRandomNumber();
                        }
                    }

                    productionSums.resize(size);
                    for (int i = 0; i < size; ++i) {
                        productionSums[i] = generateRandomNumber();
                    }

                    attractionSums.resize(size);
                    for (int i = 0; i < size; ++i) {
                        attractionSums[i] = generateRandomNumber();
                    }
                    saveInputs(matrix, size, productionSums, attractionSums);

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
            }

        }
        else {
            printRed("Log file was NOT found!");
            cout << endl << "Do You want to generate a random matrix? (y/n): ";
            choice = _getch();
            cout << endl;

            if (choice == 'n' || choice == 'N') {

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

            else if (choice == 'y' || choice == 'Y') {

                cout << "Enter the size of the matrix: ";
                cin >> size;

                matrix = vector<vector<double>>(size, vector<double>(size));
                srand(time(0));

                for (int i = 0; i < size; ++i) {
                    for (int j = 0; j < size; ++j) {
                        matrix[i][j] = generateRandomNumber();
                    }
                }

                productionSums.resize(size);
                for (int i = 0; i < size; ++i) {
                    productionSums[i] = generateRandomNumber();
                }

                attractionSums.resize(size);
                for (int i = 0; i < size; ++i) {
                    attractionSums[i] = generateRandomNumber();
                }
                saveInputs(matrix, size, productionSums, attractionSums);

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

        }


        //Check balance
        Type("Checking production & attraction balancing...........", 10);
        cout<< endl;

        double sumOfproductions = accumulate(productionSums.begin(), productionSums.end(), 0);
        double sumOfattractions = accumulate(attractionSums.begin(), attractionSums.end(), 0);

        if (sumOfproductions != sumOfattractions) {
            printRed("Productions and attractions are not balance.");
            cout << endl << "Do you want to correct it? (y/n): ";
            choice = _getch();
            cout << choice << endl;


            if (choice == 'n' || choice == 'N') {

                cout << "no correction will applied,The result may not be accurate." << endl;
            }

            else {

                Applying(10, "Applying");
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

        else if (sumOfproductions = sumOfattractions) {
            printGreen("production & attraction are balanced");
        }

        else {
            printRed("Error");
            cout << endl;
            Type("Please Enter a valid naumbers", 10);
            cout << endl;
            continue;
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
        cout << "Do Want to save the result Matrix? (y/n): ";
        choice = _getch();
        cout << choice << endl;

        if (choice == 'y' || choice == 'Y')
        {
            saveOutputs(resultMatrix);
            Applying(5, "Saving");
            Type("The result Matrix saved in 'output.txt' successfully! ", 20);
            cout << endl;

        }

        
        

        cout << "Press 'E' to exit or 'R' to reload the program...  ";
        choice = _getch();
        cout << choice << endl;
        
        if (choice == 'e' || choice == 'E') {
            Type("Exiting the program...", 15);
            break;
        }
        else if (choice == 'r' || choice == 'R') {
            Applying(5, "Reloading the program");
            system("cls");

            continue;

        }
        else {
            cout << "Invalid choice. Please enter 'e' to exit or 'r' to reload." << endl;
        }
    }
    
    return 0;
}
