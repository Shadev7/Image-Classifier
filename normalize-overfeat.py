
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


def normalize_file(file_path):
    pass

if __name__ == "__main__":
    import sys
    arg = sys.argv[1]
    if os.path.isdir(arg):
        arg = arg.rstrip("/")
        normalize_folder(arg)





