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

#pragma once

// Defines user levels compatible with this dll
enum UserLevel { LoggedOut = 0, Basic = 1, Intermediate = 2, Admin = 3};

#define PYTHONAPI extern "C" __declspec(dllexport)	// Exposes the function for consumption via Python C-Types

// Returns TRUE if input >=18 and <=65
PYTHONAPI bool ValidateAge(int age);

// returns TRUE if the input is <18
PYTHONAPI bool IsUnderAge(int age);

// returns TRUE if the input is >65
PYTHONAPI bool IsOverAge(int age);

// returns TRUE if the file was written to, otherwise false
PYTHONAPI bool WriteToFile(const char* filePath, const char* input, UserLevel userLevel);

// Gets the last error message from the dll
PYTHONAPI const char* GetLastErrorMessage();

// Clears the last error message, freeing the memory used to store it by the dll
PYTHONAPI void ClearLastErrorMessage();
