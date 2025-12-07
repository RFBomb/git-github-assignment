// 
// Robert Brenckman
// COSC - ITE-217 - Week 5
// 11/29/2025
// 
// Header file for a C++ dll to be consumed by a python FLASK project via CTypes
//
// Requirements:
// C++: Validate that a user’s age is an integer within a given range (e.g., 18-65).
// C++: Restrict access to a resource based on user roles.
// C++: Sanitize user input before using it in a file operation
//
// ValidateAge satisfies requirement 1. 
// WriteToFile satisfies requirements 2 and 3:
//		Prevents users without enough privelege to write to the file
//		Sanitizes the input prior to writing to the file
//

#include "pch.h"
#include "ITE217_Week5.h"
#include <fstream>
#include <format>
#include <cstring>
#include <mutex>
#include <chrono>
#include <iomanip>

#pragma region LAST_ERROR

std::string lastErrMessage;
std::mutex lastErrMutex;

const char* GetLastErrorMessage() 
{
    std::lock_guard<std::mutex> lock(lastErrMutex);
    
    if (lastErrMessage.empty())
        return nullptr;

    // protected copy for persistent between calls
    static std::string lastMsg; 
    lastMsg = std::string(lastErrMessage);
    return lastMsg.c_str();
}

void ClearLastErrorMessage() 
{
    std::lock_guard<std::mutex> lock(lastErrMutex); 
    lastErrMessage = std::string(); 
}

static void SetLastError(const std::string& e)
{
    std::lock_guard<std::mutex> lock(lastErrMutex);
    lastErrMessage = e;
}
static void SetLastError(const char* e) { auto s = std::string(e); SetLastError(s); }
static void SetLastError(const std::exception& e) 
{
    if (&e)
    {
        auto s = std::string(e.what()); 
        SetLastError(s);
    }
}


#pragma endregion

bool IsOverAge(int age) { return age > 65; }
bool IsUnderAge(int age) { return age < 18; }
bool ValidateAge(int age) { return age >= 18 && age <= 65; }

static bool IsPathLegal(const std::string& input)
{
    // accounts for standard local paths C:\XYZ\
    // accounts for file://C:\xyz\

    if (input.size() < 3) return false;

    bool fPrefix = input.starts_with("file://");
    int index = 0;
    bool isEmpty = true;

    for (const char& c : input)
    {
        switch (c)
        {
        case '\t':
        case '\n':
        case '<':
        case '>':
        case '"':
        case '|':
        case '?':
        case '*':
            return false;

        case ':':
            if (fPrefix)
            {
                if (index != 4 && index != 8)
                {
                    return false; // allow only index 4 or 8 to accomodate file://C:/
                }
            }
            else if (index != 1)
            {
                return false;
            }
            break;

        case ' ':
            if (isEmpty && index == input.size() - 1)
            {
                return false; // empty string detected
            }
            break;
        default:
            isEmpty = false;
            break;
        }
        index++;
    }
    return !isEmpty;
}

static void Trim(std::string& s)
{
    // Trim leading whitespace
    auto l = s.size();
    int i = 0;
    while (i < l && std::isspace(static_cast<unsigned char>(s[i])))
        i++;
    if (i > 0) s.erase(0, i);

    // Trim trailing whitespace
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
        s.pop_back();
}

static void SanitizePath(std::string& input)
{
    // 1. Remove unwanted prefixes
    if (input.starts_with("file://"))
    {
        input.erase(0, 7);
    }
    
    // 2. Normalize slashes
    for (char& c : input)
    {
        if (c == '\\')
            c = '/';
    }

    // 3. Remove duplicate slashes
    for (size_t i = 0; i + 1 < input.size();)
    {
        if (input[i] == '/' && input[i + 1] == '/')
            input.erase(i, 1);
        else
            ++i;
    }

    // 4. Trim
    Trim(input);
}

static std::string SanitizeInput(const char* input)
{
    std::string s(input);
    Trim(s);
    return s;
}

bool WriteToFile(const char* filePath, const char* input, UserLevel userLevel)
{
    const char* userLev;
    switch (userLevel)
    {
    case UserLevel::Basic:
        userLev = "BASIC";
        break;
    case UserLevel::Intermediate:
        userLev = "INTERMEDIATE";
        break;
    case UserLevel::Admin:
        userLev = "ADMIN";
        break;
    default:
        SetLastError("User does not have access level required to write to the file");
        return false; // prevent writing to the file
    }

    try
    {
        std::string fps(filePath);
        if (!IsPathLegal(fps))
        {
            SetLastError("Provided path is not a legal Windows path");
            return false;
        }

        SanitizePath(fps);
        auto si = SanitizeInput(input);

        std::ofstream f(fps, std::ios::app); // open the output file in APPEND mode
        if (!f)
        {
            auto er = std::format("Could not open file '{}' for writing.", fps);
            SetLastError(er);
            return false;
        }

        // get the date time the ~~hard~~ C++ way
        // https://en.cppreference.com/w/cpp/chrono/system_clock/now
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        std::tm utc;
        gmtime_s(&utc, &t_c); // compilation fails if this conversion is not called because its "more thread safe" even though it is a static method that should only be using local variables.
        
        f << "\n[" << userLev << "] [" << std::put_time(&utc, "%Y-%m-%d %H:%M:%S") << "] " << si;
        f.flush();
        f.close();
    }
    catch (std::exception& e)
    {
        SetLastError(e);
        return false;
    }
    return true;
}