from csv import reader, DictReader
from sys import argv
import re


def main():
    if len(argv) < 3:
        return print("Usage: python dna.py data.csv sequence.txt")

    dna = read_dna(argv[2])
    types = read_types(argv[1])
    sequences_found = find_sequences(types, dna)
    match_sequences(argv[1], sequences_found)


def read_dna(file):
    # read the input dna file first line as string
    with open(file) as dnafile:
        return dnafile.readline()


def read_types(file):
    # extract types
    with open(file) as database:
        types = database.readline().rstrip().split(',')[1:]

    return types


def find_sequences(types, dna):
    # initiating sequences dictionary
    found = {}

    # Count sequences
    for t in types:
        groups = re.findall(rf'(?:{t})+', dna)
        if not groups:
            largest = 0
            found[t] = 0
        else:
            largest = max(groups, key=len)
            found[t] = len(largest) // len(t)

    return found


def match_sequences(database, found):
    # read as dictionary
    with open(database, newline='') as peoplefile:
        people = DictReader(peoplefile)

        # iterate over people and dna to compare occurrencies
        for person in people:
            count = 0
            for dnaType in found:
                if int(person[dnaType]) == int(found[dnaType]):
                    count += 1

            if count == len(found):
                print(person['name'])
                exit()

        print("No match")


main()