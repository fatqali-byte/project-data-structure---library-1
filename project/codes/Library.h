/**
 * @file Library.h
 * @brief Library Management System using Multiple Data Structures
 * @details Includes: Linked List, BST, Stack, Queue, Sorting, Searching, File Storage
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
using namespace std;

/**
 * @brief Book data structure
 */
struct Book {
    string title;
    string author;
    string isbn;
    string category;
    int year;
    int totalCopies;
    int availableCopies;
    bool isAvailable;

    Book(string t = "", string a = "", string i = "", string c = "", 
         int y = 0, int copies = 0);
    void display() const;
};

/**
 * @brief Linked List Node structure
 */
struct ListNode {
    Book book;
    ListNode* next;
    ListNode(Book b);
};

/**
 * @brief Binary Search Tree Node structure
 */
struct TreeNode {
    Book book;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Book b);
};

/**
 * @brief Library management class
 */
class Library {
private:
    ListNode* head;                 // Linked List head pointer
    TreeNode* root;                 // BST root pointer
    stack<Book> deletedBooks;       // Stack for deleted books (LIFO)
    queue<string> searchRequests;   // Queue for search requests (FIFO)
    vector<Book> allBooks;          // Vector for all books

    // BST helper functions
    TreeNode* insertTree(TreeNode* node, Book book);
    bool searchTree(TreeNode* node, string title);
    void inOrder(TreeNode* node);
    
    // File system functions
    void saveToFile();
    void loadFromFile();

public:
    Library();
    ~Library();

    // Core operations
    void addBook(string title, string author, string isbn, string category, int year, int copies);
    void borrowBook(string title);
    void returnBook(string title);
    void deleteBook(string title);
    void restoreBook();
    
    // Search algorithms
    bool searchByTitle(string title);
    bool linearSearch(string title);
    bool binarySearch(string title);
    
    // Sorting algorithms
    void bubbleSort();
    void selectionSort();
    
    // Display functions
    void displayAllBooks();
    void displaySortedBooks();
    void processSearchQueue();
    void displayStatistics();
};

#endif