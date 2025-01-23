import random

def generate_test_data(n):
    with open('test_data_0.txt', 'w') as file:
        for _ in range(n):
            day = str(random.randint(1, 31)).zfill(2)
            month = str(random.randint(1, 12)).zfill(2)
            year = random.randint(1, 9999)
            value = random.randint(0, 2**64 - 1)

            line = f"{day}.{month}.{year} {value}\n"
            file.write(line)

if __name__ == "__main__":
    n = int(input("Введите количество строк: "))
    generate_test_data(n)
