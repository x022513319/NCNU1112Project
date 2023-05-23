
#include "infoDS.h"
//#include "department.h"

using namespace std;
bool comp(int a, int b);
void Apply(std::unordered_map<int, student*> _Mstu, std::unordered_map<int, department*> _Mdept);
void SortForDept(std::unordered_map<int, department*>);

unordered_map<int, student*> studentMap;
//std::unordered_map<int, student*>  <准考證號, 學生> 
unordered_map<int, department*> departmentMap;
//std::unordered_map<int, department*> <志願系所編號, 系所>

int main(){
    fstream inGrade("StudentGrade.csv"); //學生成績
    fstream inApply("StudentApplication.csv"); //學生志願表
    string bufGrade, bufApply, temp;
    int dotPosition = 0;
    student stutemp;

    inGrade >> bufGrade; //過濾掉第一列(准考證號、國文、...)
    while(inGrade >> bufGrade){
        inApply >> bufApply;
        student* stu = new student();

        stu->RawDataGrade = bufGrade; //尚未處理逗號的准考證號、成績原始資料
        stu->RawDataApply = bufApply; //尚未處理逗號的志願序

        stu->Standarize(); // 將准考證號、科目成績放進"student"的class
        studentMap[stu->id] = stu; // 用hash map 用准考證號對應到一個學生的成績、志願序 (ID -> grade, application)
    }

    //共有幾個系所就建立幾個dept
    for(int i = 1; i <= DEPTNUMBER; i++){
        department *dept = new department();
        dept->deptID = i;
        departmentMap[i] = dept; // 共有DEPTNUMBER個系所
        // 用hash map 用系所編號對應到一個系所
        // 1號 -> XX系, 2號 -> OO系, ..., 5號 -> OXO系
    }

    //scan所有學生的志願序，按照志願序上的系所編號紀錄是哪位學生有申請該系所
    //例如： 編號10105555學生申請了 2、5、3號系所，則分別在2、5、3號系所中記下該學生的id，以便之後排序學生順位所用
    for(unordered_map<int, student*>::iterator it = studentMap.begin(); it != studentMap.end(); it++){
        for(int j = 0; j < MAXAPPLY; j++){
            if(it->second->application[j] == -1) break;
            else
                departmentMap[it->second->application[j]]->studentApplication.push_back(it->second->id);
        }
    }

    //排序系所對學生的順位表 (暫用4科加起來的成績做排序(國英數A社))
    SortForDept(departmentMap);
    //for(unordered_map<int, department*>::iterator iter = departmentMap.begin(); iter != departmentMap.end(); iter++){
    //    sort(iter->second->studentApplication.begin(), iter->second->studentApplication.end(), comp);
    //}
    
    //配對演算法
    Apply(studentMap, departmentMap);

    //輸出檔案, 照准考證號排序
    ofstream Result("Result.csv");
    vector<pair<int,student*>> v;
    v.assign(studentMap.begin(), studentMap.end());
    sort(v.begin(), v.end(), //准考證號由小到大排序
         [](const pair<int,student*> a, pair<int, student*> b){ //lambda
            return a.second->id < b.second->id;
         }
        );
    for(vector<pair<int, student*>>::iterator it = v.begin(); it != v.end(); it++){
        Result << it->second->id;
        if(it->second->admissionCount > 0){
            Result << ",";
            int temp = 0;
            for(int i = 0; i < it->second->admissionCount; i++){
                for(int j = temp; j < KVALUE; j++){
                    if(it->second->admission[j] != 0){
                        Result << it->second->admission[j];
                        temp = j+1;
                        break;
                    }
                }
                if(i != it->second->admissionCount - 1)
                    Result << ",";
            }
        }
        Result << endl;
    }


#ifdef DEBUG
    
    cout << "#of students: " << studentList.size() << endl;
    for(list<student>::iterator iter = studentList.begin(); iter != studentList.end(); iter++){
        cout << "id: "      << iter->id <<
                " Chinese: " << iter->Chinese <<
                " English: " << iter->English <<
                " MathA: "   << iter->MathA <<
                " MathB: "   << iter->MathB <<
                " Social: "  << iter->Social <<
                " Nature: "  << iter->Nature << endl;
                cout << "App: ";
        for(int j = 0; j < MAXAPPLY; j++){
            if(iter->application[j] == -1) break;
            cout << iter->application[j] << " ";
        }
        cout << endl;
    }
#endif

#ifdef DEBUGSTUMAP
    //印出每個學生的基本資料(ID、成績、志願序)
    cout << "unordered studentMap:" << endl;
    for(unordered_map<int, student*>::iterator iter = studentMap.begin(); iter != studentMap.end(); iter++){
        cout << "id: "      << iter->second->id <<
                " Chinese: " << iter->second->Chinese <<
                " English: " << iter->second->English <<
                " MathA: "   << iter->second->MathA <<
                " MathB: "   << iter->second->MathB <<
                " Social: "  << iter->second->Social <<
                " Nature: "  << iter->second->Nature << endl;
                cout << "App: ";
        for(int j = 0; j < MAXAPPLY; j++){
            if(iter->second->application[j] == -1) break;
            cout << iter->second->application[j] << " ";
        }
        cout << endl;
    }
#endif

#ifdef DEBUGDEPTMAP
    //印出每個系所申請學生的ID和國文成績
    cout << "unordered deptMap:" << endl;
    for(unordered_map<int, department*>::iterator it = departmentMap.begin(); it != departmentMap.end(); it++){
        cout << "department " << it->first << endl;
        cout << "studentID: " << endl;
        for(auto j = 0; j < it->second->studentApplication.size(); j++){
            cout << it->second->studentApplication[j];
            cout << " Chinese: " <<studentMap[it->second->studentApplication[j]]->Chinese << endl;
        }
        cout << endl;
    }
    /*
    //每個學生的志願序原始資料
    for(unordered_map<int, student*>::iterator it = studentMap.begin(); it != studentMap.end(); it++){
        for(int j = 1; j < MAXAPPLY; j++){
            cout << it->second->application[j] << " ";
        }
        cout << endl;
    }
    */
#endif

#ifdef DEBUGCOMP
    cout << "order:" << endl;
    for(auto i = departmentMap.begin(); i != departmentMap.end(); i++){
        cout << "dept: " << i->second->deptID << endl;
        cout << "id: ";
        for(auto j = 0; j < i->second->studentApplication.size(); j++){
            cout << i->second->studentApplication[j] << " ";
        }
        cout << endl;
        cout << "total score: " << endl;
        for(auto j = 0; j < i->second->studentApplication.size(); j++){
            cout << studentMap[i->second->studentApplication[j]]->id << ": "; 
            cout << studentMap[i->second->studentApplication[j]]->Chinese +
                    studentMap[i->second->studentApplication[j]]->English +
                    studentMap[i->second->studentApplication[j]]->MathA +
                    studentMap[i->second->studentApplication[j]]->Social << endl;
        }
        cout << endl;
    }
#endif

#ifdef DEBUGRESULT
    for(unordered_map<int, department*>::iterator iterDept = departmentMap.begin(); iterDept != departmentMap.end(); iterDept++){
        cout << "department " << iterDept->second->deptID << ":" << endl;
        for(int i = 0; i < QUOTA; i++){
            if(iterDept->second->studentAccepted[i] != 0){
                cout << iterDept->second->studentAccepted[i] << " ";
            }
        }
        cout << endl;
    }

    cout << endl;
    for(unordered_map<int, student*>::iterator iterStu = studentMap.begin(); iterStu != studentMap.end(); iterStu++){
        cout << "student " << iterStu->second->id << ":" << endl;
        for(int i = 0; i < KVALUE; i++){
            if(iterStu->second->admission[i] != 0){
                cout << iterStu->second->admission[i] << " ";
            }
        }
        cout << endl;
    }
#endif

#ifdef DEBUGDEPTAPP
    for(unordered_map<int, department*>::iterator iter = departmentMap.begin(); iter != departmentMap.end(); iter++){
        cout << "department: " << iter->second->deptID << ": ";
        for(auto f  = 0; f < iter->second->studentApplication.size(); f++){
            cout << iter->second->studentApplication[f] << ", ";
        }
        cout << endl;
    }
#endif

}

//處理Raw data
void student::Standarize(){
    int dotPositionGrade = 0, dotPositionApply = 0;
    int i = 0;
    string tempGrade, tempApply;
    int bufGrade, bufApply;
    while(1){
        tempGrade = RawDataGrade.substr(dotPositionGrade, RawDataGrade.find(",", dotPositionGrade) - dotPositionGrade);
        //cout << "compare: " << compare(temp, "null") << endl;
        if(tempGrade != "null")
            bufGrade = stoi(tempGrade);
        switch(i){
            case 0:
                this->id = bufGrade;
            case 1:
                this->Chinese = bufGrade;
                break;
            case 2:
                this->English = bufGrade;
                break;
            case 3:
                if(tempGrade != "null") this->MathA = bufGrade;
                else this->MathA = 0;
                break;
            case 4:
                if(tempGrade != "null") this->MathB = bufGrade;
                else this->MathB = 0;
                break;
            case 5:
                if(tempGrade != "null") this->Social = bufGrade;
                else this->Social = 0;
                break;
            case 6:
                if(tempGrade != "null") this->Nature = bufGrade;
                else this->Nature = 0;
                break;
            default:
                break;
        }
        if(RawDataGrade.find(",", dotPositionGrade) == string::npos) break;
        dotPositionGrade = RawDataGrade.find(",", dotPositionGrade) + 1;
        i++;
    }

    i = 0;
    int flag = 0;
    while(1){
        tempApply = RawDataApply.substr(dotPositionApply, RawDataApply.find(",", dotPositionApply) - dotPositionApply);
        //cout << "compare: " << compare(temp, "null") << endl;
        if(flag == 0){
            dotPositionApply = RawDataApply.find(",", dotPositionApply) + 1;
            flag = 1;
            continue;
        }
        if(tempApply == "0") break;
        else
            bufApply = stoi(tempApply);
        application[i] = bufApply;
        //Apply(bufApply);
        if(RawDataApply.find(",", dotPositionApply) == string::npos){
            //application[i+1] = bufApply;
            break;
        }
        dotPositionApply = RawDataApply.find(",", dotPositionApply) + 1;
        i++;
    }

}

int student::compare(std::string a, std::string b){
    return a == b;
}

//演算法排序
//輸入：student, department //輸出：vector<pair<int, int[KVALUE]>> int代表准考證號 int[KVALUE]代表錄取的學校
void Apply(std::unordered_map<int, student*> studentMap, std::unordered_map<int, department*> departmentMap){
    //vector<pair<int, int[KVALUE]>> v;
    int tempStu;
    int tempApp;
    int isSuccess;
   // unordered_map<int, student*>::iterator iterStu = studentMap.begin();
   // unordered_map<int, department*>::iterator iterDept = departmentMap.begin();
    for(int i = 0; i < MAXAPPLY; i++){ // //i = 0為學生准考證號
        for(unordered_map<int, student*>::iterator iterStu = studentMap.begin(); iterStu != studentMap.end(); iterStu++){
            if(iterStu->second->admissionCount < KVALUE && iterStu->second->application[i] != -1){
                tempStu = iterStu->second->id;
                tempApp = iterStu->second->application[i];
                isSuccess = departmentMap[tempApp]->CheckApp(tempStu);
#ifdef DEBUGSTUORDER
cout << "ID:" << tempStu << ": ";
for(int xx = 0; xx < KVALUE; xx++){
    cout << studentMap[tempStu]->admission[xx] << ", "; 
}
cout << endl;
#endif
            }
        }
    }
}

int department::CheckApp(int stuID){
    int j, k, correct;
    vector<int>::iterator iter = this->studentApplication.begin(),
                          NewStu = find(this->studentApplication.begin(), this->studentApplication.end(), stuID),
                          temp, lowest;
    if(this->studentAcceptedCount < QUOTA){
        for(int i = 0; i < QUOTA; i++){
            if(this->studentAccepted[i] == 0){
                this->studentAccepted[i] = stuID;
                this->studentAcceptedCount++;
                studentMap[stuID]->admissionCount++;
                for(int j = 0; j < KVALUE; j++){
                    if(studentMap[stuID]->admission[j] == 0){
                        studentMap[stuID]->admission[j] = this->deptID;
#ifdef DEBUGORDER
for(int xx = 0; xx < KVALUE; xx++){
    cout << studentMap[stuID]->admission[xx] << ", "; 
}
cout << endl;
#endif
                        return 1;
                    }
                }
            }
        }
    }
    else{
        int vv;
        //check在新申請的學生有沒有比studentAccepted裡的任一學生優秀，若有則換，若無則不換
        for(int i = 0; i < QUOTA; i++){

            temp = find(this->studentApplication.begin(), this->studentApplication.end(), this->studentAccepted[i]);
            for(vv = 0; vv < QUOTA; vv++){
                lowest = find(this->studentApplication.begin(), this->studentApplication.end(), this->studentAccepted[vv]);
                if(lowest > temp){
                    cout << "lowest: " << *lowest << endl;
                    cout << "temp: " << *temp << endl;
                    temp = lowest;
                    correct = vv;
                    //cout << "temp changed: " << *temp << endl;
                }
            }

            //temp = find(this->studentApplication.begin(), this->studentApplication.end(), this->studentAccepted[i]);
            if(NewStu < temp){
#ifdef DEBUGSTUORDER
    cout << "changed!" << " old:" << *temp << " new: " << stuID << endl;
#endif


                 studentMap[this->studentAccepted[correct]]->admissionCount--;
                for(int j = 0; j < MAXAPPLY; j++){
                    if(studentMap[this->studentAccepted[correct]]->admission[j] == this->deptID)
                        studentMap[this->studentAccepted[correct]]->admission[j] = 0;
                }

                this->studentAccepted[correct] = stuID;
                studentMap[stuID]->admissionCount++;
                for(int j = 0; j < KVALUE; j++){
                    if(studentMap[stuID]->admission[j] == 0){
                        studentMap[stuID]->admission[j] = this->deptID;
#ifdef DEBUGORDER
for(int xx = 0; xx < KVALUE; xx++){
    cout << studentMap[stuID]->admission[xx] << ", "; 
}
cout << endl;
#endif
                        return 1;  
                    }
                }
            }
        }
    }
}

bool comp(int a, int b){ //比較 國 英 數A 社 加起來總級分

    int x, y;
    x = studentMap[a]->Chinese + studentMap[a]->English + studentMap[a]->MathA + studentMap[a]->Social;
    y = studentMap[b]->Chinese + studentMap[b]->English + studentMap[b]->MathA + studentMap[b]->Social;
    return x > y; //由大排到小
}

void SortForDept(std::unordered_map<int, department*> departmentMap){
    for(unordered_map<int, department*>::iterator iter = departmentMap.begin(); iter != departmentMap.end(); iter++){
        sort(iter->second->studentApplication.begin(), iter->second->studentApplication.end(), comp);
    }
}