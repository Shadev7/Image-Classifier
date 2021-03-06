
import os

categores = ["bagel","bread","brownie","chickennugget",
                           "churro","croissant","frenchfries","hamburger",
                           "hotdog","jambalaya","kungpaochicken","lasagna",
                           "muffin","paella","pizza","popcorn","pudding",
                           "salad","salmon","scone","spaghetti","sushi",
                           "taco","tiramisu","waffle"]

category_dict = dict(zip(categores, range(len(categores))))

def normalize_folder(folder):
    target = "%s.data"%folder
    target_f = open(target, "w")
    for category in os.listdir(folder):
        test_path = os.path.join(folder, category)
        if os.path.isdir(test_path):
            for feature_file in os.listdir(test_path):
                feature_path = os.path.join(test_path, feature_file)
                with open(feature_path, "r") as f:
                    t = f.readlines()
                    if len(t) < 2:
                        print "feature_path:%s"%feature_path
                        continue
                    s = t[1].rstrip()
                    line = " ".join(["%s:%s"%(index+1, value) for index, value in enumerate(s.split())])
                    target_f.write(str(category_dict[category]+1) + " " + line + "\n")
    target_f.close()

## for predict
def normalize_file(image_path, file_path):
    from make_overfeat import check_size, iserror, resize
    category = image_path.split("/")[1]
    if not os.path.isfile(file_path) or iserror(file_path):
        width, height = check_size(image_path)
        if width != 231 or height != 231:
            image_path = resize(image_path)
        cmd = "./overfeat/bin/linux_64/overfeat -f %s > %s"%(image_path, file_path)
        os.system("mkdir -p %s"%os.path.split(file_path)[0])
        # print cmd
        os.system(cmd)
    target = ".temp.data"
    target_f = open(target, "w")
    with open(file_path, "r") as f:
        t = f.readlines()
        s = t[1].rstrip()
        line = " ".join(["%s:%s"%(index+1, value) for index, value in enumerate(s.split())])
        target_f.write(str(category_dict[category]+1) + " " + line + "\n")
    target_f.close()
    return target

def predict(image_path):
    split_image_path = image_path.split("/")
    des_file = os.path.join("deep", "test-feature", *split_image_path[1:])+".features"
    des_file = normalize_file(image_path, des_file)
    cmd = "./svm_multiclass/svm_multiclass_classify %s deep/c46.model .temp > .info"%des_file
    # print cmd
    os.system(cmd)
    with open(".temp") as f:
        index = f.read(2)
        return index 

if __name__ == "__main__":
    import sys
    arg = sys.argv[1]
    if os.path.isdir(arg):
        arg = arg.rstrip("/")
        normalize_folder(arg)
    else:
        print predict(arg)




