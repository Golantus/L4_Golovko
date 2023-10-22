#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>

class Author;
class Book;

std::ostream& operator<< (std::ostream& out, const Author& Author);
std::ostream& operator<< (std::ostream& out, const Book& Book);

class Author
{
private:
    std::string AuthorName;
    int BirthYear;
public:
    Author(const std::string& AuthorName, int BirthYear);
    void changeBirthYear(int BirthYear);
    void changeName(const std::string& AuthorNewName);
    std::string getStringToHash() const;
    friend std::ostream& operator<< (std::ostream& out, const Author& Author);
    
    
};

class Book
{
private:
    std::string BookTitle;
    std::vector<std::shared_ptr<Author>> BookAuthors;
    int ReleaseYear;
public:
    Book(std::string BookTitle, int ReleaseYear = 0);
    std::string getTitle() const;
    void changeTitle(const std::string& NewTitle);
    void changeYear(int ReleaseYear);
    void changeAuthor(const std::vector<std::shared_ptr<Author>>& NewAuthors);
    void addAuthor(std::shared_ptr<Author>& NewAuthor);
    void removeAuthor(const std::shared_ptr<Author>& AuthorToDelete);
    int getAmountOfAuthors();
    std::vector<std::shared_ptr<Author>> getAuthors() const;
    bool isAuthorRelatedToTheBook(const std::shared_ptr<Author>& AuthorToCheck) const;
    std::string getStringToHash() const;
    friend std::ostream& operator<< (std::ostream& out, const Book& Book);
private:
    std::vector<std::shared_ptr<Author>>::const_iterator findAuthor(const std::shared_ptr<Author>& AuthorToCheck) const;
};


struct AuthorToBooks
{
public:
    AuthorToBooks(std::shared_ptr<Author> NewAuthor)
        :author(NewAuthor) {}
    bool containsBook(const std::shared_ptr<Book>& bookToCheck);
    void removeBook(const std::shared_ptr<Book>& bookToRemove);
public:
    std::shared_ptr<Author> author;
    std::list<std::shared_ptr<Book>> BooksList;
};

class Library
{
private:
    std::unordered_map<std::string, AuthorToBooks> AuthorHashMap;
    std::unordered_map<std::string, std::shared_ptr<Book>> BookHashMap;
public:
    void addAuthorToLibrary(const Author& AuthorToAdd);
    void removeAuthorFromLibrary(const Author& AuthorToDelete);
    void changeAuthorName(const Author& AuthorToChange, std::string NewName);
    void changeAuthorBirthYear(const Author& AuthorToChange, int NewBirthYear);
    void findBooksByAuthor(const Author& AuthorToLook);

    void addBookToLibrary(const Book& BookToAdd);
    void addBookToLibrary(const Book& BookToAdd, const Author& AuthorToAdd);
    void addAuthorToBookAtLibrary(const Book& BookToAddAuthor, const Author& AuthorToAdd);
    void changeAuthorsOfBookAtLibrary(const Book& BookToChangeAuthors, const std::vector<Author>& NewAuthors);
    void removeBookFromLibrary(const Book& BookToRemove);
    void changeBookTitle(const Book& BookToChange, const std::string& NewTitle);
    void changeBookYear(const Book& BookToChange, int year);
    void removeBooksByTitle(const std::string& Title);

    void displayLibraryInfo();
    void displayBookInfo(const Book& BookToDisplay);

private:

    std::shared_ptr<Book> getBookFromLibrary(const Book& BookToGet);
    AuthorToBooks* getAuthorFromLibrary(const Author& AuthorToGet);


    bool isAuthorAtLibrary(const Author& author) const;
    bool isBookAtLibrary(const Book& book) const;

    void removeAllBooksOfAuthor(std::string AuthorKey);
    void removeBookFromAuthors(std::string BookStringToHash);

    void moveAuthorToAnotherHashLocation(std::string oldKeyString, const AuthorToBooks& AuthorToMove);
    void moveBookToAnotherHashLocation(std::string oldKeyString, const  std::shared_ptr<Book>& BookToMove);
};

int main()
{
    Library library;

    Book Book1("Book of X", 2011);
    Book Book2("Book of Y", 2012);
    Book Book3("Book of Z", 2013);
    Author AuthorX("AuthorX", 2001);
    Author AuthorY("AuthorY", 2002);
    Author AuthorZ("AuthorZ", 2003);
   
    Book Book4("Z Book 1", 2014);
    Book Book5("Z Book 2", 2015);
    Book Book6("Z Book 3", 2016);
    Book Book7("co-authored book of Z and Y 1", 2017);
    Book Book8("co-authored book of Z and Y 2", 2018);



    library.addBookToLibrary(Book1, AuthorX);
    library.addBookToLibrary(Book2, AuthorY);
    library.addBookToLibrary(Book3, AuthorZ);

    std::cout << "Start state:\n\n";

    library.displayLibraryInfo();
    
    std::cout << "----------------------------------------------------------------------------------\n";
    std::cout << "Reset 1st book's author to Y:\n\n";

    std::cout << "Before: ";
    library.displayBookInfo(Book1);
    library.changeAuthorsOfBookAtLibrary(Book1, std::vector<Author> {AuthorY});
    std::cout << "After: ";
    library.displayBookInfo(Book1);
    
    std::cout << "----------------------------------------------------------------------------------\n";
    std::cout << "Changed 2nd book's name to Booooook2:\n\n";

    std::cout << "Before: ";
    library.displayBookInfo(Book2);
    library.changeBookTitle(Book2, "Booooook2");
    std::cout << "After: ";
    library.displayBookInfo(Book("Booooook2", 2012));

    std::cout << "----------------------------------------------------------------------------------\n";
    std::cout << "Added 3 more books to Z and 2 co-authored books of Y and Z:\n\n";

    library.addBookToLibrary(Book4, AuthorZ);
    library.addBookToLibrary(Book5, AuthorZ);
    library.addBookToLibrary(Book6, AuthorZ);

    library.addBookToLibrary(Book7, AuthorZ);
    library.addAuthorToBookAtLibrary(Book7, AuthorY);
    library.addBookToLibrary(Book8, AuthorZ);
    library.addAuthorToBookAtLibrary(Book8, AuthorY);

    library.displayLibraryInfo();
    std::cout << "----------------------------------------------------------------------------------\n";
    std::cout << "Deleted Z from library and changed co-authored books' names:\n\n";

    library.removeAuthorFromLibrary(AuthorZ);
    library.changeBookTitle(Book7, "Work of Y (no more Z)");
    library.changeBookTitle(Book8, "Work of Y (no more Z) 2");
    library.displayLibraryInfo();

    std::cout << "----------------------------------------------------------------------------------\n";
    std::cout << "Deleted Y from library and set X name to Writer with no books, displayed books of Writer with no books:\n\n";

    library.removeAuthorFromLibrary(AuthorY);
    library.changeAuthorName(AuthorX, "Writer with no books");
    library.findBooksByAuthor(Author("Writer with no books", 2001));

}

//Author
Author::Author(const std::string& AuthorName, int BirthYear)
{
    changeName(AuthorName);
    changeBirthYear(BirthYear);
}

void Author::changeBirthYear(int BirthYear)
{
    if (BirthYear < 0 || BirthYear > 2023)
    {
        this->BirthYear = 0;
        std::cout << "ERROR: Failed to set birth year of the author" << AuthorName << ", to year " << BirthYear << ", year set to unknown (0)" << '\n';
    }
    else
    {
        this->BirthYear = BirthYear;
    }
}

void Author::changeName(const std::string& AuthorNewName)
{
    const int MAX_NAME_CHARACTERS = 150;
    if (AuthorNewName.length() > MAX_NAME_CHARACTERS)
    {
        std::cout << "ERROR: Failed to set new name of the author" << AuthorName << ", character limit for title is " << MAX_NAME_CHARACTERS << ", name set to first 100 characters" << '\n';
        AuthorName = AuthorNewName.substr(0, MAX_NAME_CHARACTERS);
    }
    else if (AuthorNewName.empty())
    {
        std::cout << "ERROR: Failed to set new name of the author" << AuthorName << ", new name is empty, name set to <Unknown>" << '\n';
        AuthorName = "Unknown";
    }
    else
    {
        AuthorName = AuthorNewName;
    }
}

std::string Author::getStringToHash() const
{
    {
        if (AuthorName == "Unknown")
            return AuthorName;

        return AuthorName + std::to_string(BirthYear);
    }
}

//Book

Book::Book(std::string BookTitle, int ReleaseYear)
{
    changeTitle(BookTitle);
    changeYear(ReleaseYear);
}

std::string Book::getTitle() const
{
    return BookTitle;
}

void Book::changeTitle(const std::string& NewTitle)
{
    const int MAX_TITLE_CHARACTERS = 150;
    if (NewTitle.length() > MAX_TITLE_CHARACTERS)
    {
        BookTitle = NewTitle.substr(0, MAX_TITLE_CHARACTERS);
        std::cout << "ERROR: Failed to set title of the book" << BookTitle << ", character limit for title is " << MAX_TITLE_CHARACTERS << ", title set to first 150 characters" << '\n';
    }
    else if (NewTitle.empty())
    {
        std::cout << "ERROR: Failed to set title of the book" << BookTitle << ", new title is empty, title set to <Unknown>" << '\n';
        BookTitle = "Unknown";
    }
    else
    {
        BookTitle = NewTitle;
    }
}

void Book::changeYear(int ReleaseYear)
{
    if (ReleaseYear < 0 || ReleaseYear > 2023)
    {
        this->ReleaseYear = 0;
        std::cout << "ERROR: Failed to set release year of the book" << BookTitle << ", to year " << ReleaseYear << ", year set to unknown (0)" << '\n';
    }
    else
    {
        this->ReleaseYear = ReleaseYear;
    }
}

void Book::changeAuthor(const std::vector<std::shared_ptr<Author>>& NewAuthors)
{
    BookAuthors = NewAuthors;
}

void Book::addAuthor(std::shared_ptr<Author>& NewAuthor)
{
    if (isAuthorRelatedToTheBook(NewAuthor))
    {
        return;
    }
    BookAuthors.push_back(NewAuthor);
}

void Book::removeAuthor(const std::shared_ptr<Author>& AuthorToDelete)
{
    if (isAuthorRelatedToTheBook(AuthorToDelete))
    {
        BookAuthors.erase(findAuthor(AuthorToDelete));
    }
    else
    {
        std::cout << "ERROR: Author " << *AuthorToDelete << " isn't the author of the book " << BookTitle << "\n";
    }
}

int Book::getAmountOfAuthors()
{
    return BookAuthors.size();
}

std::vector<std::shared_ptr<Author>> Book::getAuthors() const
{
    return BookAuthors;
}

bool Book::isAuthorRelatedToTheBook(const std::shared_ptr<Author>& AuthorToCheck) const
{
    if (findAuthor(AuthorToCheck) == BookAuthors.end())
    {
        return false;
    }
    return true;
}

std::string Book::getStringToHash() const
{
    if (BookTitle == "Unknown")
        return BookTitle;

    return BookTitle + std::to_string(ReleaseYear);
}

std::vector<std::shared_ptr<Author>>::const_iterator Book::findAuthor(const std::shared_ptr<Author>& AuthorToCheck) const
{
    return std::find(BookAuthors.begin(), BookAuthors.end(), AuthorToCheck);
}

//AuthorToBooks

bool AuthorToBooks::containsBook(const std::shared_ptr<Book>& bookToCheck)
{
    auto iterator = std::find(BooksList.begin(), BooksList.end(), bookToCheck);
    if (iterator == BooksList.end())
        return false;
    return true;
}

void AuthorToBooks::removeBook(const std::shared_ptr<Book>& bookToRemove)
{
    auto iterator = std::find(BooksList.begin(), BooksList.end(), bookToRemove);
    BooksList.erase(iterator);
}


//Library
void Library::addAuthorToLibrary(const Author& AuthorToAdd)
{
    std::shared_ptr<Author> NewAuthor = std::make_shared<Author>(AuthorToAdd);
    if (isAuthorAtLibrary(*NewAuthor))
    {
        std::cout << "ERROR: author is already at the library" << '\n';
        return;
    }
    AuthorHashMap.insert(std::pair(
        NewAuthor->getStringToHash(), AuthorToBooks(NewAuthor)
    ));
}

void Library::removeAuthorFromLibrary(const Author& AuthorToDelete)
{
    if (getAuthorFromLibrary(AuthorToDelete) == nullptr)
    {
        return;
    }
    removeAllBooksOfAuthor(AuthorToDelete.getStringToHash());
    AuthorHashMap.erase(AuthorToDelete.getStringToHash());
}

void Library::changeAuthorName(const Author& AuthorToChange, std::string NewName)
{
    AuthorToBooks* AuthorAtLibrary = getAuthorFromLibrary(AuthorToChange);
    if (AuthorAtLibrary == nullptr)
        return;

    std::string oldKeyString = AuthorAtLibrary->author->getStringToHash();

    AuthorAtLibrary->author->changeName(NewName);

    moveAuthorToAnotherHashLocation(oldKeyString, *AuthorAtLibrary);
}

void Library::changeAuthorBirthYear(const Author& AuthorToChange, int NewBirthYear)
{
    AuthorToBooks* AuthorAtLibrary = getAuthorFromLibrary(AuthorToChange);
    if (AuthorAtLibrary == nullptr)
        return;

    std::string oldKeyString = AuthorAtLibrary->author->getStringToHash();

    AuthorAtLibrary->author->changeBirthYear(NewBirthYear);

    moveAuthorToAnotherHashLocation(oldKeyString, *AuthorAtLibrary);
}

void Library::findBooksByAuthor(const Author& AuthorToLook)
{
    AuthorToBooks* AuthorAtLibrary = getAuthorFromLibrary(AuthorToLook);
    if (AuthorAtLibrary == nullptr)
        return;
    if (AuthorAtLibrary->BooksList.empty())
    {
        std::cout << *AuthorAtLibrary->author << " has no books.\n";
    }
    else
    {
        std::cout << *AuthorAtLibrary->author << " books : \n";

        for (auto book : AuthorAtLibrary->BooksList)
        {
            std::cout << *book << '\n';
        }
        std::cout << '\n';
    }
}

void Library::addBookToLibrary(const Book& BookToAdd)
{
    std::shared_ptr<Book> NewBook = std::make_shared<Book>(BookToAdd);
    if (isBookAtLibrary(*NewBook))
    {
        std::cout << "ERROR: book is already at the library" << '\n';
        return;
    }
    BookHashMap.insert(std::pair(
        NewBook->getStringToHash(), NewBook
    ));
}

void Library::addBookToLibrary(const Book& BookToAdd, const Author& AuthorToAdd)
{
    addBookToLibrary(BookToAdd);
    addAuthorToBookAtLibrary(BookToAdd, AuthorToAdd);
}

void Library::changeAuthorsOfBookAtLibrary(const Book& BookToChangeAuthors, const std::vector<Author>& NewAuthors)
{
    std::shared_ptr<Book> BookAtLibrary = getBookFromLibrary(BookToChangeAuthors);
    if (BookAtLibrary == nullptr)
        return;

    removeBookFromAuthors(BookAtLibrary->getStringToHash());
    std::vector<std::shared_ptr<Author>> emptyVector;
    BookAtLibrary->changeAuthor(emptyVector);

    for (auto& author : NewAuthors)
    {
        addAuthorToBookAtLibrary(BookToChangeAuthors, author);
    }
}

void Library::removeBookFromLibrary(const Book& BookToRemove)
{
    if (getBookFromLibrary(BookToRemove) == nullptr)
        return;
    removeBookFromAuthors(BookToRemove.getStringToHash());
    BookHashMap.erase(BookToRemove.getStringToHash());
}

void Library::addAuthorToBookAtLibrary(const Book& BookToAddAuthor, const Author& AuthorToAdd)
{
    std::shared_ptr<Book> BookAtLibrary = getBookFromLibrary(BookToAddAuthor);
    if (BookAtLibrary == nullptr)
        return;
   
    if (isAuthorAtLibrary(AuthorToAdd) == false)
    {
        addAuthorToLibrary(AuthorToAdd);
    }

    AuthorToBooks* AuthorAtLibrary = getAuthorFromLibrary(AuthorToAdd);
    BookAtLibrary->addAuthor(AuthorAtLibrary->author);

    if (AuthorAtLibrary->containsBook(BookAtLibrary) == false)
    {
        AuthorAtLibrary->BooksList.push_back(BookAtLibrary);
    }
}

void Library::changeBookTitle(const Book& BookToChange, const std::string& NewTitle)
{
    std::shared_ptr<Book> BookAtLibrary = getBookFromLibrary(BookToChange);
    if (BookAtLibrary == nullptr)
        return;
    
    std::string oldKeyString = BookToChange.getStringToHash();

    BookAtLibrary->changeTitle(NewTitle);
    
    moveBookToAnotherHashLocation(oldKeyString, BookAtLibrary);
}

void Library::changeBookYear(const Book& BookToChange, int year)
{
    std::shared_ptr<Book> BookAtLibrary = getBookFromLibrary(BookToChange);
    if (BookAtLibrary == nullptr)
        return;

    std::string oldKeyString = BookToChange.getStringToHash();

    BookAtLibrary->changeYear(year);
    
    moveBookToAnotherHashLocation(oldKeyString, BookAtLibrary);

}

void Library::removeBooksByTitle(const std::string& Title)
{
    std::list<Book> booksToDelete;
    for (const auto& [key, book] : BookHashMap)
    {
        if (book->getTitle() == Title)
        {
            booksToDelete.push_back(*book);
        }
    }
    for (const auto& book : booksToDelete)
    {
        removeBookFromLibrary(book);
    }
}

void Library::displayLibraryInfo()
{
    std::cout << "Books info: " << '\n';
    for (const auto& [key, book] : BookHashMap)
    {
        std::cout << *book << '\n';
    }
    std::cout << "Authors info :" << '\n';
    for (const auto& [key, authorStruct] : AuthorHashMap)
    {
        std::cout << *authorStruct.author << '\n';
    }

}

void Library::displayBookInfo(const Book& BookToDisplay)
{
    std::shared_ptr<Book> BookAtLibrary = getBookFromLibrary(BookToDisplay);
    if (BookAtLibrary == nullptr)
        return;
    std::cout << *BookAtLibrary << '\n';
}

std::shared_ptr<Book> Library::getBookFromLibrary(const Book& BookToGet)
{
    if (isBookAtLibrary(BookToGet))
    {
        return BookHashMap.at(BookToGet.getStringToHash());
    }
    else
    {
        std::cout << "ERROR: There is no book " << BookToGet << " in a library" << '\n';
        return nullptr;
    }
}

AuthorToBooks* Library::getAuthorFromLibrary(const Author& AuthorToGet)
{
    if (isAuthorAtLibrary(AuthorToGet))
    {
        return &AuthorHashMap.at(AuthorToGet.getStringToHash());
    }
    else
    {
        std::cout << "ERROR: There is no author " << AuthorToGet << " in a library" << '\n';
        return nullptr;
    }
}

bool Library::isAuthorAtLibrary(const Author& author) const
{
    return AuthorHashMap.contains(author.getStringToHash());
}

bool Library::isBookAtLibrary(const Book& book) const
{
    return BookHashMap.contains(book.getStringToHash());

}

void Library::removeAllBooksOfAuthor(std::string AuthorKey)
{
    AuthorToBooks* AuthorToDelete = &AuthorHashMap.at(AuthorKey);

    std::list<std::shared_ptr<Book>> BooksToDelete;

    for (auto book : AuthorToDelete->BooksList)
    {
        if (book->getAmountOfAuthors() == 1)
        {
            BooksToDelete.push_back(book);
        }
        else
        {
            book->removeAuthor(AuthorToDelete->author);
        }
    }

    for (auto book : BooksToDelete)
    {
        BookHashMap.erase(book->getStringToHash());
    }
}

void Library::removeBookFromAuthors(std::string BookStringToHash)
{
    std::shared_ptr<Book> BookToRemove = BookHashMap.at(BookStringToHash);
    std::vector<std::shared_ptr<Author>> AuthorsThatOwnThisBook = BookToRemove->getAuthors();
    for (auto author : AuthorsThatOwnThisBook)
    {
        AuthorHashMap.at(author->getStringToHash()).removeBook(BookToRemove);
    }
}

void Library::moveAuthorToAnotherHashLocation(std::string oldKeyString, const AuthorToBooks& AuthorToMove)
{
    std::string newKeyString = AuthorToMove.author->getStringToHash();
    AuthorHashMap.insert(std::pair(
        newKeyString, AuthorToMove
    ));

    AuthorHashMap.erase(oldKeyString);
}

void Library::moveBookToAnotherHashLocation(std::string oldKeyString, const std::shared_ptr<Book>& BookToMove)
{
    std::string newKeyString = BookToMove->getStringToHash();
    BookHashMap.insert(std::pair(
        newKeyString, BookToMove
    ));

    BookHashMap.erase(oldKeyString);
}



// out functions
std::ostream& operator<<(std::ostream& out, const Book& Book)
{
    out << Book.BookTitle << ", ";

    out << "book author(s): ";
    if (Book.BookAuthors.size() < 1)
    {
        out << "unknown, ";
    }
    else
    {
        for (size_t i = 0; i < Book.BookAuthors.size(); i++)
        {
            out << *Book.BookAuthors[i] << ", ";
        }
    }

    out << "Book release date: ";
    if (Book.ReleaseYear == 0)
    {
        out << "unknown";
    }
    else
    {
        out << Book.ReleaseYear;
    }
    out << ".";

    return out;
}

std::ostream& operator<<(std::ostream& out, const Author& Author)
{
    out << Author.AuthorName << ", ";

    if (Author.BirthYear == 0)
    {
        out << "year of birth: unknown";
    }
    else
    {
        out << "born in " << Author.BirthYear;
    }


    return out;
}

