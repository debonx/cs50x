from sys import argv
from cs50 import SQL


def main():
    # Check arguments
    if len(argv) < 2:
        print("Please use like: roaster.py Gryffindor")
        exit()

    query(argv[1])


def query(house):
    # open database
    db = SQL("sqlite:///students.db")

    # prevent some wrong format
    students = db.execute("SELECT * FROM students WHERE house = (?) ORDER BY last", house.lower().capitalize())

    # print names
    for student in students:
        if student['middle']:
            print("{0} {1} {2}, born {3}".format(
                student['first'],
                student['middle'],
                student['last'],
                student['birth'],
            ))
        else:
            print("{0} {1}, born {2}".format(
                student['first'],
                student['last'],
                student['birth'],
            ))


main()