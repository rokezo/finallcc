#include <iostream>
#include <fstream>
#include <string>

class Student {
private:
    std::string name;
    int id;
    int age;
    std::string major;

public:
    Student(const std::string& name, int id, int age, const std::string& major)
        : name(name), id(id), age(age), major(major) {}

    const std::string& getName() const { return name; }
    void setName(const std::string& name) { this->name = name; }

    int getId() const { return id; }

    int getAge() const { return age; }
    void setAge(int age) { this->age = age; }

    const std::string& getMajor() const { return major; }
    void setMajor(const std::string& major) { this->major = major; }

    void print() const {
        std::cout << "Name: " << name << ", ID: " << id << ", Age: " << age << ", Major: " << major << std::endl;
    }
};

class StudentDatabase {
private:
    Student** students;
    int capacity;
    int count;

public:
    StudentDatabase(int capacity) : capacity(capacity), count(0) {
        students = new Student * [capacity];
    }

    ~StudentDatabase() {
        for (int i = 0; i < count; ++i) {
            delete students[i];
        }
        delete[] students;
    }

    void addStudent(const Student& student) {
        if (count < capacity) {
            students[count++] = new Student(student);
        }
        else {
            std::cerr << "Database is full. Cannot add more students." << std::endl;
        }
    }

    void removeStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getId() == id) {
                delete students[i];
                students[i] = students[count - 1];
                students[--count] = nullptr;
                std::cout << "Student with ID " << id << " has been removed." << std::endl;
                return;
            }
        }
        std::cerr << "Student with ID " << id << " not found." << std::endl;
    }

    Student* findStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getId() == id) {
                return students[i];
            }
        }
        return nullptr;
    }

    void listStudents() const {
        std::cout << "List of Students:" << std::endl;
        for (int i = 0; i < count; ++i) {
            students[i]->print();
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file " << filename << " for writing." << std::endl;
            return;
        }

        for (int i = 0; i < count; ++i) {
            outFile << students[i]->getName() << " "
                << students[i]->getId() << " "
                << students[i]->getAge() << " "
                << students[i]->getMajor() << std::endl;
        }

        outFile.close();
        std::cout << "Data saved to file: " << filename << std::endl;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Error opening file " << filename << " for reading." << std::endl;
            return;
        }

        std::string name, major;
        int id, age;
        while (inFile >> name >> id >> age >> major) {
            addStudent(Student(name, id, age, major));
        }

        inFile.close();
        std::cout << "Data loaded from file: " << filename << std::endl;
    }

    void updateStudent(int id, const Student& newData) {
        Student* student = findStudent(id);
        if (student) {
            student->setName(newData.getName());
            student->setAge(newData.getAge());
            student->setMajor(newData.getMajor());
            std::cout << "Student with ID " << id << " has been updated." << std::endl;
        }
        else {
            std::cerr << "Student with ID " << id << " not found." << std::endl;
        }
    }

    void filterByMajor(const std::string& major) const {
        std::cout << "Students with major " << major << ":" << std::endl;
        for (int i = 0; i < count; ++i) {
            if (students[i]->getMajor() == major) {
                students[i]->print();
            }
        }
    }
};

int main() {
    StudentDatabase database(10);

    int choice;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Add Student\n";
        std::cout << "2. Remove Student\n";
        std::cout << "3. List Students\n";
        std::cout << "4. Save Data to File\n";
        std::cout << "5. Load Data from File\n";
        std::cout << "6. Update Student\n";
        std::cout << "7. Filter Students by Major\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string name, major;
            int id, age;
            std::cout << "Enter student name: ";
            std::cin >> name;
            std::cout << "Enter student ID: ";
            std::cin >> id;
            std::cout << "Enter student age: ";
            std::cin >> age;
            std::cout << "Enter student major: ";
            std::cin >> major;
            database.addStudent(Student(name, id, age, major));
            break;
        }
        case 2: {
            int id;
            std::cout << "Enter student ID to remove: ";
            std::cin >> id;
            database.removeStudent(id);
            break;
        }
        case 3:
            database.listStudents();
            break;
        case 4:
            database.saveToFile("students.txt");
            break;
        case 5:
            database.loadFromFile("students.txt");
            break;
        case 6: {
            int id;
            std::string name, major;
            int age;
            std::cout << "Enter student ID to update: ";
            std::cin >> id;
            std::cout << "Enter updated student name: ";
            std::cin >> name;
            std::cout << "Enter updated student age: ";
            std::cin >> age;
            std::cout << "Enter updated student major: ";
            std::cin >> major;
            database.updateStudent(id, Student(name, id, age, major));
            break;
        }
        case 7: {
            std::string major;
            std::cout << "Enter major to filter by: ";
            std::cin >> major;
            database.filterByMajor(major);
            break;
        }
        case 8:
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please enter a number between 1 and 8.\n";
            break;
        }
    } while (choice != 8);

    return 0;
}
