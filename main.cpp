#include <iostream>
#include "fibonacciwrapper.h"

using namespace std;

int main() {
    cout << "Select the cache type:" << endl;
    cout << "1 - LRU Cache" << endl;
    cout << "2 - LFU Cache" << endl;

    int choice;
    cin >> choice;

    try {
        FibonacciWrapper calculator(choice);

        cout << "Select the command:" << endl;
        cout << "1 - calculation" << endl;
        cout << "2 - exit" << endl;

        string command;
        while (true) {
            cout << "\n> ";
            cin >> command;

            if (command == "1") {
                int n;
                cin >> n;

                try {
                    int result = calculator.calculate(n);
                    cout << "Result = " << result << endl;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
            else if (command == "2") {
                break;
            }
            else {
                cout << "the command not found" << endl;
            }
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}