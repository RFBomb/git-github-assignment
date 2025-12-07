from flask import Flask, request, render_template
import status

app = Flask(__name__)

COUNTERS = {}

@app.route("/counters/<name>", methods=["POST"])
def create_counter(name):
    """Creates a counter"""
    app.logger.info(f"Request to create counter: {name}")
    global COUNTERS

    if name in COUNTERS:
        return {"message":f"Counter {name} already exists"}, status.HTTP_409_CONFLICT

    COUNTERS[name] = 0
    return { name: COUNTERS[name] }, status.HTTP_201_CREATED

@app.route("/counters/<name>", methods=["GET"])
def read_counter(name):
    """Gets the value of a counter"""
    app.logger.info(f"Request to get value of counter: {name}")
    global COUNTERS
    if name in COUNTERS:
        return {name:COUNTERS[name]}, status.HTTP_200_OK
    else:
        return {"message":f"Counter {name} already exists"}, status.HTTP_404_NOT_FOUND

@app.route("/counters/<name>", methods=["PUT"])
def update_counter(name): 
    """Updates  value of a counter"""
    app.logger.info(f"Request to update counter: {name}")
    global COUNTERS
    if name in COUNTERS:
        i = COUNTERS[name]
        app.logger.info(f"Original Value : {i}")
        i = i + 1
        COUNTERS[name] = i
        app.logger.info(f"New Value : {i}")
        return {name:COUNTERS[name]}, status.HTTP_200_OK
    else:
        return {"message":f"Counter {name} was not found"}, status.HTTP_404_NOT_FOUND

@app.route("/counters/<name>", methods=["DELETE"])
def delete_counter(name): 
    """Deletes a counter if it exists"""
    app.logger.info(f"Request to delete counter: {name}")
    global COUNTERS
    if name in COUNTERS:
        del COUNTERS[name]
        app.logger.info(f"deleted counter: {name}")
        return {"message":f"Counter {name} was deleted"}, status.HTTP_204_NO_CONTENT
    else:
        app.logger.info(f"counter '{name}' was not found ")
        return {"message":f"Counter {name} was not found"}, status.HTTP_204_NO_CONTENT
