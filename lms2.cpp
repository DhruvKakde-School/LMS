/* LMS Program by Dhruv Kakde */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>
using namespace std;

// BOOK CLASS
class Book {
public:
    string title;
    string author;
    string uniqueCode;
    int copies = 3;
    bool isAvailable = true;
};

// USER CLASSES
class User {
public:
    string username;
    string password;
    virtual ~User() {}
    virtual void borrowBook(vector<Book>& books) {}
    virtual void returnBook(vector<Book>& books) {}
};

class Student : public User {
public:
    vector<string> borrowedBooks;

    void viewBorrowedBooks() {
        cout << "\nYour borrowed books:\n";
        if (borrowedBooks.empty()) cout << "You have not borrowed any books.\n";
        else for (auto& b : borrowedBooks) cout << "- " << b << endl;
    }

    void borrowBook(vector<Book>& books) override {
        string code; cout << "Enter book code to borrow: "; cin >> code;
        for (auto& b : books) {
            if (b.uniqueCode == code && b.isAvailable && b.copies > 0) {
                b.copies--; if (b.copies == 0) b.isAvailable = false;
                borrowedBooks.push_back(code);
                cout << "\nYou have successfully borrowed \"" << b.title << "\".\n\n";
                ofstream log("borrow_log.txt", ios::app);
                log << username << "|" << code << "|borrowed|" << time(0) << "\n";
                return;
            }
        }
        cout << "\nSorry, the book is not available.\n\n";
    }

    void returnBook(vector<Book>& books) override {
        string code; cout << "Enter book code to return: "; cin >> code;
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), code);
        if (it != borrowedBooks.end()) {
            for (auto& b : books) if (b.uniqueCode == code) { b.copies++; b.isAvailable = true; }
            borrowedBooks.erase(it);
            cout << "\nYou have successfully returned the book.\n\n";
            ofstream log("borrow_log.txt", ios::app);
            log << username << "|" << code << "|returned|" << time(0) << "\n";
        } else cout << "\nYou haven't borrowed this book.\n\n";
    }
};

class Teacher : public User {
public:
    vector<string> borrowedBooks;

    void viewBorrowedBooks() {
        cout << "\nYour borrowed books:\n";
        if (borrowedBooks.empty()) cout << "You have not borrowed any books.\n";
        else for (auto& b : borrowedBooks) cout << "- " << b << endl;
    }

    void borrowBook(vector<Book>& books) override {
        string code; cout << "Enter book code to borrow: "; cin >> code;
        for (auto& b : books) {
            if (b.uniqueCode == code && b.isAvailable && b.copies > 0) {
                b.copies--; if (b.copies == 0) b.isAvailable = false;
                borrowedBooks.push_back(code);
                cout << "\nYou have successfully borrowed \"" << b.title << "\".\n\n";
                ofstream log("borrow_log.txt", ios::app);
                log << username << "|" << code << "|borrowed|" << time(0) << "\n";
                return;
            }
        }
        cout << "\nSorry, the book is not available.\n\n";
    }

    void returnBook(vector<Book>& books) override {
        string code; cout << "Enter book code to return: "; cin >> code;
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), code);
        if (it != borrowedBooks.end()) {
            for (auto& b : books) if (b.uniqueCode == code) { b.copies++; b.isAvailable = true; }
            borrowedBooks.erase(it);
            cout << "\nYou have successfully returned the book.\n\n";
            ofstream log("borrow_log.txt", ios::app);
            log << username << "|" << code << "|returned|" << time(0) << "\n";
        } else cout << "\nYou haven't borrowed this book.\n\n";
    }
};

class Librarian : public User {
public:
    void addBook(vector<Book>& books) {
        string title, author, code; int copies;
        cin.ignore();
        cout << "Enter book title: "; getline(cin, title);
        cout << "Enter author: "; getline(cin, author);
        cout << "Enter unique code: "; getline(cin, code);
        cout << "Enter number of copies: "; cin >> copies;
        books.push_back({title, author, code, copies, copies>0});
        cout << "\nBook added successfully!\n\n";
    }

    void removeBook(vector<Book>& books) {
        string code; cout << "Enter the unique code of the book to remove: "; cin >> code;
        auto it = remove_if(books.begin(), books.end(), [&](Book& b){ return b.uniqueCode==code; });
        if (it != books.end()) { books.erase(it, books.end()); cout << "\nBook removed successfully.\n\n"; }
        else cout << "\nBook not found.\n\n";
    }

    void addUser(vector<User*>& users) {
        cout << "Role (student/teacher/librarian): "; string role; cin >> role;
        cout << "Username: "; string uname; cin >> uname;
        cout << "Password: "; string pass; cin >> pass;
        User* u = nullptr;
        if (role=="student") u=new Student();
        else if (role=="teacher") u=new Teacher();
        else if (role=="librarian") u=new Librarian();
        else { cout<<"\nInvalid role.\n\n"; return; }
        u->username=uname; u->password=pass; users.push_back(u);
        cout<<"\nUser added successfully!\n\n";
    }

    void removeUser(vector<User*>& users) {
        cout << "Enter the username to remove: "; string uname; cin >> uname;
        auto it = remove_if(users.begin(), users.end(), [&](User* u){ return u->username==uname; });
        if (it != users.end()) { for(auto i=it;i!=users.end();++i) delete *i; users.erase(it, users.end()); cout<<"\nUser removed successfully.\n\n"; }
        else cout<<"\nUser not found.\n\n";
    }

    void changeUserCredentials(vector<User*>& users) {
        cout << "Enter username to modify: "; string uname; cin >> uname;
        for(auto u: users){
            if(u->username==uname){
                cout<<"Enter new username: "; cin>>u->username;
                cout<<"Enter new password: "; cin>>u->password;
                cout<<"\nUser credentials updated successfully.\n\n"; return;
            }
        }
        cout<<"\nUser not found.\n\n";
    }
};

// FILE HANDLING
void saveBooks(vector<Book>& books){
    ofstream out("books.txt");
    for(auto& b: books) out<<b.title<<"|"<<b.author<<"|"<<b.uniqueCode<<"|"<<b.copies<<"|"<<b.isAvailable<<"\n";
}

void loadBooks(vector<Book>& books){
    ifstream in("books.txt"); if(!in) return; books.clear();
    string line;
    while(getline(in,line)){
        Book b; vector<string> p; size_t pos=0;
        while((pos=line.find('|'))!=string::npos){p.push_back(line.substr(0,pos)); line.erase(0,pos+1);}
        p.push_back(line);
        if(p.size()==5){ b={p[0],p[1],p[2],stoi(p[3]),p[4]=="1"||p[4]=="true"}; books.push_back(b);}
    }
}

void saveUsers(vector<User*>& users){
    ofstream out("users.txt");
    for(auto u: users){
        string role = dynamic_cast<Student*>(u)?"student":dynamic_cast<Teacher*>(u)?"teacher":"librarian";
        out<<role<<"|"<<u->username<<"|"<<u->password<<"|";
        vector<string>* borrowed=nullptr;
        if(Student* s=dynamic_cast<Student*>(u)) borrowed=&s->borrowedBooks;
        else if(Teacher* t=dynamic_cast<Teacher*>(u)) borrowed=&t->borrowedBooks;
        if(borrowed) for(size_t i=0;i<borrowed->size();i++){out<<borrowed->at(i); if(i!=borrowed->size()-1) out<<",";}
        out<<"\n";
    }
}

void loadUsers(vector<User*>& users){
    ifstream in("users.txt"); if(!in) return; users.clear();
    string line;
    while(getline(in,line)){
        vector<string> p; size_t pos=0;
        while((pos=line.find('|'))!=string::npos){p.push_back(line.substr(0,pos)); line.erase(0,pos+1);}
        p.push_back(line);
        if(p.size()>=3){
            User* u=nullptr;
            if(p[0]=="student") u=new Student();
            else if(p[0]=="teacher") u=new Teacher();
            else if(p[0]=="librarian") u=new Librarian();
            u->username=p[1]; u->password=p[2];
            if(p.size()==4 && !p[3].empty()){
                vector<string> borrowed;
                string temp=p[3]; size_t pos2=0;
                while((pos2=temp.find(','))!=string::npos){borrowed.push_back(temp.substr(0,pos2)); temp.erase(0,pos2+1);}
                borrowed.push_back(temp);
                if(Student* s=dynamic_cast<Student*>(u)) s->borrowedBooks=borrowed;
                else if(Teacher* t=dynamic_cast<Teacher*>(u)) t->borrowedBooks=borrowed;
            }
            users.push_back(u);
        }
    }
}

// MAIN
int main(){
    // Display Banner
    cout << "LL      MMMM   MMM SSSSSSS      SSSSSSS YY    YY SSSSSSS TTTTTTTT EEEEEEE MMM    MMM\n";
    cout << "LL      MMMM  MMMM SS           SS      YY    YY SS         TT    EE      MMMM  MMMM\n";
    cout << "LL      MM MMMM MM SSSSSSS      SSSSSSS   YYYY   SSSSSSS    TT    EEEEE   MM MMMM MM\n";
    cout << "LL      MM  MM  MM      SS           SS    YY         SS    TT    EE      MM  MM  MM\n";
    cout << "LLLLLLL MM      MM SSSSSSS      SSSSSSS    YY    SSSSSSS    TT    EEEEEEE MM      MM\n";
    cout << "=====================================================================================\n";
    cout << "Welcome to the Learning Management System!\n\n";

    vector<Book> books; vector<User*> users;
    loadBooks(books); loadUsers(users);

    // Create default users if empty
    if(users.empty()){
        Student* s1 = new Student(); s1->username="student1"; s1->password="pass123"; users.push_back(s1);
        Student* s2 = new Student(); s2->username="student2"; s2->password="pass456"; users.push_back(s2);
        Teacher* t1 = new Teacher(); t1->username="teacher1"; t1->password="teach1"; users.push_back(t1);
        Librarian* lib = new Librarian(); lib->username="admin"; lib->password="admin"; users.push_back(lib);
    }

    while(true){
        // LOGIN
        int idx=-1;
        idx=[&](){
            while(true){
                string uname, pass; cout<<"Username: "; cin>>uname; cout<<"Password: "; cin>>pass;
                for(size_t i=0;i<users.size();i++) if(users[i]->username==uname && users[i]->password==pass) return i;
                cout<<"\nInvalid credentials. Try again.\n\n";
            }
        }();

        User* current=users[idx]; bool logout=false, exitProgram=false;

        while(!logout && !exitProgram){
            // STUDENT MENU
            if(Student* s=dynamic_cast<Student*>(current)){
                int choice; 
                cout << "\n--- Student Menu ---\n1. Borrow Book\n2. Return Book\n3. View All Books\n4. View Borrowed Books\n5. Logout\n6. Exit\nChoice: ";
                cin>>choice;
                switch(choice){
                    case 1: s->borrowBook(books); break;
                    case 2: s->returnBook(books); break;
                    case 3: for(auto&b:books) cout<<b.title<<" | "<<b.author<<" | "<<b.uniqueCode<<" | "<<b.copies<<"\n"; break;
                    case 4: s->viewBorrowedBooks(); break;
                    case 5: logout=true; break;
                    case 6: exitProgram=true; break;
                    default: cout<<"Invalid choice.\n"; break;
                }
            }
            // TEACHER MENU
            else if(Teacher* t=dynamic_cast<Teacher*>(current)){
                int choice; 
                cout << "\n--- Teacher Menu ---\n1. Borrow Book\n2. Return Book\n3. View All Books\n4. View Borrowed Books\n5. Logout\n6. Exit\nChoice: ";
                cin>>choice;
                switch(choice){
                    case 1: t->borrowBook(books); break;
                    case 2: t->returnBook(books); break;
                    case 3: for(auto&b:books) cout<<b.title<<" | "<<b.author<<" | "<<b.uniqueCode<<" | "<<b.copies<<"\n"; break;
                    case 4: t->viewBorrowedBooks(); break;
                    case 5: logout=true; break;
                    case 6: exitProgram=true; break;
                    default: cout<<"Invalid choice.\n"; break;
                }
            }
            // LIBRARIAN MENU
            else if(Librarian* l=dynamic_cast<Librarian*>(current)){
                int choice;
                cout << "\n--- Librarian Menu ---\n1. Add Book\n2. Remove Book\n3. View All Books\n4. Add User\n5. Remove User\n6. Change User Credentials\n7. Logout\n8. Exit\nChoice: ";
                cin>>choice;
                switch(choice){
                    case 1: l->addBook(books); break;
                    case 2: l->removeBook(books); break;
                    case 3: for(auto&b:books) cout<<b.title<<" | "<<b.author<<" | "<<b.uniqueCode<<" | "<<b.copies<<"\n"; break;
                    case 4: l->addUser(users); break;
                    case 5: l->removeUser(users); break;
                    case 6: l->changeUserCredentials(users); break;
                    case 7: logout=true; break;
                    case 8: exitProgram=true; break;
                    default: cout<<"Invalid choice.\n"; break;
                }
            }
        }

        saveBooks(books); saveUsers(users);
        if(exitProgram) break;
    }

    for(auto u:users) delete u;
    return 0;
}
