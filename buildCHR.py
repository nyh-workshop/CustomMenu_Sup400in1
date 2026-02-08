import argparse

parser = argparse.ArgumentParser(description='Add font into the ROM for the custom SUP 400-in-1 Menu.')
parser.add_argument('file_path', type=str, help='CHR file')

args = parser.parse_args()

with open(args.file_path, 'rb') as F0:
    contents_CHR = F0.read()
    F0.close()

with open("output.bin", 'r+b') as F1:
    F1.seek(0)
    F1.write(contents_CHR)
    F1.close()