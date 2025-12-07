// Robert Brenckman
// CSC-2214_003 ( Spring 2025 )
// 3/12/2025
//

#include<string>;
#include<iostream>;
#include "ConsoleHelpers.h"

using namespace std;

 void setColorError() { cerr << ConsoleColor_Red; }
 void setColorNormal() { cerr << ConsoleColor_Normal; }
 void setColorGreen() { cerr << ConsoleColor_Green; }

 void printErr(std::string err)
{
    setColorError();
    cout << err;
    setColorNormal();
}

 void ClearInputBuffer()
{
    cin.clear(); // clear the error
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore entire stream
}

 int getInt(std::string question, int min, int max)
{
    double x;
    std::string response;
    do
    {
        cout << question;
        cin >> x;
        if (cin.good() && x >= min && x <= max)
        {
            return x;
        }
        // not good
        setColorError();
        cerr << "\ninvalid response - please enter a numeric value between " << min << " - " << max << '.' << endl;
        setColorNormal();
        ClearInputBuffer();
    } while (true);
}
