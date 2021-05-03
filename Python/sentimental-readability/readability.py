import re


def main():
    text = input("String: ")
    grade = get_index(text)

    if grade > 15:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


def get_index(text):
    n_words = len(text.split())
    n_letters = len(re.split(r"\w", text)) - 1
    n_sentences = len([sentence for sentence in re.split(r"[!.]|\?\s", text) if sentence])

    L = n_letters * 100 / n_words
    S = n_sentences * 100 / n_words
    index = (0.0588 * L - 0.296 * S - 15.8)

    return round(index)


main()