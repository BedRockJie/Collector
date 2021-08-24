import os

path = "./yuyiliziqiu"

fileList = os.listdir(path)
print(fileList)

n = 0
for i in fileList:
    oldname = path + os.sep + fileList[n]
    # print(oldname)
    newname = path + os.sep + "img" + str(n+606) + ".jpg"
    # print(newname)
    os.rename(oldname,newname)
    print(oldname ,">>>>>>>>>>",newname)
    n += 1

    if n == 30:
        break
print("file num is %d"%n)