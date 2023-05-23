import matplotlib.pyplot as plt
import numpy as np
import csv

def GenerateRatio(numofdept = 10):
    StudentApp =  open('StudentApplicationPyTEST.csv', 'r', newline='')
    StudentGrade =  open('StudentGradePyTEST.csv', 'r', newline='')

    label = [i for i in range(1,numofdept+1)]
    plt.pie(num,
            radius=1.5,
            labels=label,
            autopct='%.1f%%')
    plt.show()

GenerateRatio(10)