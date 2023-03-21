#define MAXAPPLY 30
#define KVALUE 4
#define QUOTA 1 //系所錄取
#define DEPTNUMBER 5
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>


class department{
public:
    int deptID; //系所代號
    int deptQuota; //系所錄取名額
    int studentAccepted[QUOTA]; //錄取名單
    int studentAcceptedCount; //現在錄取了多少人
    std::vector<int> studentApplication; //紀錄哪些學生有申請本系所

    department(){
        deptID = 0;
        deptQuota = 0;
        studentAcceptedCount = 0;
        for(int i = 0; i < QUOTA; i++) studentAccepted[i] = 0;
    }

    int CheckApp(int StuID); //確認studentAccepted是否滿了以及是否該申請學生好過studentAccepted裡的任一一人
};

class student{
public:    
    friend department;
    std::string RawDataGrade;
    std::string RawDataApply;
    int id;
    int Chinese, English, MathA, MathB, Social, Nature;
    int application[MAXAPPLY]; // 志願表
    int admission[KVALUE]; // 被哪些學校錄取
    int admissionCount; // 被幾間學校錄取
    //int admissionPtr;

    student(){
        id = 0;
        Chinese = 0; English = 0; MathA = 0; MathB = 0; Nature = 0;
        admissionCount = 0;
        for(int i = 0; i < MAXAPPLY; i++) application[i] = -1;
        for(int j = 0; j < KVALUE; j++) admission[j] = 0;
    };
    void Standarize(); // 處理讀進來的資料 (去掉逗號、輸入成績、志願序等等)
    //void initialize();
    int compare(std::string a, std::string b);
};


