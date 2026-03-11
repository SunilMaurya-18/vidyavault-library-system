#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int book_id;
    char title[50];
    char author[50];
    int quantity;
};

void addBook();
void viewBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();

int main()
{
    int choice;

    do
    {
        printf("\n====================================\n");
        printf("      LIBRARY MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Add New Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Remove Book\n");
        printf("7. Exit\n");
        printf("------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            viewBooks();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            issueBook();
            break;
        case 5:
            returnBook();
            break;
        case 6:
            deleteBook();
            break;
        case 7:
            printf("Thank you for using the system.\n");
            break;
        default:
            printf("Invalid Choice!");
        }
    } while (choice != 7);

    return 0;
}

// Add new book
void addBook()
{
    FILE *fp;
    struct Book b;

    fp = fopen("library.dat", "ab");
    if (fp == NULL)
    {
        printf("File cannot be opened!\n");
        return;
    }

    printf("\nEnter Book ID: ");
    scanf("%d", &b.book_id);

    printf("Enter Book Title: ");
    scanf(" %[^\n]", b.title);

    printf("Enter Author Name: ");
    scanf(" %[^\n]", b.author);

    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book added successfully!\n");
}

// View all books
void viewBooks()
{
    FILE *fp;
    struct Book b;

    fp = fopen("library.dat", "rb");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("\nBook ID\tTitle\t\tAuthor\t\tQuantity\n");
    printf("-------------------------------------------------\n");

    while (fread(&b, sizeof(b), 1, fp))
    {
        printf("%d\t%s\t%s\t%d\n",
               b.book_id, b.title, b.author, b.quantity);
    }

    fclose(fp);
}

// Search book
void searchBook()
{
    FILE *fp;
    struct Book b;
    int id, found = 0;

    fp = fopen("library.dat", "rb");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.book_id == id)
        {
            printf("\nBook Found!\n");
            printf("Title: %s\n", b.title);
            printf("Author: %s\n", b.author);
            printf("Quantity: %d\n", b.quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found!\n");

    fclose(fp);
}

// Issue book
void issueBook()
{
    FILE *fp;
    struct Book b;
    int id, found = 0;

    fp = fopen("library.dat", "rb+");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.book_id == id)
        {
            if (b.quantity > 0)
            {
                b.quantity--;
                fseek(fp, -sizeof(b), SEEK_CUR);
                fwrite(&b, sizeof(b), 1, fp);
                printf("Book issued successfully!\n");
            }
            else
            {
                printf("Book not available!\n");
            }
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found!\n");

    fclose(fp);
}

// Return book
void returnBook()
{
    FILE *fp;
    struct Book b;
    int id, found = 0;

    fp = fopen("library.dat", "rb+");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.book_id == id)
        {
            b.quantity++;
            fseek(fp, -sizeof(b), SEEK_CUR);
            fwrite(&b, sizeof(b), 1, fp);
            printf("Book returned successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found!\n");

    fclose(fp);
}
// Delete book
void deleteBook()
{
    FILE *fp, *temp;
    struct Book b;
    int id, found = 0;

    fp = fopen("library.dat", "rb");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        printf("Temporary file error!\n");
        fclose(fp);
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.book_id == id)
        {
            found = 1;
            printf("Book deleted successfully!\n");
        }
        else
        {
            fwrite(&b, sizeof(b), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (!found)
        printf("Book not found!\n");
}