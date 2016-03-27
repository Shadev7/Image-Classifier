## a helper python script


import os
folder = "test"

for category in os.listdir(folder):
    test_path = os.path.join(folder, category)
    if os.path.isdir(test_path):
        for image in os.listdir(test_path):
            image_path = os.path.join(test_path, image)
            featurename = image_path+".features"
            if not os.path.isfile(featurename):
                cmd = "./overfeat/bin/linux_64/overfeat_batch -f %s > deep/%s-feature/%s"%(image_path, folder, featurename)
                print "cmd:%s"%cmd
            #os.system(cmd)

