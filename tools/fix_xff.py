import struct
import os
import sys


def read_uint32(file):
    return struct.unpack('<I', file.read(4))[0]

def read_uint8(file):
    return struct.unpack('B', file.read(1))[0]


def fix_xff_reallocs(filename):
    with open(filename, 'r+b') as file:
        file.seek(0)
        magic = file.read(4)
        if magic == b'xff2' or magic == b'xff\0':
            # get section offsets
            file.seek(64)
            sec_count = read_uint32(file)
            file.seek(84)
            syms_off = read_uint32(file)
            file.seek(92)
            secs_offset = read_uint32(file)
            sec_offsets = []
            bss_sec = 0
            for i in range(sec_count):
                file.seek(secs_offset + i * 32 + 16)
                type = read_uint32(file)
                if type == 8:
                    bss_sec = i
                file.seek(secs_offset + i * 32 + 28)
                sec_offsets.append(read_uint32(file))
            #print(f"  Sections: {sec_count}")
            #print(bss_sec)
            
            #go through the relocations headers
            file.seek(56)
            rel_head_count = read_uint32(file)
            file.seek(100)
            rel_heads_offset = read_uint32(file)
            file.seek(rel_heads_offset)
            for i in range(rel_head_count):
                offset = rel_heads_offset + i * 28
                file.seek(offset + 4)
                count = read_uint32(file)
                section = read_uint32(file)
                file.seek(offset + 20)
                addr_offset = read_uint32(file)
                file.seek(offset + 24)
                data_offset = read_uint32(file)
                
                for j in range(count):
                    file.seek(addr_offset + j * 8)
                    rel_off = read_uint32(file)
                    info = read_uint32(file)
                    rel_type = info & 0xff
                    rel_sym = info >> 8
                    file.seek(data_offset + j * 8)
                    rel_dat = read_uint32(file)
                    data = struct.pack("I", rel_dat)
                    addr = sec_offsets[section] + rel_off
                    file.seek(syms_off + (rel_sym * 16) + 14)
                    sym_sec = read_uint8(file) | (read_uint8(file) << 8)
                    #print(sym_sec)
                    #if sym_sec == bss_sec:
                        #print(f"S:{section}, O:{rel_off:X}, A:{addr:X} D:{rel_dat:X}")
                    file.seek(addr)
                    file.write(data)



def main(filename):
    if os.path.exists(filename):
        fix_xff_reallocs(filename)
    else:
        print(f"File not found: {filename}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    main(sys.argv[1])

