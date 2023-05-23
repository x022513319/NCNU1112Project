import matplotlib.pyplot as plt
import numpy as np
import csv

def GenerateRatio():#numofdept = 10):
    StudentApp =  open('StudentApplicationPyTEST.csv', 'r', newline='')
    StudentGrade =  open('StudentGradePyTEST.csv', 'r', newline='')

    #label = [i for i in range(1,numofdept+1)]
    #plt.pie(num,
    #        radius=1.5,
    #        labels=label,
    #        autopct='%.1f%%')
    #plt.show()

    sum = [0 for i in range(4)]
    readGrade = csv.reader(StudentGrade)
    n = 0
    for row in readGrade:
        if n == 0:
            n = n + 1
            continue 
        tmp = int(row[1]) + int(row[2]) + int(row[3]) + int(row[5])
        if tmp > 45:
            sum[3] += 1
        elif tmp > 30 and tmp <= 45:
            sum[2] += 1
        elif tmp > 15 and tmp <= 30:
            sum[1] += 1
        elif tmp <= 15:
            sum[0] += 1
    label = ['0~15', '16~30', '31~45', '46~60']
    plt.pie(sum,
            radius=1.3,
            labels=label,
            textprops={'weight':'bold', 'size':20},
            autopct='%.1f%%',
            wedgeprops={'linewidth':3,'edgecolor':'w'}
            )
    plt.show()
    #print(sum)

