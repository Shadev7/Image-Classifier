## a helper python script


import os
folder = "test"

for child in os.listdir(folder):
    test_path = os.path.join(folder, child)
    if os.path.isdir(test_path):
        cmd = "./overfeat/bin/linux_64/overfeat_batch -i %s/%s -o %s-feature/%s"%(folder, child, folder, child)
        os.system(cmd)

