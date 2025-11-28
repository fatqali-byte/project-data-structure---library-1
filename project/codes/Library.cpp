/**
 * @file Library.cpp
 * @brief Complete implementation of Library Management System with File Storage
 */

#include "Library.h"
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;

// ==================== Book Functions Implementation ====================

/**
 * @brief Book constructor
 * @param t Book title
 * @param a Book author
 * @param i ISBN number
 * @param c Book category
 * @param y Publication year
 * @param copies Number of copies
 */
Book::Book(string t, string a, string i, string c, int y, int copies) 
    : title(t), author(a), isbn(i), category(c), year(y), 
      totalCopies(copies), availableCopies(copies), isAvailable(true) {}

/**
 * @brief Display book information
 */
void Book::display() const {
    cout << title << " | " << author << " | " << category 
         << " | " << year << " | " << availableCopies << "/" << totalCopies << endl;
}

// ==================== Linked List Implementation ====================

/**
 * @brief ListNode constructor
 * @param b Book to store in node
 */
ListNode::ListNode(Book b) : book(b), next(nullptr) {}

/**
 * @brief TreeNode constructor
 * @param b Book to store in node
 */
TreeNode::TreeNode(Book b) : book(b), left(nullptr), right(nullptr) {}

// ==================== Library Core Functions ====================

/**
 * @brief Library constructor - loads data from file
 */
Library::Library() : head(nullptr), root(nullptr) {
    loadFromFile(); // Load data when program starts
}

/**
 * @brief Library destructor - saves data to file
 */
Library::~Library() {
    saveToFile(); // Save data when program closes
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// ==================== File System Implementation ====================

/**
 * @brief Save all data to text file
 * @details Saves book data to library_data.txt using | as separator
 */
void Library::saveToFile() {
    ofstream file("library_data.txt");
    if (!file.is_open()) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    
    // Save book count first
    file << allBooks.size() << endl;
    
    // Save each book's data
    for(const auto& book : allBooks) {
        file << book.title << "|" << book.author << "|" << book.isbn << "|" 
             << book.category << "|" << book.year << "|" << book.totalCopies << "|" 
             << book.availableCopies << "|" << book.isAvailable << endl;
    }
    file.close();
    cout << "Data saved to file successfully" << endl;
}

/**
 * @brief Load data from text file
 * @details Loads book data from library_data.txt and rebuilds data structures
 */
void Library::loadFromFile() {
    ifstream file("library_data.txt");
    if (!file.is_open()) {
        cout << "No previous data file found, using default data" << endl;
        // Add default books if no file exists
        addBook("C++ Programming", "Ahmed Ali", "111111", "Programming", 2023, 5);
        addBook("Data Structures", "Sarah Mohamed", "222222", "Programming", 2022, 3);
        addBook("Mathematics", "Dr. Sami", "333333", "Science", 2021, 2);
        return;
    }
    
    // Read book count
    int bookCount;
    file >> bookCount;
    file.ignore(); // Ignore the number line
    
    for(int i = 0; i < bookCount; i++) {
        string line;
        getline(file, line);
        
        vector<string> parts;
        stringstream ss(line);
        string part;
        
        // Split line using | as separator
        while(getline(ss, part, '|')) {
            parts.push_back(part);
        }
        
        if(parts.size() == 8) {
            string title = parts[0];
            string author = parts[1];
            string isbn = parts[2];
            string category = parts[3];
            int year = stoi(parts[4]);
            int totalCopies = stoi(parts[5]);
            int availableCopies = stoi(parts[6]);
            bool isAvailable = (parts[7] == "1");
            
            // Create book object
            Book book(title, author, isbn, category, year, totalCopies);
            book.availableCopies = availableCopies;
            book.isAvailable = isAvailable;
            
            // Add to linked list
            ListNode* newNode = new ListNode(book);
            if (!head) {
                head = newNode;
            } else {
                ListNode* temp = head;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
            
            // Add to BST
            root = insertTree(root, book);
            
            // Add to vector
            allBooks.push_back(book);
        }
    }
    file.close();
    cout << "Loaded " << bookCount << " books from file" << endl;
}

// ==================== Book Management ====================

/**
 * @brief Add new book to library
 * @param title Book title
 * @param author Book author
 * @param isbn ISBN number
 * @param category Book category
 * @param year Publication year
 * @param copies Number of copies
 */
void Library::addBook(string title, string author, string isbn, string category, int year, int copies) {
    Book newBook(title, author, isbn, category, year, copies);
    
    ListNode* newNode = new ListNode(newBook);
    if (!head) {
        head = newNode;
    } else {
        ListNode* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    
    root = insertTree(root, newBook);
    allBooks.push_back(newBook);
    
    saveToFile(); // Save changes to file
    cout << "Book added: " << title << endl;
}

/**
 * @brief Insert book into BST
 * @param node Current node in BST
 * @param book Book to insert
 * @return Pointer to root node of BST
 */
TreeNode* Library::insertTree(TreeNode* node, Book book) {
    if (!node) return new TreeNode(book);
    if (book.title < node->book.title) {
        node->left = insertTree(node->left, book);
    } else {
        node->right = insertTree(node->right, book);
    }
    return node;
}

/**
 * @brief Borrow a book from library
 * @param title Title of book to borrow
 */
void Library::borrowBook(string title) {
    ListNode* current = head;
    while (current) {
        if (current->book.title == title && current->book.availableCopies > 0) {
            current->book.availableCopies--;
            if (current->book.availableCopies == 0) {
                current->book.isAvailable = false;
            }
            saveToFile(); // Save changes to file
            cout << "Book borrowed: " << title << endl;
            return;
        }
        current = current->next;
    }
    cout << "Book not available: " << title << endl;
}

/**
 * @brief Return a book to library
 * @param title Title of book to return
 */
void Library::returnBook(string title) {
    ListNode* current = head;
    while (current) {
        if (current->book.title == title) {
            current->book.availableCopies++;
            current->book.isAvailable = true;
            saveToFile(); // Save changes to file
            cout << "Book returned: " << title << endl;
            return;
        }
        current = current->next;
    }
    cout << "Book not found: " << title << endl;
}

/**
 * @brief Delete a book from library
 * @param title Title of book to delete
 */
void Library::deleteBook(string title) {
    if (!head) {
        cout << "Library is empty!" << endl;
        return;
    }

    ListNode* current = head;
    ListNode* prev = nullptr;
    
    while (current) {
        if (current->book.title == title) {
            deletedBooks.push(current->book);
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            delete current;
            saveToFile(); // Save changes to file
            
            // Rebuild vector after deletion
            allBooks.clear();
            current = head;
            while (current) {
                allBooks.push_back(current->book);
                current = current->next;
            }
            
            cout << "Book deleted: " << title << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Book not found: " << title << endl;
}

/**
 * @brief Restore last deleted book
 */
void Library::restoreBook() {
    if (deletedBooks.empty()) {
        cout << "No deleted books to restore" << endl;
        return;
    }
    
    Book restoredBook = deletedBooks.top();
    deletedBooks.pop();
    addBook(restoredBook.title, restoredBook.author, restoredBook.isbn, 
            restoredBook.category, restoredBook.year, restoredBook.totalCopies);
    cout << "Book restored: " << restoredBook.title << endl;
}

// ==================== Search Algorithms ====================

/**
 * @brief Search for book by title
 * @param title Title to search for
 * @return true if found, false otherwise
 */
bool Library::searchByTitle(string title) {
    searchRequests.push(title);
    bool found = searchTree(root, title);
    cout << (found ? "Book found: " : "Book not found: ") << title << endl;
    return found;
}

/**
 * @brief Search in BST
 * @param node Current node in BST
 * @param title Title to search for
 * @return true if found, false otherwise
 */
bool Library::searchTree(TreeNode* node, string title) {
    if (!node) return false;
    if (node->book.title == title) return true;
    if (title < node->book.title) {
        return searchTree(node->left, title);
    } else {
        return searchTree(node->right, title);
    }
}

/**
 * @brief Linear search algorithm
 * @param title Title to search for
 * @return true if found, false otherwise
 */
bool Library::linearSearch(string title) {
    for (const auto& book : allBooks) {
        if (book.title == title) return true;
    }
    return false;
}

/**
 * @brief Binary search algorithm
 * @param title Title to search for
 * @return true if found, false otherwise
 */
bool Library::binarySearch(string title) {
    if (allBooks.empty()) return false;
    
    vector<Book> sortedBooks = allBooks;
    sort(sortedBooks.begin(), sortedBooks.end(), 
         [](const Book& a, const Book& b) { return a.title < b.title; });
    
    int left = 0, right = sortedBooks.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sortedBooks[mid].title == title) return true;
        if (sortedBooks[mid].title < title) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}

// ==================== Sorting Algorithms ====================

/**
 * @brief Bubble sort algorithm
 */
void Library::bubbleSort() {
    if (allBooks.empty()) {
        cout << "No books to sort" << endl;
        return;
    }
    
    vector<Book> sorted = allBooks;
    for (int i = 0; i < sorted.size()-1; i++) {
        for (int j = 0; j < sorted.size()-i-1; j++) {
            if (sorted[j].title > sorted[j+1].title) {
                swap(sorted[j], sorted[j+1]);
            }
        }
    }
    
    cout << "Books after Bubble Sort:" << endl;
    for (const auto& book : sorted) book.display();
}

/**
 * @brief Selection sort algorithm
 */
void Library::selectionSort() {
    if (allBooks.empty()) {
        cout << "No books to sort" << endl;
        return;
    }
    
    vector<Book> sorted = allBooks;
    for (int i = 0; i < sorted.size()-1; i++) {
        int minIndex = i;
        for (int j = i+1; j < sorted.size(); j++) {
            if (sorted[j].title < sorted[minIndex].title) {
                minIndex = j;
            }
        }
        swap(sorted[i], sorted[minIndex]);
    }
    
    cout << "Books after Selection Sort:" << endl;
    for (const auto& book : sorted) book.display();
}

// ==================== Data Display ====================

/**
 * @brief Display all books
 */
void Library::displayAllBooks() {
    if (!head) {
        cout << "No books in library" << endl;
        return;
    }
    
    cout << "All Books:" << endl;
    ListNode* current = head;
    while (current) {
        current->book.display();
        current = current->next;
    }
}

/**
 * @brief Display sorted books using BST
 */
void Library::displaySortedBooks() {
    if (!root) {
        cout << "No books in library" << endl;
        return;
    }
    cout << "Books Sorted by Title (BST):" << endl;
    inOrder(root);
}

/**
 * @brief In-order traversal of BST
 * @param node Current node in BST
 */
void Library::inOrder(TreeNode* node) {
    if (node) {
        inOrder(node->left);
        node->book.display();
        inOrder(node->right);
    }
}

/**
 * @brief Process all search requests in queue
 */
void Library::processSearchQueue() {
    if (searchRequests.empty()) {
        cout << "No search requests" << endl;
        return;
    }
    
    cout << "Processing Search Queue:" << endl;
    while (!searchRequests.empty()) {
        string title = searchRequests.front();
        searchRequests.pop();
        searchByTitle(title);
    }
}

/**
 * @brief Display library statistics
 */
void Library::displayStatistics() {
    int totalBooks = 0, availableBooks = 0;
    ListNode* current = head;
    while (current) {
        totalBooks++;
        if (current->book.isAvailable) availableBooks++;
        current = current->next;
    }
    
    cout << "Library Statistics:" << endl;
    cout << "Total Books: " << totalBooks << endl;
    cout << "Available Books: " << availableBooks << endl;
    cout << "Deleted Books: " << deletedBooks.size() << endl;
    cout << "Search Requests: " << searchRequests.size() << endl;
}