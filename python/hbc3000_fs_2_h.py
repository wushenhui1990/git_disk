# -*- coding: utf-8 -*-
 


import os,sys


base = [str(x) for x in range(10)] + [ chr(x) for x in range(ord('A'),ord('A')+6)]


def bin2dec(string_num):
    '''二进制 to 十进制: int(str,n=10) '''
    return str(int(string_num, 2))


def dec2hex(string_num):
    '''十进制 to 十六进制'''
    num = int(string_num)
    if num == 0:
        return '0'
    mid = []
    while True:
        if num == 0: break
        num,rem = divmod(num, 16)
        mid.append(base[rem])

    return ''.join([str(x) for x in mid[::-1]])


def bin2hex(string_num):
    '''二进制 to 十六进制'''
    string_len = len(string_num)  
    res = dec2hex(bin2dec(string_num))
    return res.zfill(string_len/4)



 
if __name__ == '__main__':
    if len(sys.argv) > 1 and os.path.exists(sys.argv[1]):
        
        fs_data = ['0x47', '0x57', '0x31', '0x4E']
        
        with open(sys.argv[1]) as fr:
            for line in fr:
                line = line.strip()
                _line_hex = bin2hex(line)
                for i in range(0, len(_line_hex), 2):
         
                    
                    fs_data.append("0x{}".format(_line_hex[i:i+2]) )
                
                            
        with open("_fs.h", "w") as f:
            f.write(r"static unsigned char FS_chksum[2] = { %s, %s }; " % (fs_data[4], fs_data[5]))
            f.write("\n\n")
 
        
        with open("_fs.h", "a") as f:
            f.write(r"static unsigned char FS_data[][256] = {")
            f.write("\n")
 
 
            
            page_count = len(fs_data)/256 + 1
            for page_index in range(0, page_count):
 
                f.write("{\n")
   
                _page_data = fs_data[page_index*256 : page_index*256 + 256]
        
                if  not _page_data:
                    break
 
                if len(_page_data) < 256:
                    _page_data.extend(["0xFF"] * (256- len(_page_data)))
                
                for index, _byte in enumerate(_page_data):
 
                    if index > 0 and index%16 ==0: 
                        f.write("\n")
                        
                    f.write("{},".format(_byte))

                
                
                
                
          
                f.write("\n},\n\n")
       
             
                        
            f.write("\n\n};\n")
                
                
                
        print"""\nDone. """        
                
    else:
        print"""Usage: fs_2_h.exe  <bitStream file path>
        Programs converts FS data to "_fs.h".
        
        
        Version : 0.1 alpha
        Gowin Semicon LTD.  2017
               
        
        
        """