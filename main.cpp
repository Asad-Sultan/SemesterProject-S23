#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>
using namespace std;

// ----- Function Declarations ----- //

int searchStudents(string program, int semester, int rollNo);
bool userExists(string username);

// ----- Aesthetic & Utility Functions ----- //

void setColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}

void printHeader(string text) {
  setColor(9);
  cout << "# ----- ";
  setColor(7);
  cout << text;
  setColor(9);
  cout << " ----- #\n\n";
  setColor(7);
}

void printSuccess(string text) {
  setColor(10);
  cout << text;
  setColor(7);
}

void printWarning(string text) {
  setColor(6);
  cout << text;
  setColor(7);
}

void printError(string text) {
  setColor(12);
  cout << text;
  setColor(7);
}

// ----- Classes ----- //

class User {
protected:
  static int userCount;
  static char currentUserType;
  static int currentUserIndex;

  string username;
  string password;
  string firstName;
  string lastName;
  long int contactNo;
  string address;

  bool lengthCheck(string text, int lenMin, int lenMax) {
    if (text.length() < lenMin || text.length() > lenMax)
      return false;
    else
      return true;
  }

public:
  User(string un = "",
       string pw = "",
       string fN = "",
       string lN = "",
       long int cN = 0,
       string add = "") {
    ++userCount;
    username = un;
    password = pw;
    firstName = fN;
    lastName = lN;
    contactNo = cN;
    address = add;
  }

  // ----- Setters ----- //
  static void setUserCount(int uC) { userCount = uC; }
  static void setCurrentUserType(char cUT) { currentUserType = cUT; }
  static void setCurrentUserIndex(int cUI) { currentUserIndex = cUI; }

  void changePassword() {
    string pwVerify, pwNew, pwConfirm;

    do {
      cout << "Enter current password: ";
      cin >> pwVerify;

      if (pwVerify == password)
        break;
      else
        printError("Incorrect password!\n\n");
    } while (true);

    do {
      cout << "Enter new password: ";
      cin >> pwNew;
      cout << "Confirm password: ";
      cin >> pwConfirm;

      if (pwNew == pwConfirm)
        break;
      else
        printError("Passwords do no match\n\n");
    } while (true);

    password = pwNew;
  }

  // ----- Getters ----- //
  static int getUserCount() { return userCount; }
  static char getCurrentUserType() { return currentUserType; }
  static int getCurrentUserIndex() { return currentUserIndex; }

  string getUsername() { return username; }
  string getPassword() { return password; }
  string getFirstName() { return firstName; }
  string getLastName() { return lastName; }
  long int getContactNumber() { return contactNo; }
  string getAddress() { return address; }

  // ----- Input Setters ----- //

  void inputUsername() {
    string newUsername;

    do {
      do {
        cout << "Enter username: ";
        cin >> newUsername;

        if (lengthCheck(newUsername, 4, 20))
          break;
        else
          printWarning("Username should be between 4 & 20 characters\n\n");
      } while (true);

      if (newUsername == username)
        break;
      else if (userExists(newUsername))
        printError("A record with this username already exits!\n\n");
      else
        break;
    } while (true);

    username = newUsername;
  }

  void inputPassword() {
    do {
      cout << "Enter password: ";
      cin >> password;
      if (lengthCheck(password, 8, 32))
        break;
      else
        printError("Password should be between 8 & 32 characters\n\n");
    } while (true);
  }

  void inputFirstName() {
    cout << "Enter first name: ";
    cin >> firstName;
  }

  void inputLastName() {
    cout << "Enter last name: ";
    cin >> lastName;
  }

  void inputContactNumber() {
    cout << "Enter contact number: ";
    cin >> contactNo;
  }

  void inputAddress() {
    cout << "Enter address: ";
    cin.ignore(1, '\n');
    getline(cin, address);
  }

  void inputData() {
    inputUsername();
    inputPassword();
    inputFirstName();
    inputLastName();
    inputContactNumber();
    inputAddress();
  }
};

class Admin : public User {
private:
  static int adminCount;

public:
  Admin(string un = "",
        string pw = "",
        string fN = "",
        string lN = "",
        long int cN = 0,
        string add = "") : User(un, pw, fN, lN, cN, add) {}

  static void setAdminCount(int aC) { adminCount = aC; }

  // ----- Getters ----- //
  static int getAdminCount() { return adminCount; }

  // ----- Input Setters ----- //
};

struct Subject {
  string name;
  int attendance;
  float gpa;
};

class Student : public User {
private:
  static int studentCount;
  string program;
  int semester;
  int rollNo;
  string studentID;
  float semesterGPA;

  string offeredPrograms[5] = {"BSSE", "BSCS", "BSDS", "BSAI", "BSGM"};

  void createStudentID() {
    studentID = program + "-SM" + to_string(semester) + "-" + to_string(rollNo);
  }

public:
  Student(string un = "",
          string pw = "",
          string fN = "",
          string lN = "",
          long int cN = 0,
          string add = "",
          string p = "",
          int s = 0,
          int rN = 0,
          string sID = "",
          float sGPA = 0.0) : User(un, pw, fN, lN, cN, add) {
    ++studentCount;
    program = p;
    semester = s;
    rollNo = rN;
    studentID = sID;
    semesterGPA = sGPA;
  }

  static void setStudentCount(int sC) { studentCount = sC; }

  // ----- Getters ----- //
  static int getStudentCount() { return studentCount; }
  string getProgram() { return program; }
  int getSemester() { return semester; }
  int getRollNo() { return rollNo; }
  string getStudentID() { return studentID; }

  // ----- Input Setters ----- //
  void inputProgram() {
    cout << endl;
    for (int i = 0; i < 5; i++) {
      cout << i + 1 << ": " << offeredPrograms[i] << endl;
    }
    cout << endl;

    int option;
    do {
      do {
        cout << "Choose a program: ";
        cin >> option;

        if (option < 1 || option > 5)
          printError("Invalid option!\n\n");
        else
          break;
      } while (true);

      if (searchStudents(offeredPrograms[option - 1], semester, rollNo) != -1)
        printError("Duplicate record found!\n\n");
      else
        break;
    } while (true);

    program = offeredPrograms[option - 1];
    void createStudentID();
  }

  void inputSemester() {
    do {
      do {
        cout << "Input semester: ";
        cin >> semester;
        if (semester < 1 || semester > 8)
          printError("Value should be between 1 & 8\n\n");
        else
          break;
      } while (true);

      if (searchStudents(program, semester, rollNo) != -1)
        printError("Duplicate record found!\n\n");
      else
        break;
    } while (true);

    void createStudentID();
  }

  void inputRollNo() {
    do {
      cout << "Input roll number: ";
      cin >> rollNo;
      if (searchStudents(program, semester, rollNo) != -1)
        printError("Duplicate record found!\n\n");
      else
        break;
    } while (true);

    void createStudentID();
  }

  void inputData() {
    User::inputData();
    inputProgram();
    inputSemester();
    inputRollNo();
  }
};

class Teacher : public User {
private:
  static int teacherCount;

public:
  Teacher(string un = "",
          string pw = "",
          string fN = "",
          string lN = "",
          long int cN = 0,
          string add = "") : User(un, pw, fN, lN, cN, add) {}

  static void setTeacherCount(int aC) { teacherCount = aC; }

  // ----- Getters ----- //
  static int getTeacherCount() { return teacherCount; }

  // ----- Input Setters ----- //
};

// ----- Initializing Static Variables ----- //

int User::userCount = 0;
char User::currentUserType = 'G'; // 'G' = Guest
int User::currentUserIndex = -1;

int Admin::adminCount = 0;
int Student::studentCount = 0;
int Teacher::teacherCount = 0;

// ----- Data Vectors ----- //

vector<Admin> adminRecords;
vector<Student> studentRecords;
vector<Teacher> teacherRecords;

// ----- Data Handling ----- //

vector<string> splitLine(string &line) {
  vector<string> result;
  stringstream lineStream(line);
  string item;

  while (getline(lineStream, item, ',')) {
    result.push_back(item);
  }

  return result;
}

void loadData() {
  fstream reader("data.csv", ios::in);

  if (reader.is_open()) {
    int users, admins, students, teachers;
    reader >> users;
    reader >> admins;
    reader >> students;
    reader >> teachers;

    string line;
    vector<string> items;

    // Load admin data
    for (int i = 0; i < admins; i++) {
      reader >> line;
      items = splitLine(line);

      Admin tempAdmin;

      adminRecords.push_back(tempAdmin);
    }

    // Load student data
    for (int i = 0; i < students; i++) {
      reader >> line;
      items = splitLine(line);

      Student tempStudent(items[0],
                          items[1],
                          items[2],
                          items[3],
                          stol(items[4]),
                          items[5],
                          items[6],
                          stoi(items[7]),
                          stoi(items[8]),
                          items[9],
                          stof(items[10]));

      studentRecords.push_back(tempStudent);
    }

    // Load teacher data
    for (int i = 0; i < teachers; i++) {
      reader >> line;
      items = splitLine(line);

      Teacher tempTeacher;

      teacherRecords.push_back(tempTeacher);
    }
  } else {
  }
}

// ----- Search Functions ----- //

int searchStudents(string program, int semester, int rollNo) {
  for (int i = 0; i < studentRecords.size(); i++) {
    if (studentRecords[i].getProgram() == program && studentRecords[i].getSemester() == semester && studentRecords[i].getRollNo() == rollNo)
      return i;
  }
  return -1;
}

int searchStudents(string username) {
  for (int i = 0; i < studentRecords.size(); i++) {
    if (studentRecords[i].getUsername() == username)
      return i;
  }
  return -1;
}

bool userExists(string username) {
  int index;
  index = searchStudents(username);

  if (index = -1)
    return false;
  else
    return true;
}

// ----- Login Function ----- //

void login() {
  string username, password;
  bool loggedIn = false;

  do {
    system("cls");
    printHeader("University Management System");
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    cout << endl;

    for (int i = 0; i < adminRecords.size(); i++) {
      if (adminRecords[i].getUsername() == username && adminRecords[i].getPassword() == password) {
        loggedIn = true;
        User::setCurrentUserType('A');
        User::setCurrentUserIndex(i);
        printSuccess("Login Successful!");
        Sleep(1000);
        break;
      }
    }

    for (int i = 0; i < studentRecords.size(); i++) {
      if (studentRecords[i].getUsername() == username && studentRecords[i].getPassword() == password) {
        loggedIn = true;
        User::setCurrentUserType('S');
        User::setCurrentUserIndex(i);
        printSuccess("Login Successful!");
        Sleep(1000);
        break;
      }
    }

    for (int i = 0; i < teacherRecords.size(); i++) {
      if (teacherRecords[i].getUsername() == username && teacherRecords[i].getPassword() == password) {
        loggedIn = true;
        User::setCurrentUserType('T');
        User::setCurrentUserIndex(i);
        printSuccess("Login Successful!");
        Sleep(1000);
        break;
      }
    }

    if (loggedIn)
      break;
    else {
      printError("Incorrect username/password");
      Sleep(1000);
    }
  } while (true);
}

// ----- Main Function ----- //

void adminPanel() {}

// ----- Main Function ----- //

void studentPanel() {}

// ----- Main Function ----- //

void teacherPanel() {}

// ----- Main Function ----- //

int main() {
  loadData();
  login();

  switch (User::getCurrentUserType()) {
  case 'A':
    adminPanel();
    break;
  case 'S':
    studentPanel();
    break;
  case 'T':
    teacherPanel();
    break;
  }

  cout << endl;
  system("pause");
}