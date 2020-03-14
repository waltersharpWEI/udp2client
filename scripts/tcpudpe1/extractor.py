import argparse

parser = argparse.ArgumentParser(description='Data extractor')
parser.add_argument('path', metavar='P', type=str, nargs='+',
                    help='path to the raw log.')


def extract_last_line(path=""):
    with open(path,'r') as f1:
        lines = f1.readlines()
        if len(lines) > 5:
            last_line = lines[-5]
        else:
            last_line = "0,0"
    return last_line

if __name__=="__main__":
    args = parser.parse_args()
    line = extract_last_line(args.path[0])
    print(line)
