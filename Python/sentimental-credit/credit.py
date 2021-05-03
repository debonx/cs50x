from cs50 import get_string


def main():
    card = 0
    while card == 0:
        card = get_string("Number: ")
        break
    card_type(card)


def card_type(card):
    if len(card) == 15 and int(card[:2]) in [34, 37]:
        brand = "AMEX"
    elif len(card) in [13, 16] and int(card[:1]) == 4:
        brand = "VISA"
    elif len(card) == 16 and int(card[:2]) in [51, 53, 54, 55]:
        brand = "MASTERCARD"
    else:
        brand = "UNKNOWN"

    valid = valid_sequence(int(card))
    if valid and brand != "UNKNOWN":
        print(brand)
    else:
        print("INVALID")


def digits_of(n):
    return [int(d) for d in str(n)]

# inspired by https://stackoverflow.com/questions/21079439/implementation-of-luhn-formula


def valid_sequence(card):
    digits = digits_of(card)
    odd = digits[-1::-2]
    even = digits[-2::-2]
    checksum = 0
    checksum += sum(odd)
    for d in even:
        checksum += sum(digits_of(d*2))
    return checksum % 10 == 0


main()

