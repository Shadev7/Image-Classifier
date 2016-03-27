## a helper python script

import re
import os
import subprocess
folder = "test"


## http://stackoverflow.com/a/10759761/847357
def check_size(file_path):
    path = file_path
    dim = subprocess.Popen(["identify","-format","\"%w,%h\"",path], stdout=subprocess.PIPE).communicate()[0]
    (width, height) = [ int(x) for x in re.sub('[\t\r\n"]', '', dim).split(',') ]
    return (width, height)

def iserror(thefile):
    with open(thefile, "r") as f:
        t = f.readlines()
        if len(t) < 2:
            return True
    return False

def resize(file_path, width, height):

    ratio = 250 / float(min(width, height))
    width = int(width * ratio) + 1
    height = int(height * ratio) + 1

    cmd = "convert %s -resize %sx%s %s"%(file_path, width, height,file_path)
    print cmd
    os.system(cmd)

for category in os.listdir(folder):
    test_path = os.path.join(folder, category)
    if os.path.isdir(test_path):
        for image in os.listdir(test_path):
            image_path = os.path.join(test_path, image)
            featurename = image_path+".features"
            des = "deep/%s-feature/%s"%(folder, featurename[5:])
            new_folder = "deep/%s-feature/%s"%(folder,category)
            os.system("mkdir -p %s"%new_folder)
            if not os.path.isfile(des) or thefile(des):
                width, height = check_size(image_path)
                if  width < 231 or height < 231:
                    resize(image_path, width, height)
                cmd = "./overfeat/bin/linux_64/overfeat -f %s > %s"%(image_path, des)
                print "cmd:%s"%cmd
                os.system(cmd)

