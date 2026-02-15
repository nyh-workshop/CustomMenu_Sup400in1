# Simple python program to swap bits on the SUP Box handheld ROMs:

import argparse
from pathlib import Path

parser = argparse.ArgumentParser(description='Swap data bits on SUP 400-in-1 handhelds ROM dumps.')
parser.add_argument('file_path', type=str, help='Binary file path of ROM dump')
parser.add_argument('swap_back_to_original', nargs='?', type=str, help='Add \'O\' to restore bit-swapped ROMs back to original.')
parser.add_argument('bits_swap', type=str, help='Bit swaps. Example: SWAP_DATA_BITS_9A_TO_12 is swap bits 9,10 with 1,2')

args = parser.parse_args()

def swap_9A_to_12bits(i):
    i0 = i & 0x600
    i1 = i & 0x006
    ii = (i & ~0x606) | (i0 >> 8) | (i1 << 8)
    return ii

if(args.bits_swap == "SWAP_DATA_BITS_9A_TO_12"):    
    if(args.swap_back_to_original == 'O'):
         new_bin_file_path = f'{Path(args.file_path).stem}_O'
    else:
        new_bin_file_path = f'{Path(args.file_path).stem}_SWAP_DATA_BITS_9A_TO_12'
else:
    raise Exception("Invalid bit swap!")
    
new_bin_file = open(f'output_rom_handheld/{new_bin_file_path}.bin','wb')

with open(args.file_path,'rb') as bin_file:
    data = bin_file.read(2)
    new_data = swap_9A_to_12bits(int.from_bytes(data))
    new_bin_file.write((new_data).to_bytes(2))
    while(data):
        data = bin_file.read(2)
        new_data = swap_9A_to_12bits(int.from_bytes(data))
        new_bin_file.write((new_data).to_bytes(2))

new_bin_file.close()

if(args.swap_back_to_original == 'O'):
    print("Bit swaps restored to original successfully! :D")
else:
    print("Bit swaps successful! :D")
    
print(f"Created new bin file: {new_bin_file_path}")