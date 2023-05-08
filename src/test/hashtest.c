#include "hashset.h"

void main()
{
    printf("Adding 384\n");
    hashset_add((field)384);

    bool contains = hashset_contains((field)3884);
    printf("Contains 3884: %s\n", contains ? "True" : "False");

    contains = hashset_contains((field)384);
    printf("Contains 384: %s\n", contains ? "True" : "False");

    contains = hashset_contains((field)382);
    printf("Contains 382: %s\n", contains ? "True" : "False");

    printf("Adding 270\n");
    hashset_add((field)270);

    contains = hashset_contains((field)270);
    printf("Contains 270: %s\n", contains ? "True" : "False");

    contains = hashset_contains((field)384);
    printf("Contains 384: %s\n", contains ? "True" : "False");

    printf("Adding 727\n");
    hashset_add((field)727);

    contains = hashset_contains((field)777);
    printf("Contains 777: %s\n", contains ? "True" : "False");

    contains = hashset_contains((field)727);
    printf("Contains 727: %s\n", contains ? "True" : "False");

    printf("Clearing hashset\n");
    hashset_clear();

    contains = hashset_contains((field)384);
    printf("Contains 384: %s\n", contains ? "True" : "False");

    contains = hashset_contains((field)270);
    printf("Contains 270: %s\n", contains ? "True" : "False");

    contains = hashset_contains((field)727);
    printf("Contains 727: %s\n", contains ? "True" : "False");

    int counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);

    printf("Adding 5000\n");
    hashset_add((field)5000);

    contains = hashset_contains((field)5000);
    printf("Contains 5000: %s\n", contains ? "True" : "False");

    counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);

    printf("Adding 5000\n");
    hashset_add((field)5000);

    contains = hashset_contains((field)5000);
    printf("Contains 5000: %s\n", contains ? "True" : "False");

    counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);

    printf("Clearing hashset\n");
    hashset_clear();

    contains = hashset_contains((field)5000);
    printf("Contains 5000: %s\n", contains ? "True" : "False");

    counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);

    printf("Adding 5000\n");
    hashset_add((field)5000);

    contains = hashset_contains((field)5000);
    printf("Contains 5000: %s\n", contains ? "True" : "False");

    counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);

        printf("Adding 5000\n");
    hashset_add((field)5000);

    contains = hashset_contains((field)5000);
    printf("Contains 5000: %s\n", contains ? "True" : "False");

    counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);

        printf("Adding 5000\n");
    hashset_add((field)5000);

    contains = hashset_contains((field)5000);
    printf("Contains 5000: %s\n", contains ? "True" : "False");

    counter = hashset_duplicates();
    printf("Duplicate counter for 5000: %d\n", counter);
}