/**
 * @file main.cpp
 * @brief Main Interface for Library Management System
 */

#include "Library.h"
#include <iostream>
using namespace std;

/**
 * @brief Display main menu options
 */
void displayMenu() {
    cout << "\n===== Library Management System =====" << endl;
    cout << "1. Add New Book" << endl;
    cout << "2. Search Book by Title" << endl;
    cout << "3. Display All Books" << endl;
    cout << "4. Display Sorted Books (BST)" << endl;
    cout << "5. Borrow Book" << endl;
    cout << "6. Return Book" << endl;
    cout << "7. Delete Book" << endl;
    cout << "8. Restore Deleted Book" << endl;
    cout << "9. Linear Search" << endl;
    cout << "10. Binary Search" << endl;
    cout << "11. Bubble Sort" << endl;
    cout << "12. Selection Sort" << endl;
    cout << "13. Display Statistics" << endl;
    cout << "14. Process Search Queue" << endl;
    cout << "15. Exit" << endl;
    cout << "Choose option: ";
}

/**
 * @brief Main function - program entry point
 */
int main() {
    Library library;
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        string title, author, isbn, category;
        int year, copies;
        
        switch(choice) {
            case 1:
                cout << "Title: "; getline(cin, title);
                cout << "Author: "; getline(cin, author);
                cout << "ISBN: "; getline(cin, isbn);
                cout << "Category: "; getline(cin, category);
                cout << "Year: "; cin >> year;
                cout << "Copies: "; cin >> copies;
                library.addBook(title, author, isbn, category, year, copies);
                break;
            case 2:
                cout << "Title: "; getline(cin, title);
                library.searchByTitle(title);
                break;
            case 3:
                library.displayAllBooks();
                break;
            case 4:
                library.displaySortedBooks();
                break;
            case 5:
                cout << "Title: "; getline(cin, title);
                library.borrowBook(title);
                break;
            case 6:
                cout << "Title: "; getline(cin, title);
                library.returnBook(title);
                break;
            case 7:
                cout << "Title: "; getline(cin, title);
                library.deleteBook(title);
                break;
            case 8:
                library.restoreBook();
                break;
            case 9:
                cout << "Title: "; getline(cin, title);
                cout << (library.linearSearch(title) ? "Found (Linear)" : "Not found (Linear)") << endl;
                break;
            case 10:
                cout << "Title: "; getline(cin, title);
                cout << (library.binarySearch(title) ? "Found (Binary)" : "Not found (Binary)") << endl;
                break;
            case 11:
                library.bubbleSort();
                break;
            case 12:
                library.selectionSort();
                break;
            case 13:
                library.displayStatistics();
                break;
            case 14:
                library.processSearchQueue();
                break;
            case 15:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while(choice != 15);
    
    return 0;
}