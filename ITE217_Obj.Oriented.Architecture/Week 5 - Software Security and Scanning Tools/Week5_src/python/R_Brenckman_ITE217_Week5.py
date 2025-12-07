#
# Robert Brenckman
# COSC ITE-217 - Week 5
# 11-25-2025
#
# Assignment : 
# - Validate a user’s email input.
#       - This is accomplished by regex validation
# - Prevent SQL injection attacks when interacting with a database
#       - This is accomplished by use of parametized queries (instead of concatenated queries)
# - Implement a basic token-based authentication system
#       - TO DO : use flask, create an end point, log in?
#       - Can this be used to create a simple hmtl page with a login button?
#       - Run the web page on script start. Provide some buttons for cookie management.
#

from Week5CTypes import *   # C++ dll portion o f the assignment
import re                   # regex
import sqlite3              # db storage
import hashlib              # required for hashing consistently across runs -- use bcrypt for a better approach and if salting
import uuid                 # generates UUID for tokens
from datetime import datetime, timezone, timedelta
from flask import Flask, jsonify, request # api
from typing import Dict

# Setup the FLASK app
app = Flask(__name__)

# this would typically be an environment variable or in a config file
DBPath = ".\\RBrenckman_ITE217_Week5_py.db"

# DB Structure
# users
#   id - integer (autonumber)
#   email - text unique
#   passHash - password hash
#   age - integer
#   userlevel - integer
# tokens
#   tokenID - text pkey
#   userID - users.id
#   expDate - integer (unix time)
#   

def getDB():
    return sqlite3.connect(DBPath)

# -------------------------
# Data Validation
# -------------------------

# Load the DLL
dll:Week5Dll = Week5Dll(".\\ITE217_Week5_SecureCodingCPP.dll")

# Validates a user's email address
# If successful, returns (True, sanitized_input)
# If bad input, returns (False, error_descrip)
# this pattern allows consumer to throw the error or ignore it, instead of raising the error immediately
def EmailValidation(input:str)->tuple:
    if not (isinstance(input, str)):
        return (False, "" + input + " is not a string")
    
    input = input.strip() # trim
    if (len(input) < 5):
        return (False, "Email has a minimum length of 5") # _@_._
    
    email_regex = r'^\S+@\S+\.\S+$' #match non-white-space chars
    if not re.match(email_regex, input):
        return (False,"Invalid email format")
    
    return (True, input)

# Checks password requirements
# returns (False, error_descrip) or (True, hash)
def PasswordValidation(passwd:str) -> tuple:
    if not (isinstance(passwd, str)):
        return (False, "" + passwd + " is not a string")
    passwd = passwd.strip()
    if (len(passwd) < 5): 
        return (False, "Password has minimum length of 5 characters")
    h = hashlib.md5(passwd.encode()).hexdigest() #encoding is required to work with hashlib
    return (True, h)


# -------------------------
# Helpers
# -------------------------

def CreateErrResponse(errMsg:str):
    return jsonify({"success" : False, "message": errMsg})

# -------------------------
# Routes
# -------------------------

@app.route('/')
def Root():
    return "Hello World"

@app.route('/register', methods=['PUT'])
def Register():
    db:sqlite3.Connection = None
    try:
        data:Dict[str, any] = request.json or {}
        username = data.get("email", "")
        password = data.get("pass", "")
        age = data.get("age", 0)
        userLevel = data.get("level", 0)

        # validate data
        if dll.validate_age(age) == False:
            return CreateErrResponse("user age must be between 18-65")
        
        if (dll.is_valid_userlevel(userLevel) == False):
            return CreateErrResponse("user level is outside of valid range")

        (success, username) = EmailValidation(username)
        if not (success):
            return CreateErrResponse(username)

        (success, password) = PasswordValidation(password)
        if not (success):
            return CreateErrResponse(password)

        # check if username exists
        db = getDB()
        cursor = db.cursor()
        cursor.execute("SELECT 1 FROM Users WHERE email = ? LIMIT 1;", (username,))
        if (cursor.fetchone() is None):
            # update the db
            cursor.execute("INSERT INTO Users (email, passHash, age, userlevel) VALUES (?,?, ?, ?);", (username, password, age, userLevel))
            db.commit()
            return jsonify({"success" : True})
        else:
            return CreateErrResponse("Username already exists in system")
        pass

    except Exception as e:
        return CreateErrResponse(str(e))
    finally:
        if not (db is None): db.close()
    pass


@app.route('/login', methods=['POST'])
def Login():
    db:sqlite3.Connection = None
    try:
        data:Dict[str, any] = request.json or {}
        username = data.get("email", None)
        password = data.get("pass", None)
        supplied_token  = data.get("token", None)
        
        curTime = datetime.now(timezone.utc)
        expDate = int((curTime + timedelta(hours=72)).timestamp())  # Unix expiration timestamp

        loginOK: bool = False
        token: str = None
        userID: int = None

        db = getDB()
        cursor = db.cursor()
        
        # ------- Check Suppplied token ---------
        # prioritize the token if one was supplied
        if supplied_token is not None:
            # check db token
            cursor.execute("SELECT userID, expDate FROM Tokens WHERE id = ? LIMIT 1;", (supplied_token,))
            row = cursor.fetchone()
            if (row is not None):
                db_userID, db_expDate = row #extract the values from the row
                if db_expDate >= curTime.timestamp():
                    loginOK = True
                    token = supplied_token
                    userID = db_userID
                    cursor.execute("UPDATE Tokens SET expDate = ? WHERE id = ?;", (expDate, token))
                    db.commit()
                pass
            pass # end of supplied_token


        # ------- Check Username and Password  ---------
        if not loginOK:
            if (password is None or username is None or username == "" or password == ""):
                return CreateErrResponse("Incorrect username or password")

            (eSuccess, username) = EmailValidation(username)
            (psuccess, pHash) = PasswordValidation(password) 
            if not eSuccess or not psuccess:
                return CreateErrResponse(username)
            
            # to-do : Update this query to also select the user ID
            cursor.execute("SELECT id, passHash FROM Users WHERE email = ?", (username,))
            row = cursor.fetchone()
            if (row is None):
                return CreateErrResponse("Incorrect username or password")
            
            db_userID, db_passHash = row
            if pHash != db_passHash:
                return CreateErrResponse("Incorrect username or password")

            # username/password combo is valid
            userID = db_userID
            token = str(uuid.uuid4())
            cursor.execute(
                """
                INSERT INTO Tokens (id, userID, expDate)
                VALUES (?, ?, ?)
                ON CONFLICT(id) DO UPDATE SET expDate=excluded.expDate;
                """,
                (token, userID, expDate)
            )
            db.commit()
            loginOK = True
            pass # end of username/password

        if loginOK:
            return jsonify({"success": True, "token": token, "userID": userID})
        
        return CreateErrResponse("Login Failed")
        #end of try
    except Exception as e:
        return CreateErrResponse(str(e))
    finally:
        if db is not None: db.close()
        pass
    pass


@app.route('/write', methods=['POST'])
def Writer():
    db:sqlite3.Connection = None
    try:
        data:Dict[str, any] = request.json or {}
        supplied_token  = data.get("token", None)
        userMessage = data.get("submission", None)
        if (userMessage is None or supplied_token is None):
           return CreateErrResponse("token or message is invalid")

        loginOK = False
        userID = None

        db = getDB()
        cursor = db.cursor()

        # ------- Check Suppplied token ---------
        if supplied_token is not None:
            # check db token
            cursor.execute("SELECT userID, expDate FROM Tokens WHERE id = ? LIMIT 1;", (supplied_token,))
            row = cursor.fetchone()
            if (row is not None):
                db_userID, db_expDate = row #extract the values from the row
                if db_expDate > datetime.now(timezone.utc).timestamp():
                    loginOK = True
                    userID = db_userID
                    pass
                pass
            pass # end of supplied_token

        # typically this would be 'If not loginOK' and return an error response
        # but the assignment requires that the C++ prevent action based on user roles, so that is what is implemented instead.
        userLevel = 0
        if loginOK:
            cursor.execute("SELECT userLevel FROM Users where id = ?", (userID,))
            row = cursor.fetchone()
            if (row is not None):
                userLevel = row[0]
        
        if not dll.write_to_file("UserSubmissions.txt", userMessage, userLevel):
            return CreateErrResponse(dll.get_last_error())
        elif userLevel == 0:
            return CreateErrResponse("C++ DLL DID NOT PREVENT THE OPERATION !!!!")
        else:
            return jsonify({"success": True})
        pass
    except Exception as e:
        return CreateErrResponse(str(e))
    finally:
        if (db is not None): db.close()
        pass
    pass
        

# -----------------------------------------------------------------------------
#           Script Entry
# -----------------------------------------------------------------------------
if __name__ == "__main__":
    # Constructs the base DB this file communicates with
    # https://docs.python.org/3/library/sqlite3.html
    try:
        con =  getDB()
        cursor = con.cursor()
        cursor.execute("CREATE TABLE IF NOT EXISTS Users(id INTEGER PRIMARY KEY, email TEXT UNIQUE, passHash TEXT, age INTEGER, userlevel INTEGER)")
        cursor.execute("CREATE TABLE IF NOT EXISTS Tokens(id TEXT PRIMARY KEY, userID INTEGER, expDate INTEGER)")
        con.commit() # save changes to disk
    except Exception as e:
        print("ERROR DURING DB INIT : ", str(e))
        raise e
    finally:
        cursor.close()
        con.close()
    pass
    app.run()
    
