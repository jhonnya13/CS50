import re

text = input("Text: ")
letters = (len(re.findall('[a-zA-Z]', text)))
words = 1 + (len(re.findall(' ', text)))
sentences = (len(re.findall('[!.?]', text)))

L = (letters / words) * 100.0
S = (sentences / words) * 100.0
index = 0.0588 * L - 0.296 * S - 15.8
grade = round(index)

if grade > 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {grade}")
