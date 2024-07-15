#include "BinaryInt.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;
struct args
{
    int numInputs;
    vector<BinaryInt *> inputsBinary;
    vector<int> inputs;
    BinaryInt key = BinaryInt(0);
    string fullLine;
    int order;
    vector<BinaryInt *> messages;
    vector<BinaryInt *> remainders;

    args(string line)
    {
        fullLine = line;
        stringstream ss(line);
        string temp;
        // first int is numInputs
        ss >> numInputs;
        // read the inputs
        for (int i = 0; i < numInputs; i++)
        {
            ss >> temp;

            inputsBinary.push_back(new BinaryInt(stoi(temp)));
            inputs.push_back(stoi(temp));
        }

        // read the code
        ss >> temp;
        key = BinaryInt(temp);

        // get the order
        getOrder();
    }
    void printInputs()
    {
        cout << "inputs: ";
        for (int i = 0; i < numInputs; i++)
        {
            cout << inputs[i] << " ";
        }
        cout << endl;
    }
    void getOrder()
    {
        int keyLength = key.binary.size();
        for (int i = 0; i < keyLength; i++)
        {
            if (key.binary[i] == 1)
            {
                order = keyLength - i - 1;

                return;
            }
        }
    }
    void print()
    {
        cout << "numInputs: " << numInputs << endl;
        cout << "inputs: ";
        for (int i = 0; i < numInputs; i++)
        {
            cout << inputs[i] << " ";
        }
        cout << endl;
        cout << "inputsBinary: ";
        for (int i = 0; i < numInputs; i++)
        {
            inputsBinary[i]->print();
        }
        cout << endl;
        cout << "key: ";
        key.print();
        cout << endl;
        cout << "order: " << order << endl;
    }
    string getStr()
    {
        string str = "";
        str += "numInputs: " + to_string(numInputs) + "\n";
        str += "inputs: ";
        for (int i = 0; i < numInputs; i++)
        {
            str += to_string(inputs[i]) + " ";
        }
        str += "\n";
        str += "inputsBinary: ";
        for (int i = 0; i < numInputs; i++)
        {
            str += inputsBinary[i]->getBinaryString();
        }
        str += "\n";
        str += "key: ";
        str += key.getBinaryString();
        str += "\n";
        str += "order: " + to_string(order) + "\n";
        return str;
    }
};
class message
{
public:
    BinaryInt *key;
    vector<BinaryInt *> inputsBinary;
    int order;
    args *ref;
};
