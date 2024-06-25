import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Wrong command line argument")

    # TODO: Read database file into a variable
    d_file = []
    with open(sys.argv[1]) as csvfile:

        # create list of dict database
        reader = csv.DictReader(csvfile)
        for row in reader:
            d_file.append(row)

        # print(d_file)

    with open(sys.argv[1]) as csvfile:
        # creates list of SRTs
        SRTs = csv.DictReader(csvfile)
        SRTs = (SRTs.fieldnames)

        # remove 'name' from SRTs
        SRTs.remove('name')
        # print(SRTs)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as textfile:
        dna = textfile.read()
        # print("DNA: " + dna)

    # TODO: Find longest match of each STR in DNA sequence
    # and create dict with this numbres
    result = {}

    for i in SRTs:
        x = longest_match(dna, i)
        result[i] = str(x)

    # print(result)

    # TODO: Check database for matching profiles
    # Initialize a flag to track if a match is found
    found = False
    for row in d_file:
        name = row['name']
        del row['name']
        # Compare row with result
        if row == result:
            print(name)
            found = True
    if not found:
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
