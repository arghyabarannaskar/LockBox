//version 1.1
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<algorithm>
#include<cctype>
using namespace std;


class app{
private:
    int passwordCount;
    string username, password;
public:
    app();
    bool securityCheck();
    void storeData();
    void readData();
    void encrypt(string &username, string &password);
    void changePassword();
    void runApp();
};

app::app(){
    string line;
    ifstream fin("store.txt");
    getline(fin, line);
    istringstream sin(line);
    sin >> username >> password;
    passwordCount = username.length() + password.length()+1;
    fin.close();
}

bool app::securityCheck(){
    string uname, pword;
    cout << "Enter the username: ";
    cin >> uname;
    cout << "Enter the password: ";
    cin >> pword;
    encrypt(uname, pword);
    if(username == uname && password == pword)
        return true;

    cout << "+-------------------------------+" << endl;
    cout << "| Credentials did not match!!!  |" << endl;
    cout << "+-------------------------------+" << endl;

    return false;
}

void app::storeData(){
    ofstream fout;
    fout.open("store.txt", ios::app);
    char choice;
    string website, pass;
    do{
        cout << "Enter the name of the website: ";
        cin >> website;
        cout << "Enter the password: ";
        cin >> pass;
        fout << website << " : " << pass << endl;

        cout << "Want to enter more data: Y/y for yes anything else to exit: ";
        cin >> choice;
        choice = tolower(choice);
    } while (choice == 'y');
    fout.close();
}

void app::readData(){
    ifstream fin;
    fin.open("store.txt", fstream::in);

    string line, platform, word;

    cout << "Enter the name of the platform: ";
    cin >> platform;
    transform(platform.begin(), platform.end(), platform.begin(), ::tolower);
    getline(fin, line);
    while (getline(fin, line)) {
        istringstream sin(line);
        sin >> word;
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        int j = 0;
        string trimmed = "";
        for (int i = 0; i < word.size(); i++) {   //Instead of trimming like this I could use getline(sin, word, ',')
            if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
                trimmed = trimmed + word[i];
        }
        cout << trimmed << "    " << platform << endl;
        if (trimmed == platform) {
            cout << "+----------------------------------------------------------" << endl;
            cout << "| " << line << "\t" << endl;
            cout << "+----------------------------------------------------------";
            break;
        }
    }

    fin.close();
}

char increment(char c) { return c + 3; }

void app::encrypt(string &username, string &password){
    transform(username.begin(), username.end(), username.begin(), increment);
    transform(password.begin(), password.end(), password.begin(), increment);
}

void app::changePassword(){
    ifstream fin("store.txt");
    ofstream fout("Copy.txt");
    string line;
    int newCount = 0;
    cout << endl
         << endl
         << "To reset username and password enter old Credentials first..." << endl;
    bool admin = securityCheck();
    if(admin){
        
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter new password: ";
        cin >> password;
        encrypt(username, password);
        newCount = newCount + username.length() + password.length() + 1;
        fout << username << " " << password;
        fin.seekg(passwordCount, ios::beg);
        while(getline(fin, line)){
            fout << line << endl;
        }
        passwordCount = newCount;
        fin.close();
        fout.close();
        remove("store.txt");
        rename("Copy.txt", "store.txt");
    }
}

void app::runApp(){
    bool admin = securityCheck();

    char choice;
    if(admin){
        while(1){
            cout << "\nEnter your choice: E: Enter data,  R: Read data,  P: To reset Password,  Press any other to exit ----> ";
            cin >> choice;
            choice = tolower(choice);
            if(choice == 'e')
                storeData();
            else if(choice == 'r')
                readData();
            else if(choice == 'p')
                changePassword();
            else
                break;
        }
    }
}

int main() {
    app newApp;
    newApp.runApp();

    return 0;
}

