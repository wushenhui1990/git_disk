# -*- coding: utf-8 -*-

import os,sys
def hex_str(hex_file):
    out_file = hex_file + '_out'
    f_in = open(hex_file, 'r')
    f_out = open(out_file, 'w')
    f_in.seek(0, 2)
    size = f_in.tell()
    size_left = size
    block_size = 3 * 32
    f_in.seek(0, 0)
    while size_left > 0:
        if size_left >= block_size:
            hex_str = f_in.read(block_size)
            size_left -= block_size
        else:
            hex_str = f_in.read(size_left)
            size_left = 0
        byte = bytearray.fromhex(hex_str)
        #print(byte.decode('utf-8', errors='ignore'))
        #print(hex_str)
        f_out.write(hex_str)
        for i in byte:
            if i == ord('\r'):
                #print("\\r",end = " ")
                f_out.write('\\r')
            elif i == ord('\n'):
                #print("\\n",end = " ")
                f_out.write('\\n')
            else:
                #print(chr(i),end = "  ")
                f_out.write(chr(i))
        #print('\n')
        f_out.write('\n')


    f_in.close()
    f_out.close()

if __name__ == '__main__':
    hex_str(sys.argv[1])
