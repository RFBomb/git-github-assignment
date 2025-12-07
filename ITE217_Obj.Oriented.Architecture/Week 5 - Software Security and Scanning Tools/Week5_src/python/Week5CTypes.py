"""
RBrenckman - COSC - ITE217 - Week 5
11-25-2025
This python file wraps the C++ dll to import its functions and use as needed by the FLASK server

"""

import ctypes
from ctypes import c_int, c_char_p, c_bool

class Week5Dll:

    class UserLevel:
        LoggedOut = 0
        Basic = 1
        Intermediate = 2
        Admin = 3

    def __init__(self, dll_path: str):
        self._dll = ctypes.CDLL(dll_path)

        # --- Setup function prototypes ---
        self._dll.ValidateAge.argtypes = [c_int]
        self._dll.ValidateAge.restype = c_bool

        self._dll.IsUnderAge.argtypes = [c_int]
        self._dll.IsUnderAge.restype = c_bool

        self._dll.IsOverAge.argtypes = [c_int]
        self._dll.IsOverAge.restype = c_bool

        self._dll.WriteToFile.argtypes = [c_char_p, c_char_p, c_int]
        self._dll.WriteToFile.restype = c_bool

        self._dll.GetLastErrorMessage.argtypes = []
        self._dll.GetLastErrorMessage.restype = c_char_p

        self._dll.ClearLastErrorMessage.argtypes = []
        self._dll.ClearLastErrorMessage.restype = None

    # --- Wrapper methods ---
    def validate_age(self, age: int) -> bool:
        if not isinstance(age, int):
            raise "age is not an integer!"
        else:
            return self._dll.ValidateAge(age)

    def is_under_age(self, age: int) -> bool:
        if not isinstance(age, int):
            raise "age is not an integer!"
        else:
            return self._dll.IsUnderAge(age)

    def is_over_age(self, age: int) -> bool:
        if not isinstance(age, int):
            raise "age is not an integer!"
        else:
            return self._dll.IsOverAge(age)

    def write_to_file(self, file_path: str, content: str, user_level: int) -> bool:
        """Writes to file. Returns True on success, False on failure. Use get_last_error() to inspect errors."""
        success = self._dll.WriteToFile(file_path.encode("utf-8"),
                                        content.encode("utf-8"),
                                        user_level)
        return success

    def get_last_error(self) -> str | None:
        err_ptr = self._dll.GetLastErrorMessage()
        if not err_ptr:
            return None
        return err_ptr.decode("utf-8")

    def clear_last_error(self):
        self._dll.ClearLastErrorMessage()

    def is_valid_userlevel(self, level:int)-> bool:
        if not isinstance(level, int):
            raise "level is not an integer!"
        elif (level >  Week5Dll.UserLevel.Admin or level < 0):
            #raise "level is outside of enumerated values"
            return False
        return True;
