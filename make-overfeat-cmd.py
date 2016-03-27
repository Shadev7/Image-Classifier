## a helper python script


import os
import subprocess
folder = "test"


## http://stackoverflow.com/a/10759761/847357
def check_size(file_path):
    path = file_path
    dim = subprocess.Popen(["identify","-format","\"%w,%h\"",path], stdout=subprocess.PIPE).communicate()[0]
    (width, height) = [ int(x) for x in re.sub('[\t\r\n"]', '', dim).split(',') ]
    return (width, height)

def resize(file_path):
    cmd = "convert %s -resize 231x231 %s"%(file_path, file_path)
    print cmd

for category in os.listdir(folder):
    test_path = os.path.join(folder, category)
    if os.path.isdir(test_path):
        for image in os.listdir(test_path):
            image_path = os.path.join(test_path, image)
            featurename = image_path+".features"
            des = "deep/%s-feature/%s"%(folder, featurename[5:])
            new_folder = "deep/%s-feature/%s"%(folder,category)
            os.system("mkdir -p %s"%new_folder)
            if not os.path.isfile(des):
                width, height = check_size(image_path)
                if  width < 231 and height < 231:
                    resize(image_path)
                cmd = "./overfeat/bin/linux_64/overfeat -f %s > %s"%(image_path, des)
                print "cmd:%s"%cmd
                os.system(cmd)

