from sys import argv
from csv import reader
from cs50 import SQL

# main function


def main():
    # Check arguments
    if len(argv) < 2:
        print("Please use like: import.py file-to-import.csv")
        exit()

    processData(argv[1])


# process csv to sql
# @param str file - path to file


def processData(file):
    # open database
    db = SQL("sqlite:///students.db")
    # read csv
    with open(file, newline='') as dataFile:
        data = reader(dataFile)
        # skip first row
        next(data)
        # loop over entries
        for entry in data:
            # we fill the void after in ternary operation
            name = entry[0].split()
            # fill the database
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       name[0],
                       name[1] if len(name) == 3 else None,
                       name[2] if len(name) == 3 else name[1],
                       entry[1],
                       entry[2],
                       )


main()