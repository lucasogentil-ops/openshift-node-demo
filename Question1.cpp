#include <assert.h>
#include <stdio.h>

// 1. Swap two integers through their addresses.
void swap_ints(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 2. Swap two characters through their addresses.
void swap_chars(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

// 3. Find the largest value in a nonempty array.
int find_max(int *arr, int size)
{
    int largest = *arr;
    for (int i = 1; i < size; i++)
    {
        if (*(arr + i) > largest)
        {
            largest = *(arr + i);
        }
    }
    return largest;
}

// 4. Reverse an array in place by swapping opposite elements.
void reverse_array(int *arr, int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        swap_ints(arr + i, arr + (size - 1 - i));
    }
}

// 5. Count characters before the string terminator.
int strlen_ptr(char *str)
{
    int length = 0;
    while (*str != '\0')
    {
        length++;
        str++;
    }
    return length;
}

// 6. Count uppercase and lowercase vowels.
int count_vowels(char *str)
{
    int count = 0;
    while (*str != '\0')
    {
        char c = *str;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
        {
            count++;
        }
        str++;
    }
    return count;
}

// 7. Copy a string and write its terminator. Assume enough space in dest.
void copy_string(char *dest, char *src)
{
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

// 8. Append src at the end of dest. Assume enough space in dest.
void concat_strings(char *dest, char *src)
{
    while (*dest != '\0')
    {
        dest++;
    }
    copy_string(dest, src);
}

// 9. Add every value in the array.
int sum_array(int *arr, int size)
{
    int total = 0;
    for (int i = 0; i < size; i++)
    {
        total += *(arr + i);
    }
    return total;
}

// 10. Skip matching characters, then compare the first differing pair.
int compare_strings(char *str1, char *str2)
{
    while (*str1 != '\0' && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    return static_cast<unsigned char>(*str1)
         - static_cast<unsigned char>(*str2);
}

// One main for this standalone practice program.
int main()
{
    int x = 5, y = 10;
    swap_ints(&x, &y);
    assert(x == 10 && y == 5);

    char a = 'A', b = 'B';
    swap_chars(&a, &b);
    assert(a == 'B' && b == 'A');

    int negatives[] = {-8, -3, -10};
    assert(find_max(negatives, 3) == -3);

    int numbers[] = {1, 2, 3, 4, 5};
    reverse_array(numbers, 5);
    for (int i = 0; i < 5; i++)
    {
        assert(*(numbers + i) == 5 - i);
    }

    char hello[] = "Hello";
    char empty[] = "";
    assert(strlen_ptr(hello) == 5);
    assert(strlen_ptr(empty) == 0);

    char vowels[] = "OpenAI";
    assert(count_vowels(vowels) == 4);

    char dest[20] = "abcdefghij";
    copy_string(dest, hello);
    assert(compare_strings(dest, hello) == 0);
    copy_string(dest, empty);
    assert(*dest == '\0');

    char greeting[20] = "Hello, ";
    char name[] = "Lucas";
    char expected[] = "Hello, Lucas";
    concat_strings(greeting, name);
    assert(compare_strings(greeting, expected) == 0);

    assert(sum_array(numbers, 5) == 15);

    char cat[] = "cat";
    char car[] = "car";
    char cats[] = "cats";
    assert(compare_strings(cat, car) > 0);
    assert(compare_strings(cat, cats) < 0);
    assert(compare_strings(cat, cat) == 0);

    puts("All pointer lab checks passed.");
    return 0;
}
