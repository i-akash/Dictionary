import csv
import re;

with open("dictionary.csv") as infs:
    csvfile=csv.reader(infs)
    with open("dictionary.txt",'w') as outfs:
        for line in csvfile:
            length=len(line)
            for i in range(length):
                word=line[i]
                if i==0:
                    word=re.sub('[^a-zA-Z]','',word)
                    word=word.lower()

                outfs.write(word)
                if i<length-1:
                    outfs.write("$")
            outfs.write("\n")

