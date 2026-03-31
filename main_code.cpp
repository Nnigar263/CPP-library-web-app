#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;
string get_date() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", ltm);
    return string(buffer);
}

class Library {
public:
    void login(string u, string p) {
        ifstream file("user.txt");
        string line;
        getline(file, line); 

        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string fid, fuser, fpass, frole;
            getline(ss, fid, '|');
            getline(ss, fuser, '|');
            getline(ss, fpass, '|');
            getline(ss, frole, '|');

         
            if(!frole.empty() && frole.back() == '\r') frole.pop_back();

            if (u == fuser && p == fpass) {
                cout << "{\"success\":true,\"role\":\"" << frole << "\",\"id\":\"" << fid << "\",\"name\":\"" << fuser << "\"}";
                return;
            }
        }
        cout << "{\"success\":false,\"message\":\"Invalid Credentials!\"}";
    }

    void add_book(string id, string name, string auth, string qty) {
        ofstream out("book.txt", ios::app);
        out << id << "|" << name << "|" << auth << "|" << qty << endl;
        cout << "{\"success\":true,\"message\":\"Book '" << name << "' added!\"}";
    }

    void view_all() {
        ifstream file("book.txt");
        string line, all = "";
        while (getline(file, line)) all += line + "<br>";
        cout << "{\"success\":true,\"message\":\"" << (all == "" ? "No Books Found" : all) << "\"}";
    }

    void issue_book(string uid, string uname, string bname) {
        ofstream out("issue.txt", ios::app);
        out << uid << "|" << uname << "|" << bname << "|" << get_date() << endl;
        cout << "{\"success\":true,\"message\":\"Book issued to " << uname << "\"}";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) return 0;
    Library lib;
    string act = argv[1];

    if (act == "login" && argc >= 4) lib.login(argv[2], argv[3]);
    else if (act == "add_book" && argc >= 6) lib.add_book(argv[2], argv[3], argv[4], argv[5]);
    else if (act == "view_books") lib.view_all();
    else if (act == "issue_book" && argc >= 5) lib.issue_book(argv[2], argv[3], argv[4]);

    return 0;
}