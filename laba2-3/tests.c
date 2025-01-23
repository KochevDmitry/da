// В файле находятся три программы, генерирующие разные типы тестов


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include "patricia.h" // Подключаем заголовочный файл с определением дерева Patricia

// #define NUM_TESTS 100000 // Количество тестов
// #define MAX_LENGTH 256 // Максимальная длина случайной строки

// typedef struct {
//     char* string;
//     int number;
// } Pair;

// void deletePair(Pair* array, int index, size_t* size) {
//     if (index >= *size) {
//         printf("Ошибка: индекс за пределами массива\n");
//         return;
//     }
//         // printf("here\n");
//     // free(array[index].string);
//         // printf("here\n");

//     // Сдвигаем оставшиеся элементы влево
//     for (size_t i = index; i < *size - 1; ++i) {
//         array[i] = array[i + 1];
//     }
//     // printf("here\n");
//     // Уменьшаем размер массива
//     --(*size);

//     // Очищаем последний элемент
//     array[*size].string = NULL;
//     array[*size].number = 0;
// }

// char toLower(char ch) {
//     if (ch >= 'A' && ch <= 'Z') {
//         return ch + ('a' - 'A');
//     }
//     return ch;
// }

// char* stringToBinary(const char* str) {
//     static char binary[1600];
//     // printf("+ %s ", str);
//     memset(binary, 0, sizeof(binary));
//     int index = 0;
//     for (int i = 0; i < strlen(str); ++i) {
//         if (str[i] == '\n')
//             continue;
//         char lowerCh = toLower(str[i]);
//         int asciiCode = (int)lowerCh;
//         for (int j = 4; j >= 0; --j) {
//             binary[index++] = ((asciiCode >> j) & 1) + '0';
//         }
//     }
//     return binary;
// }

// // Функция для генерации случайной строки длины len
// char* generate_random_string(int len) {
//     char* str = (char*)malloc((len + 1) * sizeof(char));
//     for (int i = 0; i < len; ++i) {
//         str[i] = 'a' + rand() % 26; // Генерируем случайный символ от 'a' до 'z'
//     }
//     str[len] = '\0'; // Добавляем завершающий нулевой символ
//     return str;
// }

// // Функция для генерации случайного числа
// unsigned long long generate_random_number() {
//     return rand() % 1000000; // Генерируем число от 0 до 999
// }

// unsigned long long generate_random_number2(int a) {
//     return rand() % a; // Генерируем число от 0 до 999
// }

// int main() {
//     srand(time(NULL)); // Инициализация генератора случайных чисел
//     size_t size = NUM_TESTS;
//     Pair* array = malloc(size * sizeof(Pair));
//     Patricia* root = NULL;
//     int have_root = 0;
//     int a = 0;
//     int b = 0;
//     int c = 0;
//     // Вставляем случайные элементы в дерево
//     for (int i = 0; i < NUM_TESTS; ++i) {
//         char* random_string = generate_random_string(rand() % MAX_LENGTH + 1); // Генерируем случайную строку
//         unsigned long long random_number = generate_random_number(); // Генерируем случайное число
//         printf("%s %llu\n", random_string, random_number);
//         // printf("%llu\n",random_number);

//         array[i].string = random_string;
//         array[i].number = random_number;

//         if (have_root == 0){
//             root = createPatricia(random_number, stringToBinary(random_string));
//             have_root = 1;
//         }
//         else
//             insertToPatricia(root, stringToBinary(random_string), random_number, 0); // Вставляем элемент в дерево
//         a += 1;

//     }

//     for (int i = 0; i < NUM_TESTS; i++){
//         Patricia* found = search(root, stringToBinary(array[i].string), -2); // Ищем элемент в дереве
//         // printf("%s, %s\n", found->word, array[i].string);
//         if (strcmp(found->word, stringToBinary(array[i].string)) == 0) {
//             b += 1;
//         }
//     }

//     // printPatriciaInAddress(root, -2);

//     printf("a:%d, b:%d\n", a, b);
//     for (int i = 0; i < NUM_TESTS / 2; i++){
//         int k = generate_random_number2(NUM_TESTS - i);
//         // printf("%s\n", array[i].string);
//         removeNode(root,  stringToBinary(array[k].string));
//         // printf("here\n");
//         deletePair(array, k, &size);
//     }

//     for (int i = 0; i < NUM_TESTS / 2; i++){
//         Patricia* found = search(root, stringToBinary(array[i].string), -2); // Ищем элемент в дереве
//         if (strcmp(found->word, stringToBinary(array[i].string)) == 0) {
//             c += 1;
//         }
//     }

//     // Удаление дерева Patricia
//     printf("a:%d, b:%d, c:%d", a, b, c);
//     deletePatricia(root, -2);

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRING_LENGTH 256

// Структура для хранения данных теста
typedef struct TestData {
    char* string;
    unsigned long long number;
} TestData;

// Функция для генерации случайной строки
char* generateRandomString(int maxLength) {
    char* str = (char*)malloc((maxLength + 1) * sizeof(char));
    if (str == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    int length = rand() % maxLength + 1;
    for (int i = 0; i < length; ++i) {
        str[i] = 'a' + rand() % 26;
    }
    str[length] = '\0';
    return str;
}

// Функция для генерации случайного числа типа unsigned long long
unsigned long long generateRandomNumber() {
    unsigned long long number = 0;
    for (int i = 0; i < sizeof(unsigned long long); ++i) {
        number = (number << 8) | (rand() & 0xFF);
    }
    return number;
}

// Функция для создания тестовых данных
TestData* createTestData(int numTests) {
    TestData* testData = (TestData*)malloc(numTests * sizeof(TestData));
    if (testData == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numTests; ++i) {
        testData[i].string = generateRandomString(MAX_STRING_LENGTH);
        testData[i].number = generateRandomNumber();
    }
    return testData;
}

// Функция для записи тестовых данных в файл
void writeTestData(FILE* file, TestData* testData, int numTests) {
    for (int i = 0; i < numTests; ++i) {
        fprintf(file, "+ %s %llu\n", testData[i].string, testData[i].number);
    }
}

// Функция для записи запросов на поиск в файл
void writeSearchData(FILE* file, TestData* testData, int numTests, int numSearches) {
    for (int i = 0; i < numSearches; ++i) {
        int index = rand() % numTests;
        fprintf(file, "%s\n", testData[index].string);
    }
}

// Функция для записи запросов на удаление в файл
void writeDeleteData(FILE* file, TestData* testData, int numTests, int numDeletes) {
    for (int i = 0; i < numDeletes; ++i) {
        int index = rand() % numTests;
        fprintf(file, "- %s\n", testData[index].string);
    }
}

int main() {
    srand(time(NULL));

    int numTests = 100000;
    int numSearches = 100000;
    int numDeletes = 100000;

    TestData* testData = createTestData(numTests);

    FILE* file = fopen("tests.txt", "w");
    if (file == NULL) {
        perror("File opening error");
        exit(EXIT_FAILURE);
    }

    writeTestData(file, testData, numTests);
    writeSearchData(file, testData, numTests, numSearches);
    writeDeleteData(file, testData, numTests, numDeletes);

    fclose(file);

    // Освобождаем память, выделенную под строки
    for (int i = 0; i < numTests; ++i) {
        free(testData[i].string);
    }
    free(testData);

    return 0;
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_STRING_LENGTH 256

// // Функция для создания строки вида "a", "aa", "aaa" и т.д.
// char* generateString(int length) {
//     char* str = (char*)malloc((length + 1) * sizeof(char));
//     if (str == NULL) {
//         perror("Memory allocation error");
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < length; ++i) {
//         str[i] = 'a';
//     }
//     str[length] = '\0';
//     return str;
// }

// // Функция для создания тестовых данных
// char** createTestData(int numTests) {
//     char** testData = (char**)malloc(numTests * sizeof(char*));
//     if (testData == NULL) {
//         perror("Memory allocation error");
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < numTests; ++i) {
//         testData[i] = generateString(i + 1);
//     }
//     return testData;
// }

// // Функция для записи тестовых данных в файл
// void writeTestData(FILE* file, char** testData, int numTests) {
//     for (int i = 0; i < numTests; ++i) {
//         fprintf(file, "+ %s %llu\n", testData[i], (unsigned long long)(i + 1));
//     }
// }

// // Функция для записи запросов на поиск в файл
// void writeSearchData(FILE* file, char** testData, int numTests, int numSearches) {
//     for (int i = 0; i < numSearches; ++i) {
//         int index = rand() % numTests;
//         fprintf(file, "%s\n", testData[index]);
//     }
// }

// // Функция для записи запросов на удаление в файл
// void writeDeleteData(FILE* file, char** testData, int numTests, int numDeletes) {
//     for (int i = 0; i < numDeletes; ++i) {
//         int index = rand() % numTests;
//         fprintf(file, "- %s\n", testData[index]);
//     }
// }

// int main() {
//     int numTests = 10;
//     int numSearches = 5;
//     int numDeletes = 5;

//     char** testData = createTestData(numTests);

//     FILE* file = fopen("tests.txt", "w");
//     if (file == NULL) {
//         perror("File opening error");
//         exit(EXIT_FAILURE);
//     }

//     writeTestData(file, testData, numTests);
//     writeSearchData(file, testData, numTests, numSearches);
//     writeDeleteData(file, testData, numTests, numDeletes);

//     fclose(file);

//     // Освобождаем память, выделенную под строки
//     for (int i = 0; i < numTests; ++i) {
//         free(testData[i]);
//     }
//     free(testData);

//     return 0;
// }