#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cmath>

using namespace std;

const int FREEZE_TIME = 10;
const int MAX_RETRY = 4;
const int MAX_USER_LIMIT = 100;
const string ADMIN_PASS = "admin2007";
const string USER_PASS = "user0402";
const string STUDENT_FILE = "student_data.txt";
const string USER_FILE = "user_date.txt";

void clearScreen() { system("cls"); }
void pauseSystem() { system("pause"); }
void clearBuffer() { cin.ignore(1024, '\n'); }

time_t getNow() { return time(NULL); }

string inputPasswordWithStars() {
    string password;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            cout << endl;
            break;
        } else if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            if (password.length() < 30) {
                password += ch;
                cout << "*";
            }
        }
    }
    return password;
}

string generateCaptcha() {
    const string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string captcha = "";
    for (int i = 0; i < 6; ++i) {
        captcha += chars[rand() % chars.length()];
    }
    return captcha;
}

bool checkPasswordComplexity(const string& pwd) {
    bool hasDigit = false;
    bool hasLetter = false;
    for (char c : pwd) {
        if (isdigit(c)) hasDigit = true;
        if (isalpha(c)) hasLetter = true;
    }
    return hasDigit && hasLetter;
}

void showCountdown(int seconds) {
    system("color 0C");
    for (int i = seconds; i > 0; --i) {
        cout << "\r账号已冻结！请在 " << i << " 秒后重试......";
        Sleep(1000);
    }
    cout << "\r账号已解冻，请重新登录！            " << endl;
    system("color 0A");
    Sleep(1500);
    clearScreen();
}

void quitAnimation() {
    clearScreen();
    system("color 7c");
    float x, y, a;
    for (y = 1.5f; y > -1; y -= 0.1f) {
        for (x = -1.5f; x < 1.5f; x += 0.05f) {
            a = x * x + y * y - 1;
            putchar(a * a * a - x * x * y * y * y < 0.0f ? '*' : ' ');
        }
        Sleep(10);
        putchar('\n');
    }
    cout << "\n\t欢迎下次使用，祝您生活愉快事业有成学业进步！！！\t\n";
    exit(0);
}

void welcomeAnimation() {
    system("color 0A");
    string arr1 = "Welcome to the King's World!";
    string arr2 = "############################";
    int left = 0, right = arr2.length() - 1;
    while (left <= right) {
        arr2[left] = arr1[left];
        arr2[right] = arr1[right];
        cout << arr2 << endl;
        Sleep(100);
        clearScreen();
        left++;
		right--;
    }
    cout << arr2 << endl;
    Sleep(500);
}

class Student {
public:
    int stuNum;
    string name;
    string dept;
    string className;
    float math, english, computer;
    float total, avg;

    Student() : stuNum(0), math(0), english(0), computer(0), total(0), avg(0) {}

    Student(int n, string na, string d, string c, float m, float e, float com)
        : stuNum(n), name(na), dept(d), className(c), math(m), english(e), computer(com) {
        calculate();
    }

    void calculate() {
        total = math + english + computer;
        avg = total / 3.0f;
    }

    void printRow() const {
        cout << left << setw(8) << stuNum
             << setw(10) << name
             << setw(15) << dept
             << setw(10) << className
             << setw(8) << fixed << setprecision(1) << math
             << setw(8) << english
             << setw(8) << computer
             << setw(8) << total
             << setw(8) << avg << endl;
    }

    friend ostream& operator<<(ostream& os, const Student& s) {
        s.printRow();
        return os;
    }
};

class StudentManager {
private:
    vector<Student> students;

public:
    StudentManager() { loadFromFile(); }

    void loadFromFile() {
        ifstream file(STUDENT_FILE);
        if (!file.is_open()) return;
        students.clear();
        int n;
		string na, d, c;
		float m, e, co;
        float t_dump, a_dump;
        while (file >> n >> na >> d >> c >> m >> e >> co >> t_dump >> a_dump) {
            students.emplace_back(n, na, d, c, m, e, co);
        }
        file.close();
    }

    void saveToFile() {
        ofstream file(STUDENT_FILE);
        if (!file.is_open()) return;
        for (const auto& s : students) {
            file << s.stuNum << " " << s.name << " " << s.dept << " " << s.className << " "
                 << s.math << " " << s.english << " " << s.computer << " "
                 << s.total << " " << s.avg << endl;
        }
        file.close();
    }

    bool checkStuNum(int num) {
        for (const auto& s : students) {
			if (s.stuNum == num) {
				return true;
			}
		}
        return false;
    }

    void inputStudent() {
        int n;
		string na, d, c;
		float m, e, co;
        while (true) {
            cout << "请输入学生学号：";
			cin >> n;
            if (checkStuNum(n)) {
				cout << "该学号已存在，请重新输入！\n";
			} else break;
        }
        cout << "请输入学生姓名：";
		cin >> na;
        cout << "请输入学生系别：";
		cin >> d;
        cout << "请输入学生班级：";
		cin >> c;
        cout << "请输入学生数学成绩：";
		cin >> m;
        cout << "请输入学生英语成绩：";
		cin >> e;
        cout << "请输入学生计算机成绩：";
		cin >> co;

        students.emplace_back(n, na, d, c, m, e, co);
        saveToFile();
        clearBuffer();
        cout << "学生信息录入成功！\n";
        pauseSystem();
        clearScreen();
    }

    void printHeader() {
        cout << "==========================================================================================\n";
        cout << left << setw(8) << "学号" << setw(10) << "姓名" << setw(15) << "系别"
             << setw(10) << "班级" << setw(8) << "数学" << setw(8) << "英语"
             << setw(8) << "机试" << setw(8) << "总分" << setw(8) << "平均" << endl;
        cout << "==========================================================================================\n";
    }

    void printStudent() {
        if (students.empty()) {
			cout << "暂无学生信息！\n";
			pauseSystem();
			return;
		}

        cout << "请选择要打印的方式：\n";
        cout << "1.打印全部学生信息\n";
        cout << "2.按系打印\n";
        cout << "3.按班级打印\n";
        cout << "请输入选择：";
        char choice;
		cin >> choice;
        clearScreen();

        string target = "";
        if (choice == '2') {
			cout << "请输入要打印的系别：";
			cin >> target;
		}else if (choice == '3') {
			cout << "请输入要打印的班级：";
			cin >> target;
		}

        printHeader();
        int count = 0;
        for (const auto& s : students) {
            bool match = false;
            if (choice == '1') match = true;
            else if (choice == '2' && s.dept == target) match = true;
            else if (choice == '3' && s.className == target) match = true;

            if (match) {
			cout << s;
			count++;
			}
        }
        cout << "==========================================================================================\n";
        if(count == 0) cout << "未找到相关信息\n";
        else cout << "共打印" << count << "名学生信息\n";
        pauseSystem();
        clearScreen();
    }

    void countStudent() {
        cout << "学生的总人数为:" << students.size() << endl;
        pauseSystem();
        clearScreen();
    }

    void findStudent() {
        if (students.empty()) {
			cout << "暂无学生信息！\n";
			pauseSystem();
			return;
		}

        clearScreen();
        cout << "请选择查找的方式：\n";
        cout << "1.按学号查找学生\n";
        cout << "2.按姓名查找学生\n";
        cout << "3.查找学生成绩\n";
        cout << "0.返回\n";
        cout << "请输入选择: ";
        char type = _getch();
		cout << endl;

        if (type == '0') return;

        if (type == '1') {
            int num;
			cout << "请输入查找的学生学号：";
			cin >> num;
            bool found = false;
            for (const auto& s : students) {
                if (s.stuNum == num) {
                    cout << "==================================== 学生信息 =================================\n";
                    cout << "学号：" << s.stuNum << "\n姓名：" << s.name << "\n系别：" << s.dept << "\n班级：" << s.className << endl;
                    cout << "数学成绩：" << s.math << " | 英语成绩：" << s.english << " | 计算机成绩：" << s.computer << endl;
                    cout << "总成绩：" << s.total << " | 平均成绩：" << s.avg << endl;
                    cout << "===============================================================================\n";
                    found = true; break;
                }
            }
            if (!found) cout << "未找到学生信息\n";
        }
        else if (type == '2') {
            string nameKey; cout << "请输入姓名: ";
			cin >> nameKey;
            bool found = false;
            int count = 0;
            for (const auto& s : students) {
                if (s.name == nameKey) {
                    if (!found) {
						cout << "\n找到了以下名为 [" << nameKey << "] 的学生:\n";
						printHeader();
					}
                    cout << s;
					found = true;
					count++;
                }
            }
            if (!found) cout << "未找到学生信息\n";
			else cout << "共找到 " << count << " 人。\n";
        }
        else if (type == '3') {
            cout << "请选择科目：1.数学 2.英语 3.计算机\n";
            cout << "请输入选择：";
            char sub = _getch();
			cout << endl;
            if (sub!='1' && sub!='2' && sub!='3') {
				cout<<"选择错误！\n";
				return;
			}

            string subName = (sub=='1'?"数学":(sub=='2'?"英语":"计算机"));
            clearScreen();

            printf("=============================%s成绩列表===============================\n", subName.c_str());
            printf("%-8s %-10s %-15s %-8s %-8s\n","学号","姓名","系别","班级", subName.c_str());
            printf("======================================================================\n");

            int pCount = 0;
            for (const auto& s : students) {
                float sc = (sub=='1'?s.math : (sub=='2'?s.english : s.computer));
                printf("%-8d %-10s %-15s %-8s %-8.1f\n ", s.stuNum, s.name.c_str(), s.dept.c_str(), s.className.c_str(), sc);
                pCount++;
            }
            printf("=========================================================================\n");
            printf("共查询到%d名学生的%s成绩\n", pCount, subName.c_str());
        }
        pauseSystem();
        clearScreen();
    }

    void modifyStudent() {
        int n;
		cout << "请输入要修改的学生的学号：";
		cin >> n;
        for (auto& s : students) {
            if (s.stuNum == n) {
                cout << "============================= 当前学生信息 =======================\n";
                cout << "学号：" << s.stuNum << "\n姓名：" << s.name << "\n系别：" << s.dept << "\n班级：" << s.className << endl;
                cout << "数学成绩：" << s.math << "\n英语成绩：" << s.english << "\n计算机成绩：" << s.computer << endl;
                cout << "总成绩：" << s.total << "\n平均成绩：" << s.avg << endl;
                cout << "===================================================================\n";

                cout << "请选择要修改的字段: \n";
                cout << "1.姓名\n2.系别\n3.班级\n4.数学成绩\n5.英语成绩\n6.计算机成绩\n";
                cout << "请输入你的选择：";
                char c = _getch(); cout << endl;
                if(c=='1') {
					cout<<"请输入新姓名：";
					cin>>s.name;
                }else if(c=='2') {
					cout<<"请输入新系别：";
					cin>>s.dept;
				}else if(c=='3') {
					cout<<"请输入新班级：";
					cin>>s.className;
				}else if(c=='4') {
					cout<<"请输入新的数学成绩：";
					cin>>s.math;
				}else if(c=='5') {
					cout<<"请输入新的英语成绩：";
					cin>>s.english;
				}else if(c=='6') {
					cout<<"请输入新的计算机成绩：";
					cin>>s.computer;
				}else {
					cout<<"选择错误，修改取消！\n";
					return;
				}

                s.calculate();
                saveToFile();
                cout << "修改成功\n";
                pauseSystem();
				clearScreen();
                return;
            }
        }
        cout << "未找到学生信息\n";
        pauseSystem();
		clearScreen();
    }

    void deleteStudent() {
        int n; cout << "请输入删除的学生学号："; cin >> n;
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->stuNum == n) {
                students.erase(it);
                saveToFile();
                cout << "删除成功\n";
                pauseSystem();
				clearScreen();
                return;
            }
        }
        cout << "未找到学生\n";
        pauseSystem();
		clearScreen();
    }

    void sortStudent() {
        if(students.empty()) {
			cout<<"无数据\n";
			return;
		}

        clearScreen();
        cout << "============= 排序范围选择 =============\n";
        cout << "1. 全体学生\n";
        cout << "2. 按系别筛选\n";
        cout << "3. 按班级筛选\n";
        cout << "0. 返回\n";
        cout << "========================================\n";
        cout << "请选择范围: ";
        char scope = _getch();
		cout << endl;

        if (scope == '0') return;

        vector<Student> target;
        string title = "全体";

        if (scope == '1') target = students;
        else if (scope == '2') {
            string d; cout << "请输入系别名称: ";
			cin >> d;
			title=d;
            for(const auto& s : students){
			if(s.dept == d){
				target.push_back(s);
				}
			}
        }
        else if (scope == '3') {
            string c;
			cout << "请输入班级名称: ";
			cin >> c;
			title=c;
            for(const auto& s : students) {
				if(s.className == c) {
					target.push_back(s);
					}
			}
        }
        else {
			cout << "无效选择\n";
			return;
		}

        if(target.empty()) {
			cout<<"未找到符合条件的学生数据。\n";
			pauseSystem();
			return;
		}

        cout << "请选择排序方式：\n";
        cout << "1.按总分降序\n2.按数学升序\n3.按英语升序\n4.按计算机升序\n";
        cout << "请输入你的选择：";
        char key = _getch();

        if(key=='1') sort(target.begin(), target.end(), [](const Student& a, const Student& b){return a.total > b.total;});
        else if(key=='2') sort(target.begin(), target.end(), [](const Student& a, const Student& b){return a.math < b.math;});
        else if(key=='3') sort(target.begin(), target.end(), [](const Student& a, const Student& b){return a.english < b.english;});
        else if(key=='4') sort(target.begin(), target.end(), [](const Student& a, const Student& b){return a.computer < b.computer;});
        else return;

        clearScreen();
        cout << "排序完成,结果如下：\n";
        printHeader();
        for(const auto& s : target) cout << s;

        pauseSystem();
		clearScreen();
    }

    void analyzeStudentScore() {
        if(students.empty()) {
            cout << "暂无学生信息，无法进行成绩分析！\n";
            pauseSystem();
			return;
        }

        clearScreen();
        cout << "============= 分析范围选择 =============\n";
        cout << "1. 全体学生\n";
        cout << "2. 按系别分析\n";
        cout << "3. 按班级分析\n";
        cout << "0. 返回\n";
        cout << "========================================\n";
        cout << "请选择范围: ";
        char scopeOp = _getch();
		cout << endl;

        if (scopeOp == '0') return;

        vector<Student> target;
        string scopeName = "全体";

        if (scopeOp == '1') target = students;
        else if (scopeOp == '2') {
            string d;
			cout << "请输入系别名称: ";
			cin >> d;
			scopeName=d;
            for(const auto& s : students) {
				if(s.dept == d) {
					target.push_back(s);
				}
			}
        }
        else if (scopeOp == '3') {
            string c;
			cout << "请输入班级名称: ";
			cin >> c;
			scopeName=c;
            for(const auto& s : students) {
				if(s.className == c) {
					target.push_back(s);
				}
			}
        }
        else {
			cout << "无效选择\n";
			return;
		}

        if(target.empty()) {
			cout<<"未找到该范围的学生数据。\n";
			pauseSystem();
			return;
		}

        clearScreen();
        cout << "============================================\n";
        cout << "=                请选择分析维度            =\n";
        cout << "=    1 .科目基础分析（平均分/最高分/最低分）\n";
        cout << "=    2 .优秀率/及格率                       \n";
        cout << "============================================\n";
        cout << "请选择：";
        char choice = _getch();
		cout << endl;

        if(choice == '1') {
            float mS=0, eS=0, cS=0, tS=0;
            float mMax=0, eMax=0, cMax=0, tMax=0;
            float mMin=100, eMin=100, cMin=100, tMin=300;
            for(const auto& s : target) {
                mS+=s.math;
				eS+=s.english;
				cS+=s.computer;
				tS+=s.total;
                mMax=max(mMax, s.math);
				eMax=max(eMax, s.english);
				cMax=max(cMax, s.computer);
				tMax=max(tMax, s.total);
                mMin=min(mMin, s.math);
				eMin=min(eMin, s.english);
				cMin=min(cMin, s.computer);
				tMin=min(tMin, s.total);
            }
            int n = target.size();
            cout << "================== " << scopeName << " 基础分析结果===============\n";
            cout << left;
			cout << setw(8) << "科目"
			     << setw(10) << "平均分"
			     << setw(10) << "最高分"
			     << setw(10) << "最低分" << '\n';
            printf("=====================================================\n");

        	cout << setw(8) << "数学"
			     << setw(10) << setprecision(1) << fixed << mS/n
			     << setw(10) << mMax
			     << setw(10) << mMin << '\n';
			cout << setw(8) << "英语"
			     << setw(10) << eS/n
			     << setw(10) << eMax
			     << setw(10) << eMin << '\n';
			cout << setw(8) << "计算机"
				 << setw(10) << cS/n
				 << setw(10) << cMax
				 << setw(10) << cMin << '\n';
			cout << setw(8) << "总分"
			     << setw(10) << tS/n
			     << setw(10) << tMax
			     << setw(10) << tMin << '\n';
        }
        else if(choice == '2') {
            cout << "请选择统计的科目：\n1.数学\n2.英语\n3.计算机\n4.总分\n";
            cout << "请选择：";
            char subChoice = _getch();
			cout << endl;
            if(subChoice < '1' || subChoice > '4') {
				cout<<"选择错误\n";
				return;
			}

            int excellent=0, pass=0, fail=0;
            int n=target.size();

            for(const auto& s : target) {
                float v;
                if(subChoice=='1') v=s.math;
                else if(subChoice=='2') v=s.english;
                else if(subChoice=='3') v=s.computer;
                else v=s.total;

                if(v >= 90) excellent++;
                if(v >= 60) pass++;
				else fail++;
            }

            float excellentRate = (excellent * 100.0f) / n;
            float passRate = (pass * 100.0f) / n;

            cout << "======================统计结果===================\n";
            cout << "总人数：" << n << endl;
            cout << left
			     << setw(8) << "类别"
			     << setw(8) << "人数"
			     << setw(10) << "占比" << '\n';
            cout << "=================================================\n";
            cout << setw(8) << "优秀"
			     << setw(8) << excellent
			     << setw(10) << fixed << setprecision(1) << excellentRate << "%\n";
            cout << setw(8) << "及格"
			     << setw(8) << pass
			     << setw(10) << passRate << "%\n";
			cout << setw(8) << "不及格"
			     << setw(8) << fail
			     << setw(10) << (100.0f - passRate) << "%\n";
		cout << "==================================================\n";
		cout << "优秀（>=90） | 及格（>=60）\n";
		cout << "====================================================\n";

        }
        else { cout << "选择错误，分析取消！\n"; }

        cout << "====================================================\n";
        pauseSystem(); clearScreen();
    }
};



class BaseUser {
public:
    virtual ~BaseUser() {}
    virtual void showMenu() = 0;
    virtual void handleInput(StudentManager& sm) = 0;
};


class AdminUser : public BaseUser {
public:
    void showMenu() override {
        clearScreen();
        cout << "*******************************************************\n";
		cout << "* 欢迎使用学生成绩管理系统                            *\n";
		cout << "*******************************************************\n";
		cout << "* 请选择功能                                          *\n";
		cout << "* 1.录入学生信息                                      *\n";
		cout << "* 2.打印学生信息                                      *\n";
		cout << "* 3.统计学生信息                                      *\n";
		cout << "* 4.查找学生信息                                      *\n";
		cout << "* 5.修改学生信息                                      *\n";
		cout << "* 6.删除学生信息                                      *\n";
		cout << "* 7.按成绩排序                                        *\n";
		cout << "* 8.成绩分析                                          *\n";
		cout << "* 0.退出系统                                          *\n";
		cout << "*******************************************************\n";
		cout << "请选择：";
    }

    void handleInput(StudentManager& sm) override {
        while(true) {
            showMenu();
            char c = _getch();
		if(c == '0') {
                clearScreen();
                cout << '\n';
				cout << "*******************************************************\n";
				cout << "* 您确定要彻底退出系统吗？                            *\n";
				cout << "*******************************************************\n";
				cout << "* [Y] 是，退出程序  [N] 否，返回系统                  *\n";
				cout << "*******************************************************\n";
				cout << "请选择：";
                char sure = _getch();
                if(sure == 'y' || sure == 'Y') {
                    quitAnimation();
                } else {
                    return;
                }
            }
            switch(c) {
                case '1':
					sm.inputStudent();
					break;
                case '2':
					sm.printStudent();
					break;
                case '3':
					sm.countStudent();
					break;
                case '4':
					sm.findStudent();
					break;
                case '5':
					sm.modifyStudent();
					break;
                case '6':
					sm.deleteStudent();
					break;
                case '7':
					sm.sortStudent();
					break;
                case '8':
					sm.analyzeStudentScore();
					break;
                default:
                    printf("没有该选项，请重新输入！\n");
                    pauseSystem();
					clearScreen();
                    break;
            }
        }
    }
};


class NormalUser : public BaseUser {
public:
    void showMenu() override {
        clearScreen();
        cout << "********************************************" << endl;
		cout << "* 欢迎使用学生成绩管理系统               *" << endl;
		cout << "********************************************" << endl;
		cout << "* 请选择功能                          *" << endl;
		cout << "* 1.打印学生信息                      *" << endl;
		cout << "* 2.统计学生信息                      *" << endl;
		cout << "* 3.查找学生信息                      *" << endl;
		cout << "* 4.成绩分析                          *" << endl;
		cout << "* 0.退出系统                          *" << endl;
		cout << "*********************************************" << endl;
		cout << "请选择：";
    }

    void handleInput(StudentManager& sm) override {
        while(true) {
            showMenu();
            char c = _getch();
		if(c == '0') {
                clearScreen();
                cout << '\n';
				cout << "*******************************************************\n";
				cout << "* 您确定要彻底退出系统吗？                            *\n";
				cout << "*******************************************************\n";
				cout << "* [Y] 是，退出程序  [N] 否，返回系统                  *\n";
				cout << "*******************************************************\n";
				cout << "请选择：";
                char sure = _getch();
                if(sure == 'y' || sure == 'Y') {
                    quitAnimation();
                } else {
                    return;
                }
            }
            switch(c) {
                case '1':
					sm.printStudent();
					break;
                case '2':
					sm.countStudent();
					break;
                case '3':
					sm.findStudent();
					break;
                case '4':
					sm.analyzeStudentScore();
					break;
                default:
                    printf("没有该选项，请重新输入！\n");
                    pauseSystem();
					clearScreen();
                    break;
            }
        }
    }
};

//用户

struct UserData {
    string username;
    string password;
    string question;
    string answer;
    int attemptCount;
    time_t freezeStartTime;
};

class UserManager {
private:
    vector<UserData> users;
    int systemLoginAttempt = 0;
    time_t systemFreezeStart = 0;

public:
    UserManager() { loadFromFile(); }

    void loadFromFile() {
        ifstream file(USER_FILE);
        if (!file.is_open()) return;
        users.clear();
        UserData u;
        while (file >> u.username >> u.password >> u.question >> u.answer >> u.attemptCount >> u.freezeStartTime) {
            users.push_back(u);
        }
        file.close();
    }

    void saveToFile() {
        ofstream file(USER_FILE);
        for (const auto& u : users) {
            file << u.username << " " << u.password << " " << u.question << " "
                 << u.answer << " " << u.attemptCount << " " << u.freezeStartTime << endl;
        }
        file.close();
    }

    // 核心登录逻辑
    BaseUser* login() {
        clearScreen();
        // 检查系统级冻结
        if (systemFreezeStart != 0 && (getNow() - systemFreezeStart < FREEZE_TIME)) {
            showCountdown(FREEZE_TIME - (getNow() - systemFreezeStart));
            return nullptr;
        }


        cout << "\n**************请选择系统**************" << endl;
		cout << "********** 1.管理员系统   **********" << endl;
		cout << "********** 2.学生系统     **********" << endl;
		cout << "**************************************" << endl;
		char type = _getch();
		cout << "你的选择是：" << type << endl;

        string sysName = (type == '1' ? "管理员系统" : "学生系统");
        string correctPass = (type == '1' ? ADMIN_PASS : USER_PASS);

        if(type != '1' && type != '2') {
             printf("无效选择，请重新选择！\n");
             Sleep(500);
			 clearScreen();
			 return nullptr;
        }

        printf("请输入密码：\n");
        string input = inputPasswordWithStars();

        if (input == correctPass) {
            printf("%s登录成功！\n", sysName.c_str());
            Sleep(500);
            systemLoginAttempt = 0;
            if (type == '1') return new AdminUser();
            else return new NormalUser();
        } else {
            systemLoginAttempt++;
            printf("密码错误，剩余尝试次数：%d\n", MAX_RETRY - systemLoginAttempt);
            if (systemLoginAttempt >= MAX_RETRY) {
                systemFreezeStart = getNow();
                printf("登陆失败次数太多，账号冻结 %d 秒\n", FREEZE_TIME);
            }
            Sleep(1000);
            clearScreen();
            return nullptr;
        }
    }

    // 账号登录（old）
    bool loginUserAccount() {
        string name;
        cout<<"请输入姓名：\n";
		cin >> name;
        UserData* target = nullptr;
        for(auto& u : users) if(u.username == name) target = &u;

        if(!target) {
            cout << "该用户名不存在！\n";
            pauseSystem();
			clearScreen();
			return false;
        }

        if(target->freezeStartTime != 0 && (getNow() - target->freezeStartTime < FREEZE_TIME)) {
            cout << "账号冻结中，剩余" << (FREEZE_TIME - (getNow() - target->freezeStartTime)) << "秒" << endl;
             pauseSystem();
			 clearScreen();
			 return false;
        }

        while(true) {
            cout << "请输入密码：";
            string pwd = inputPasswordWithStars();
            if(pwd == target->password) {
                cout << "登陆成功,欢迎回来" << name << "!" << endl;
                target->attemptCount = 0;
                saveToFile();
                Sleep(1000);
                clearScreen();
                return true;
            } else {
                target->attemptCount++;
                if(target->attemptCount >= MAX_RETRY) {
                    target->freezeStartTime = getNow();
                    saveToFile();
         			cout << "\033[1;31m";
                	cout << "密码错误，账号将被冻结10秒" << endl;
                	cout << "\033[0m";
                    showCountdown(FREEZE_TIME);
                    return false;
                }
				cout << "\033[1;31m";
            	cout << "密码错误还剩" << (MAX_RETRY - target->attemptCount) << "次机会" << endl;
            	cout << "\033[0m";
            	cout << "请重新输入密码......" << endl;                Sleep(500);
                system("color 0A");

                clearScreen();
            }
        }
    }

    void registerUser() {
        if(users.size() >= MAX_USER_LIMIT) {
			cout << "注册人数已达到上线，无法注册！" << endl;
			return;
		}
        clearScreen();
        cout << "\n==== 用户注册 ====" << endl;
        string name;
		cout << "请输入用户姓名：" << endl;
		cin >> name;
        for(const auto& u : users) if(u.username == name) {
            cout << "该用户已存在，请输入其他用户名！" << endl;
            pauseSystem();
			clearScreen();
			return;
        }

        string captcha = generateCaptcha();
        string inCap;
		cout << "请输入验证码(" << captcha << ")：";
		cin >> inCap;
        if(inCap != captcha) {
            cout << "验证码错误，你是人机！" << endl;
            Sleep(1000);
			clearScreen();
			return;
        }

        string p1, p2;
        while(true) {
            cout << "请设置您的密码(最多10位)：";
			p1 = inputPasswordWithStars();
            if(!checkPasswordComplexity(p1) || p1.length()>10) {
                cout << "密码必须包含数字和字母且不超过10位！" << endl;
				continue;
            }
            cout << "请再次输入密码：";
			p2 = inputPasswordWithStars();
            if(p1 == p2)
			break;
            cout << "两次输入的密码不一样，请重新输入！" << endl;
        }

        UserData u;
        u.username = name;
		u.password = p1;
		u.attemptCount = 0;
		u.freezeStartTime = 0;
        cout << "请设置安全问题：" << endl;
		clearBuffer();
		getline(cin, u.question);
        cout << "请设置答案：" << endl;
		getline(cin, u.answer);

        users.push_back(u);
        saveToFile();
        cout << "注册成功！" << endl;
        Sleep(1000);
		clearScreen();
    }

    void forgetPassword() {
        string name;
		printf("====== 找回密码 =====\n请输入你的用户名>\n");
		cin >> name;
        UserData* target = nullptr;
        for(auto& u : users){
			if(u.username == name){
				target = &u;
			}
		}

        if(!target) {
            printf("该用户名不存在！\n");
            clearScreen();
			return;
        }

        printf("安全问题：%s\n", target->question.c_str());
        printf("请输入答案：");
        string ans;
		clearBuffer();
		getline(cin, ans);

        if(ans == target->answer) {
            printf("回答正确\n");
            string p1, p2;
            while(true) {
                printf("请输入您的新密码（必须包含数字和字母最多十个）：\n");
				p1 = inputPasswordWithStars();
                if(!checkPasswordComplexity(p1)) {
					printf("密码必须包含数字和字母！\n");
					continue;
				}
                printf("请再次输入你的密码：\n");
				p2 = inputPasswordWithStars();
                if(p1 == p2) break;
                printf("两次输入的密码不一样请重新设置！\n");
            }
            target->password = p1;
            target->attemptCount = 0;
            saveToFile();
            printf("密码重置成功！\n");
        } else {
            printf("答案错误！\n");
        }
        pauseSystem();
		clearScreen();
    }
};

int main() {
    srand(time(0));
    StudentManager sm;
    UserManager um;

    welcomeAnimation();

    // 第一层循环：登录/注册
    bool isUserLoggedIn = false;
    while(!isUserLoggedIn) {
        cout << "\n";
		cout << "================ 请选择登陆方式 =================" << endl;
		cout << "================ 1. 注册新用户  =================" << endl;
		cout << "================ 2. 登录账号    =================" << endl;
		cout << "================ 3. 忘记密码    =================" << endl;
		cout << "================ 4. 退出系统    =================" << endl;
		cout << "=================================================" << endl;

        char c = _getch();
        cout << "\n";

        switch(c) {
            case '1':
				um.registerUser();
				break;
            case '2':
				if(um.loginUserAccount()) {
					isUserLoggedIn = true;
				}
				break;
            case '3':
				um.forgetPassword();
				break;
            case '4':
				quitAnimation();
				break;
            default:
                printf("没有该选项！\n");
                _getch();
				clearScreen();
                break;
        }
    }

    while(true) {
        BaseUser* currentUser = um.login();

        if(currentUser != nullptr) {
            currentUser->handleInput(sm);
            delete currentUser;
        }
    }

    return 0;
}