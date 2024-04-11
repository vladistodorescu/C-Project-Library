#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 100
#define MAX_BOOKS 50
#define MAX_BOOK_NAME_LEN 100
#define MAX_CLIENTS 10
#define MAX_LOANS 50

typedef struct{
    char title[MAX_BOOK_NAME_LEN];
    char author[MAX_NAME_LEN];
    int copies_available;
}Book;

typedef struct{
    char name[MAX_NAME_LEN];
}Client;

typedef struct {
    char title[MAX_NAME_LEN];
    char author[MAX_NAME_LEN];
    char borrower[MAX_NAME_LEN];
}Loan;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void load_books_from_file(Book books[], const char* filename){
    FILE *file = fopen(filename, "r");

    if (!file){
        printf("Failed to open file for reading! Please try again!");
        return;
    }

    int i = 0;
    while (!feof(file) && i < MAX_BOOKS){
        fscanf(file, "%99[^,], %99[^,], %d\n", books[i].title, books[i].author, &books[i].copies_available);
        i++;
    }

    fclose(file);
}

void save_books_to_file(Book books[], const char* filename){
    FILE *file = fopen(filename, "w");

    if(!file){
        printf("Failed to open file for writing! Please try again!");
    }

    for (int i = 0; i < MAX_BOOKS; i++){
        if (books[i].title[0] != '\0'){
            fprintf(file, "%s, %s, %d\n", books[i].title, books[i].author, books[i].copies_available);
        }
    }

    fclose(file);
}

void load_loans_from_file(Loan loans[], const char* filename){
    FILE *file = fopen(filename, "r");
    if(!file) {
        printf("Failed to open file for reading! Please try again!");
        return;
    }

    int i = 0;
    while (!feof(file) && i < MAX_BOOKS){
        fscanf(file, "%99[^,], %99[^,], %99[^\n]\n", loans[i].title, loans[i].author, loans[i].borrower);
        i++;
    }

    fclose(file);
}

void save_loans_to_file(Loan loans[], const char* filename){
    FILE *file = fopen(filename, "w");
    if(!file) {
        printf("Failed to open file for writing! Please try again!");
        return;
    }

    for (int i = 0; i < MAX_BOOKS && loans[i].title[0] != '\0'; i++){
        fprintf(file, "%s, %s, %s\n", loans[i].title, loans[i].author, loans[i].borrower);
    }

    fclose(file);
}

void donate_books(Book *books) {
    printf("Enter the title of the book you want to donate: \n");
    fflush(stdout);

    clear_input_buffer();

    char book_title[MAX_NAME_LEN];
    fgets(book_title, MAX_NAME_LEN, stdin);
    book_title[strcspn(book_title, "\n")] = '\0';

    printf("Enter the author of the book you want to donate: \n");
    fflush(stdout);

    char book_author[MAX_NAME_LEN];
    fgets(book_author, MAX_NAME_LEN, stdin);
    book_author[strcspn(book_author, "\n")] = '\0';

    for (int i = 0; i < MAX_BOOKS; i++) {
        if (books[i].title[0] == '\0') {
            strcpy(books[i].title, book_title);
            strcpy(books[i].author, book_author);
            books[i].copies_available = 1;
            printf("Thank you for your donation.\n");
            break;
        }
    }
}

void return_books(Book *books, Loan *loans){
    printf("Please enter your user credentials again: \n");
    fflush(stdout);

    clear_input_buffer();

    char user_credentials[MAX_NAME_LEN];
    fgets(user_credentials, MAX_NAME_LEN, stdin);
    user_credentials[strcspn(user_credentials, "\n")] = 0;

    printf("Enter the title of the book you want to return: \n");
    fflush(stdout);

    char book_to_return[MAX_NAME_LEN];
    fgets(book_to_return, MAX_NAME_LEN, stdin);
    book_to_return[strcspn(book_to_return, "\n")] = 0;

    for (int i = 0; i < MAX_LOANS; i++){

        if (strcmp(loans[i].title, book_to_return) == 0 && strcmp(loans[i].borrower, user_credentials) == 0){

            for (int j = i; j < MAX_LOANS - 1; j++){
                loans[j] = loans[j + 1];
            }

            loans[MAX_LOANS-1].title[0] = '\0';
            loans[MAX_LOANS-1].author[0] = '\0';
            loans[MAX_LOANS-1].borrower[0] = '\0';

            printf("You have successfully returned the book!\n");
            return;
        }
    }

    printf("Loan record not found.\n");
}

void view_loans(Loan *loans){
    bool has_loans = false;

    for (int i = 0; i < MAX_LOANS; i++){
        if (loans[i].title[0] != '\0'){
            printf("The book %s, written by %s, was borrowed to %s.\n", loans[i].title, loans[i].author, loans[i].borrower);
            has_loans = true;
        }
    }

    if (!has_loans) {
        printf("There are no current loans.\n");
    }
}

void borrow_book(Book *books, Loan *loans){
    printf("Enter the title of the book you want to borrow:\n");
    fflush(stdout);

    clear_input_buffer();

    char book_to_borrow[MAX_NAME_LEN];
    fgets(book_to_borrow, MAX_NAME_LEN, stdin);
    book_to_borrow[strcspn(book_to_borrow, "\n")] = 0;

    for (int i = 0; i < MAX_BOOKS; i++){
        if (strcmp(book_to_borrow, books[i].title) == 0){
            if (books[i].copies_available == 0){
                printf("Unfortunately, there are no more copies of this book available!\nPlease search for another book!\n");
                return;
            }
            printf("This book is available. Would you like to borrow it?\n(1) Yes\n(2) No\n");
            fflush(stdout);

            int input;
            scanf("%d", &input);
            while (input != 1 && input != 2){
                printf("Wrong input, try again:\n");
                fflush(stdout);
                scanf("%d", &input);
            }

            if (input == 1){
                books[i].copies_available -= 1;
                printf("Please enter your user credentials again: \n");
                fflush(stdout);

                clear_input_buffer();

                char user_credentials[MAX_NAME_LEN];
                fgets(user_credentials, MAX_NAME_LEN, stdin);
                user_credentials[strcspn(user_credentials, "\n")] = 0;
                for (int j = 0; j < MAX_LOANS; j++){
                    if (loans[j].title[0] == '\0'){
                        strncpy(loans[j].title, book_to_borrow, MAX_NAME_LEN);
                        strncpy(loans[j].author, books[i].author, MAX_NAME_LEN);
                        strncpy(loans[j].borrower, user_credentials, MAX_NAME_LEN);
                        break;
                    }
                }
                printf("You have successfully borrowed the book.\n");
                return;
            } else if (input == 2) {
                printf("No problem. Let us know if there's another book you'd like to borrow.\n");
                return;
            }
        }
    }

    printf("The book you requested is not found in our library.\n");
}

void search_books(Book *books) {
    printf("(1) Search books by title\n(2) Search books by author\n");
    int menu_option;
    scanf("%d", &menu_option);
    getchar();

    if (menu_option == 1) {
        printf("Enter the title of the book:\n");
        char book_title[MAX_NAME_LEN];
        fgets(book_title, MAX_NAME_LEN, stdin);
        book_title[strcspn(book_title, "\n")] = 0;

        bool book_found = false;
        for (int i = 0; i < MAX_BOOKS; i++) {
            if (strcmp(book_title, books[i].title) == 0) {
                printf("Book: %s\nAuthor: %s\nCopies available: %d\n", books[i].title, books[i].author,
                       books[i].copies_available);
                book_found = true;
                break;
            }
        }

        if (book_found == false) {
            printf("Book was not found, please try again!\n");
            search_books(books);
        }
    }

    if (menu_option == 2) {
        printf("Enter the Author of the book: ");
        char book_author[MAX_NAME_LEN];
        fgets(book_author, MAX_NAME_LEN, stdin);
        book_author[strcspn(book_author, "\n")] = 0;

        bool author_found = false;
        int nr_books = 0;
        for (int i = 0; i < MAX_BOOKS; i++) {
            if (strcmp(book_author, books[i].author) == 0) {
                if (nr_books == 0) {
                    printf("There are the following books from this author: \n");
                    nr_books += 1;
                    printf("%s: %s\n", books[i].author, books[i].title);
                    author_found = true;
                } else {
                    printf("%s: %s\n", books[i].author, books[i].title);
                }
            }
        }

        if (author_found == false) {
            printf("This author has not been found. Please try again.\n");
            search_books(books);
        }
    }
}

int main(){

    Book books[MAX_BOOKS] = {0};
    Loan loans[MAX_LOANS] = {0};

    Client clients[MAX_CLIENTS]= {
            {"Vlad Istodorescu"},
            {"Maria Andreea"},
            {"Casian Colaj"},

    };

    load_books_from_file(books, "/Users/vladistodorescu/C Projects 4/untitled/Books.txt");
    load_loans_from_file(loans, "/Users/vladistodorescu/C Projects 4/untitled/Loans.txt");

    bool login = false;
    int user_id;

    while (login == false){
        char login_input[MAX_NAME_LEN];
        printf("Login: [First Name] [Last Name]\n");
        fgets(login_input, MAX_NAME_LEN, stdin);
        login_input[strcspn(login_input, "\n")] = 0;
        for (int i = 0; i < MAX_CLIENTS; i++){
            if (strcmp(login_input, clients[i].name) == 0){
                printf("Login succesful!\n");
                login = true;
                user_id = i;
                break;
            }
        }
        if (login == false){
            printf("Login failed. Please try again.\n");
        }
    }

    char user[MAX_NAME_LEN];
    strcpy(user, clients[user_id].name);
    bool exit_app = false;

    while (exit_app == false){
        bool menu_option_chosen = false;
        while (menu_option_chosen == false){
            int menu_option;
            printf("Welcome to the Menu!\n[1] Borrow Books\n[2] Return Books\n[3] Donate Books\n[4] View Loans\n[5] Search Books\n[6] Exit\n");
            scanf("%d", &menu_option);
            if (menu_option == 1){
                menu_option_chosen = true;
                borrow_book(books, loans);
            }
            if (menu_option == 2){
                menu_option_chosen = true;
                return_books(books, loans);

            }
            if (menu_option == 3){
                menu_option_chosen = true;
                donate_books(books);
            }
            if (menu_option == 4){
                menu_option_chosen = true;
                view_loans(loans);
            }
            if (menu_option == 5){
                menu_option_chosen = true;
                search_books(books);
            }
            if (menu_option == 6){
                menu_option_chosen = true;
                exit_app = true;
            }
        }
    }

    save_books_to_file(books, "/Users/vladistodorescu/C Projects 4/untitled/Books.txt");
    save_loans_to_file(loans, "/Users/vladistodorescu/C Projects 4/untitled/Loans.txt");

    return 0;
}
