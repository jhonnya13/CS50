from cs50 import get_float


def main():
    # Ask how many cents the customer is owed
    cents = get_cents()

    # Calculate the number of quarters to give the customer
    quarters = int(calculate_quarters(cents))
    cents = cents - quarters * 25

    # Calculate the number of dimes to give the customer
    dimes = int(calculate_dimes(cents))
    cents = cents - dimes * 10

    # Calculate the number of nickels to give the customer
    nickels = int(calculate_nickels(cents))
    cents = cents - nickels * 5

    # Calculate the number of pennies to give the customer
    pennies = int(calculate_pennies(cents))
    cents = cents - pennies * 1

    # Sum coins
    coins = quarters + dimes + nickels + pennies

    print(coins)


def get_cents():
    while True:
        cents = get_float("Change: ") * 100
        if cents > 0:
            return cents


def calculate_quarters(cents):
    quarter = cents / 25
    return quarter


def calculate_dimes(cents):
    dimes = cents / 10
    return dimes


def calculate_nickels(cents):
    nickels = cents / 5
    return nickels


def calculate_pennies(cents):
    pennies = cents / 1
    return pennies


main()
