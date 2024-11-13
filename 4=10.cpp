#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

// output random number from 1000 to 9999
// used for testing
int generateRandomNumber() {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> distribution(1000, 9999);

    return distribution(gen);
}

// outputs array of doubles
void outputDoubleArray(const double arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// calculates how many combinations of the current set exists
int combinationsAmount(double numbers[], int size) {
    int combinations = 0;
    do {
        combinations++;
    } while (next_permutation(numbers, numbers + size));
    return combinations;
}

// outputs array of ints
void outputIntArray(const int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// compare arrays
bool compareArrays(const int arr1[], const int arr2[], int size) {
    for (int i = 0; i < size; ++i) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

// makes actions do all checks by incrementing last until its 4
void increaseArray(int arr[], int size) {
    int carry = 1;
    for (int i = size - 1; i >= 0; --i) {
        arr[i] += carry;
        carry = (arr[i] > 4) ? 1 : 0;
        if (carry) {
            arr[i] = 1;
        }
        else {
            break;
        }
    }
}

// performs the basic operation based on the number of the variable(1-4)
double performOperation(double num1, double num2, int operation) {
    if (operation == 1) {
        return num1 + num2;
    }
    else if (operation == 2) {
        return num1 - num2;
    }
    else if (operation == 3) {
        return num1 * num2;
    }
    else if (operation == 4) {
        return num1 / num2;
    }
    return 0;
}

// outputs the basic operation based on the number of the variable(1-4)
void outputOperation(const int actions[], int i)
{
    if (actions[i] == 1) {
        cout << "+";
    }
    if (actions[i] == 2) {
        cout << "-";
    }
    if (actions[i] == 3) {
        cout << "*";
    }
    if (actions[i] == 4) {
        cout << "/";
    }
}

// reset actions array to {1,1,1}
void resetActions(int actions[], int size)
{
    int reset[] = {1, 1, 1};
    for (int i = 0; i < size; ++i) {
        actions[i] = reset[i];
    }
}

// cute output of the result(compicated and based on if's)
void outputResult(double numbers[], int actions[], int size, int tries, int combinations)
{
    if((tries > combinations && tries <= combinations*2) || (tries > combinations*4 && tries <= combinations*5)) {
        cout << "(";
    }

    cout << numbers[0];
    outputOperation(actions, 0);

    if((tries > combinations*2 && tries <= combinations*3) || (tries > combinations*5 && tries <= combinations*6)) {
        cout << "(";
    }

    cout << numbers[1];

    if(tries > combinations && tries <= combinations*2) {
        cout << ")";
    }

    outputOperation(actions, 1);

    if(tries > combinations*3 && tries <= combinations*4) {
        cout << "(";
    }

    cout << numbers[2];

    if((tries > combinations*2 && tries <= combinations*3) || (tries > combinations*4 && tries <= combinations*5)) {
        cout << ")";
    }
    outputOperation(actions, 2);
    cout << numbers[3];

    if((tries > combinations*3 && tries <= combinations*4) || (tries > combinations*5 && tries <= combinations*6)) {
        cout << ")";
    }
}

// converts array of numbers from string to array
void stringToIntArray(const string &str, double arr[], int size)
{
    for (int i = 0; i < size; ++i) {
        arr[i] = str[i] - '0';
    }
}

// main solving algorithm
double tryOrderResult(double numbers[], int actions[], int brackets, int d_brackets) {
    brackets--;
    d_brackets--;
    // copy arrays
    double b_numbers[4];
    for (int i = 0; i < 4; ++i) {
        b_numbers[i] = numbers[i];
    }

    // add bracket property
    int b_actions[4];
        for (int i = 0; i < 3; ++i) {
            b_actions[i] = actions[i];
            if(brackets == i){
                b_actions[i] += 10;         
            }
            if(d_brackets == i){
                b_actions[i] += 10;         
            }
        } 

    int numbersSize = 4;
    int actionsSize = 3;
    for (int i = 0; i < actionsSize; i++) {
        // try for (*, /)
        if (b_actions[i] > 12) {
            b_actions[i] -=10;
            b_numbers[i] = performOperation(b_numbers[i], b_numbers[i+1], b_actions[i]);
            if (i != actionsSize - 1) {
                for (int j = i; j < actionsSize - 1; j++) {
                    b_actions[j] = b_actions[j + 1];
                    b_numbers[j + 1] = b_numbers[j + 2];
                }
            }
            numbersSize--;
            actionsSize--;
            i--;
        }
    }
    for (int i = 0; i < actionsSize; i++) {
        // try for (+, -)
        if (b_actions[i] > 10) {
            b_actions[i] -=10;
            b_numbers[i] = performOperation(b_numbers[i], b_numbers[i+1], b_actions[i]);
            if (i != actionsSize - 1) {
                for (int j = i; j < actionsSize - 1; j++) {
                    b_actions[j] = b_actions[j + 1];
                    b_numbers[j + 1] = b_numbers[j + 2];
                }
            }
            numbersSize--;
            actionsSize--;
            i--;
        }
    }
    for (int i = 0; i < actionsSize; i++) {
        // try for *, /
        if (b_actions[i] == 3 || b_actions[i] == 4) {
            if (b_actions[i] == 3) {
                b_numbers[i] *= b_numbers[i + 1];
            }
            if (b_actions[i] == 4) {
                b_numbers[i] /= b_numbers[i + 1];
            }
            if (i != actionsSize - 1) {
                for (int j = i; j < actionsSize - 1; j++) {
                    b_actions[j] = b_actions[j + 1];
                    b_numbers[j + 1] = b_numbers[j + 2];
                }
            }
            numbersSize--;
            actionsSize--;
            i--;
        }
    }


    // check if only one number is left and return it
    double result = b_numbers[0];
    if (result == 10 && actionsSize == 0) {
        return result;
    }

    // perform plus'es and minus'es
    for (int i = 0; i < actionsSize; i++) {
        result = performOperation(result, b_numbers[i + 1], b_actions[i]);
    }
    return result;
}

// main
int main(int argc, char *argv[]) {
    // variables
    string input;
    double numbers[4] = {0, 0, 0, 0};
    int actions[3] = {1, 1, 1};
    int finish[3] = {4, 4, 4};

    // get numbers in proper state
    cout << "Numbers(or 0 for random) > ";
    cin >> input;
    if(input == "0") {
        input = to_string(generateRandomNumber());
        cout << input << endl;
        stringToIntArray(input, numbers, 4);
    }
    else{
        stringToIntArray(input, numbers, 4);
    }

    sort(numbers, numbers + 4);

    int combinations = combinationsAmount(numbers, 4);
    double result = -1;
    int tries = 0;

    // main solving algorithm
    // if breaks try writing 7 here
    while (result != 10 && !(tries + 1 > combinations * 6)) {
        tries++;
        while (!compareArrays(actions, finish, 3)) {
            // no brackets
            if (tries <= combinations) {
                result = tryOrderResult(numbers, actions, 0, 0);
                if (result == 10) {
                    break;
                }
                else {
                    increaseArray(actions, 3);
                }
            }
            // mono brackets
            else if (tries <= combinations * 4) {
                int bracketSlot = floor((tries-1)/combinations);
                result = tryOrderResult(numbers, actions, bracketSlot, 0);
                if (result == 10) {
                    break;
                }
                else {
                    increaseArray(actions, 3);
                }
            }
            // double brackets
            else {
                // gives the first or second slot
                int bracketSlot = floor((tries-1-combinations*3)/combinations);
                result = tryOrderResult(numbers, actions, bracketSlot, bracketSlot+1);
                if (result == 10) {
                    break;
                }
                else {
                    increaseArray(actions, 3);
                }
            }
        }
        // try another combination
        if (result != 10) {
            resetActions(actions, 3);
            next_permutation(numbers, numbers + 4);
        }
    }

    cout << "==========" << endl;
    if (tries + 1 > combinations * 6) {
        outputDoubleArray(numbers, 4);
        cout << "Result: No solution!" << endl;
    }
    else {
        outputResult(numbers, actions, 3, tries, combinations);
        cout << " = " << result << endl;
    }

    cout << "Type anything and press enter to close...";
    cin >> result;

    return 0;
}