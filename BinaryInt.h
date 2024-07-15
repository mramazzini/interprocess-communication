#include <vector>
#include <iostream>

using namespace std;
class BinaryInt
{
public:
    vector<int> binary;
    BinaryInt()
    {
        binary.push_back(0);
    }
    BinaryInt(int n)
    {

        while (n > 0)
        {
            binary.push_back(n % 2);
            n = n / 2;
        }
        // reverse algorithm because its backwards
        for (int i = 0; i < binary.size() / 2; i++)
        {
            int temp = binary[i];
            binary[i] = binary[binary.size() - i - 1];
            binary[binary.size() - i - 1] = temp;
        }
    }
    BinaryInt(string s)
    {
        // Input a string of 1s and 0s
        for (int i = 0; i < s.size(); i++)
        {
            binary.push_back(s[i] - '0');
        }
    }

    BinaryInt(vector<int> b)
    {
        binary = b;
    }

    void print()
    {
        for (int i = 0; i < binary.size(); i++)
        {
            cout << binary[i];
        }
    }
    string getBinaryString()
    {
        string s = "";
        for (int i = 0; i < binary.size(); i++)
        {
            s += to_string(binary[i]);
        }
        return s;
    }
};