with open("chicago_oblique_basic.chr", 'rb') as F0:
    contents_CHR = F0.read()
    F0.close()

with open("output.bin", 'r+b') as F1:
    F1.seek(0)
    F1.write(contents_CHR)
    F1.close()