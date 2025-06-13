#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Student {
    string name;
    vector<float> homeworks;
    vector<float> exams;
    float project;
    vector<char> otherGrades;

    float finalScore = 0;
    char finalLetter = 'F';
    float gpa = 0;

    void calculateFinalGrade() {
        float hwAvg = 0, examAvg = 0;
        for (float h : homeworks) hwAvg += h;
        for (float e : exams) examAvg += e;
        hwAvg /= homeworks.size();
        examAvg /= exams.size();
        finalScore = hwAvg * 0.4 + examAvg * 0.4 + project * 0.2;

        if (finalScore >= 90) finalLetter = 'A';
        else if (finalScore >= 80) finalLetter = 'B';
        else if (finalScore >= 70) finalLetter = 'C';
        else if (finalScore >= 60) finalLetter = 'D';
        else finalLetter = 'F';
    }

    void calculateGPA() {
        map<char, float> gradeToGPA = {{'A', 4.0}, {'B', 3.0}, {'C', 2.0}, {'D', 1.0}, {'F', 0.0}};
        float sum = gradeToGPA[finalLetter];
        for (char g : otherGrades) {
            sum += gradeToGPA[g];
        }
        gpa = sum / (otherGrades.size() + 1);
    }

    void print() const {
        cout << name << ": HW[";
        for (size_t i = 0; i < homeworks.size(); ++i) {
            cout << homeworks[i] << (i < homeworks.size() - 1 ? ", " : "");
        }
        cout << "] Exams[";
        for (size_t i = 0; i < exams.size(); ++i) {
            cout << exams[i] << (i < exams.size() - 1 ? ", " : "");
        }
        cout << "] Project[" << project << "]\n";
    }

    void printFinal() const {
        cout << name << " - Final Score: " << fixed << setprecision(2) << finalScore
             << " Grade: " << finalLetter << " GPA: " << gpa << endl;
    }
};

vector<Student> readCSV(const string& filename) {
    ifstream file(filename);
    string line;
    vector<Student> students;

    while (getline(file, line)) {
        stringstream ss(line);
        Student s;
        string val;

        // Read name
        getline(ss, s.name, ',');

        // Read 3 homework scores (float)
        for (int i = 0; i < 3; ++i) {
            getline(ss, val, ',');
            s.homeworks.push_back(stof(val));
        }

        // Read 2 exam scores (float)
        for (int i = 0; i < 2; ++i) {
            getline(ss, val, ',');
            s.exams.push_back(stof(val));
        }

        // Read project score (float)
        getline(ss, val, ',');
        s.project = stof(val);

        // Read 4 course grades (char)
        for (int i = 0; i < 4; ++i) {
            getline(ss, val, ',');
            s.otherGrades.push_back(val[0]);
        }

        students.push_back(s);
    }
    return students;
}

void writeToCSV(const string& filename, const vector<Student>& students) {
    ofstream out(filename);
    out << "Name,Final Score,Final Grade,GPA\n";
    for (const auto& s : students) {
        out << s.name << "," << fixed << setprecision(2) << s.finalScore << ","
            << s.finalLetter << "," << s.gpa << "\n";
    }
}

int main() {
    vector<Student> students = readCSV("data/students.csv");

    cout << "Imported Student Data:\n";
    for (auto& s : students) {
        s.print();
        s.calculateFinalGrade();
        s.calculateGPA();
    }

    map<char, int> gradeCount;
    for (const auto& s : students) {
        gradeCount[s.finalLetter]++;
    }

    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });

    cout << "\nSorted Students with Final Grades:\n";
    for (const auto& s : students) {
        s.printFinal();
    }

    writeToCSV("output/student_results.csv", students);

    cout << "\nClass Grade Distribution:\n";
    int total = students.size();
    for (const auto& [grade, count] : gradeCount) {
        cout << grade << ": " << fixed << setprecision(2) << (100.0 * count / total) << "%\n";
    }

    return 0;
}
