def get_int(prompt):
    while True:
        try:
            x = int(input(prompt))
            if x <= 0:
                print("Can't be negative")
            elif x >= 9:
                print("Choose from 1 to 8")
            else:
                return x
        except ValueError:
            print("Not an integer")

h = get_int("Height: ")
for i in range(h):
    print(" " * (h-i-1) + "#" * (i+1))
