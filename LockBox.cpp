//version 1.4 windows
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<algorithm>
#include<cctype>
#include<conio.h>
#include<unordered_map>
using namespace std;


class app{
private:
    int passwordCount;
    string username, password, owner, filename;
    unordered_map<string, string> websitePasswords;

public:
    app();
    bool securityCheck();
    void storeData();
    void readData();
    // void encrypt(string &str, char key);
    // void decrypt(string &str, char key);
    void encrypt(string &username, string &password);

    void decrypt(string &uname);
    void setPassword();
    void enterCredentials(string &uname, string &pass);
    void changePassword();
    void runApp();
    void enterPass(string &pass);
    void loadWebsitePasswords();
    void updateWebsitePassword();

    void saveWebsitePasswords();
    ~app();
};

app::app(){
    filename = "data.txt";
    string line;
    ifstream fin(filename);
    getline(fin, line);
    if(line == "")  // if nothing is stored then create a new account
        setPassword();   
    else{          // else store the user name and password in the data memebers -> 'username' and 'password'
        istringstream sin(line);
        sin >> username >> password;
        loadWebsitePasswords();
        string uname = username;
        decrypt(uname);
        this->owner = uname;
    }
    fin.close();
}

void app::setPassword(){ // set new password
    cout << "Enter new Credentials: " << endl;
    enterCredentials(username, password);
    string uname = username;
    decrypt(uname);
    owner = uname;
}

void app::enterCredentials(string &uname, string &pass){
    cout << "Enter the username: ";
    cin >> uname;
    cout << "Enter the password: ";
    enterPass(pass);
    encrypt(uname, pass);
}

bool app::securityCheck(){ // Authentication
    string uname, pword="";
    enterCredentials(uname, pword);
    if(username == uname && password == pword)
        return true;

    cout << "+-------------------------------+" << endl;
    cout << "| Credentials did not match!!!  |" << endl;
    cout << "+-------------------------------+" << endl;

    return false;
}

void app::enterPass(string& pass){ // password hiding while entering
    char ch;
    ch = getch();
    while(ch !=13){
        pass.push_back(ch);
        cout << "*";
        ch = getch();
    }
    cout << endl;
}

void app::storeData(){ // store credentials of a new website
    char choice;
    string website, pass="";
    do{
        cout << "Enter the name of the website: ";
        cin >> website;
        transform(website.begin(), website.end(), website.begin(), ::tolower);

        if(websitePasswords.find(website) == websitePasswords.end()){
            cout << "Enter the password: ";
            enterPass(pass);
            websitePasswords[website] = pass;
        }else{
            cout<<"Website already exists!"<<endl;
        }

        cout << "Want to enter more data: Y/y for yes anything else to exit: ";
        cin >> choice;
        choice = tolower(choice);
    } while (choice == 'y');
}

void app::readData(){ // to print all the data
    cout << endl;
    for (const auto &entry : websitePasswords) {
        cout << entry.first << ": " << entry.second << endl;
    }
    cout << endl;
}
char decrement(char c) { return c - 3; }
char increment(char c) { return c + 3; }

void app::encrypt(string &username, string &password){
    transform(username.begin(), username.end(), username.begin(), increment);
    transform(password.begin(), password.end(), password.begin(), increment);
}

void app::decrypt(string &str){
    transform(str.begin(), str.end(), str.begin(), decrement);
}
// void app::encrypt(string &data, char key) {
//     for (char &c : data) {
//         c ^= key; 
//     }
// }

// void app::decrypt(string &data, char key) {
//     for (char &c : data) {
//         c ^= key; 
//     }
// }

void app::changePassword(){ // Change the master password
    cout << endl
         << endl
         << "To reset username and password enter old Credentials first..." << endl;
    bool admin = securityCheck();
    if(admin){
        password = "";
        setPassword();
    }
}

void app::loadWebsitePasswords() { // loads all the website -> password key-value pairs from file
    ifstream fin(filename);
    string line, website, pass;

    getline(fin, line); // skip master credentials
    while (getline(fin, line)) {
        size_t delimPos = line.find(":");
        if (delimPos != string::npos) {
            website = line.substr(0, delimPos);
            pass = line.substr(delimPos + 2); // skip ": "
            decrypt(website);
            decrypt(pass);
            websitePasswords[website] = pass;
        }
    }
    fin.close();
}

void app::updateWebsitePassword() { // to update the password of an individual website
    string website, newPass;
    cout << "Enter the website to update: ";
    cin >> website;
    transform(website.begin(), website.end(), website.begin(), ::tolower);
    auto it = websitePasswords.find(website);
    if (it != websitePasswords.end()) {
        cout << "Enter new password: ";
        enterPass(newPass);
        websitePasswords[website] = newPass;
        cout << "Password updated successfully!" << endl;
    } else {
        cout << "No password found for " << website << endl;
    }
}

void app::saveWebsitePasswords() { // saves website and password in the file
    ofstream fout("temp.txt");
    fout << username << " " << password << endl;
    for (const auto &entry : websitePasswords) {
        string web = entry.first;
        string pass = entry.second;
        encrypt(web, pass); // encrypting both website and password before saving
        fout << web << ": " << pass << endl;
    }
    fout.close();
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
}

app::~app(){
    saveWebsitePasswords(); // saving the updated data before exiting
    websitePasswords.clear(); // freeing up memory
}

void app::runApp(){
    cout << "+-------------------------------+" << endl;
    cout << "| Welcome, "<<owner<<"      !!! |" << endl;
    cout << "+-------------------------------+" << endl;
    bool admin = securityCheck();

    char choice;
    if(admin){
        char choice;
        do {
            cout << "\nOptions: \nE: Enter data\nR: Read data\nU: Update website password\nP: Change master password\nQ: Quit\n\nEnter choice: ";
            cin >> choice;
            choice = tolower(choice);
            switch (choice) {
                case 'e': storeData(); break;
                case 'r': readData(); break;
                case 'u': updateWebsitePassword(); break;
                case 'p': changePassword(); break;
                case 'q': cout << "Exiting..." << endl; break;
                default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 'q');
    }
}

int main() {
    app newApp;
    newApp.runApp();

    return 0;
}

