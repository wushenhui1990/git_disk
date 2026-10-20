import sys
import os
import wave
import re

def write_data_to_file(file_path, data, sample_width, sample_rate):
    #data_24 = array.array('B')
    #index = 1
    #print("data count:", int(len(data) / 4), len(data))
    #for i in range(int(len(data) / 4)):
    #    data_24 = data_24 + data[index: index + 3]
    #    print(i, range(int(len(data) / 4)), end="\r")
    #    index = index + 4

    with wave.open(file_path, 'w') as wf:
        wf.setnchannels(2)
        wf.setsampwidth(sample_width)
        wf.setframerate(sample_rate)
        wf.writeframes(data)
        wf.close()
        print("write ", file_path, len(data))

def pcm_to_wav(src_path, out_dir):
    if "FS" not in src_path:
        print("not fs args")
        return
    if "16" not in src_path and "24" not in src_path:
        print("not sample width args")
        return
    if "pcm" not in src_path:
        print("file unknown")
        return

    file_name = os.path.basename(src_path)
    rate_str = re.sub(r'[^_]*_FS([^_]*)_([^._]*).*.pcm', r'\1', file_name)
    width_str = re.sub(r'[^_]*_FS([^_]*)_([^._]*).*.pcm', r'\2', file_name)
    out_file_name = file_name.replace("pcm", "wav")
    dst_path = out_dir + "/" + out_file_name
    print("rate str:", rate_str, "width str:", width_str, "dst_path:", dst_path)
    
    rate = int(rate_str)
    if rate > 100:
        rate = rate * 100
    else:
        rate = rate * 1000
    width = int(int(width_str) / 8)
    
    out_dir = os.path.dirname(dst_path)
    if os.path.exists(out_dir) == False:
        os.mkdir(out_dir)
    print("rate :", rate, "width :", width, "dst_path:", dst_path)
    with open(src_path, 'rb') as f_src:
        data = f_src.read()
        write_data_to_file(dst_path, data, width, rate)


def pcm_dir_to_wav(pcm_dir):
    pcm_files = os.listdir(pcm_dir)
    out_dir = pcm_dir + "_wavs"
    for f in pcm_files:
        pcm_to_wav(pcm_dir + "/" + f, out_dir)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("args :dir or pcm file")
        exit()

    arg1 = str(sys.argv[1])
    if os.path.exists(arg1) == False:
        print("path not exist:", arg1)

    if os.path.isfile(arg1):
        dir_name = os.path.dirname(arg1)
        pcm_to_wav(arg1, dir_name + "_wavs")
    elif os.path.isdir(arg1):
        pcm_dir_to_wav(os.path.dirname(arg1))
