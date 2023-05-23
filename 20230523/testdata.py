import csv
import random
from numpy.random import choice

#IDstartingNumber = 10000000
#NumberOfSubject = 6
#MaxApply = 10
#numofdept = 10

def GenerateData(number, dept, IDstartingNumber=10000000,
                               NumberOfSubject = 6,
                               MaxApply = 10):
    StudentApp =  open('StudentApplicationPyTEST.csv', 'w', newline='')
    with open('StudentGradePyTEST.csv', 'w', newline='') as StudentGrade:
        writerGrade = csv.writer(StudentGrade)
        writeApp = csv.writer(StudentApp)
        #'ID,Chinese,English,MathA,MathB,Society,Nature'
        writerGrade.writerow(['ID','Chinese','English','MathA','MathB','Society','Nature'])

        grade = [i for i in range(0,16)]
        weight = [0.03, 10.5, 14.5, 10.0, 12.0, 8.5, 4.0, 4.0, 6.0, 8.0, 7.5, 9.8, 16.3, 16.0, 18.0, 18.0]
        nums = random.sample(range(IDstartingNumber, IDstartingNumber*2-1), number)
        nums = sorted(nums)
        for i in nums:
            row = []
            row.append(i)
            for j in range(NumberOfSubject):
                if j == 3 or j == 5:
                    row.append('null')
                else:
                    row.append(random.choices(grade,
                                             weights = weight,
                                             k=1)[0])
                    #row.append(random.randint(1,15))
            writerGrade.writerow(row)

            row = []
            row.append(i)
            row.extend(random.sample(range(1, dept+1),MaxApply))
            writeApp.writerow(row)
#GenerateData(10, numofdept)
GenerateData(10, 10)