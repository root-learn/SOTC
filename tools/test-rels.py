import struct
import sys
from pathlib import Path

VRAM = 0x40000000




def read_uint32(file):
    return struct.unpack('<I', file.read(4))[0]

def read_uint8(file):
    return struct.unpack('B', file.read(1))[0]

def read_uint16(file):
    return read_uint8(file) | (read_uint8(file) << 8)

def read_string(file, offset):
    file.seek(offset)
    result = bytearray()
    while True:
        char = file.read(1)
        if char == b'\0':
            return result.decode('utf-8')
        result.extend(char)


def test_rels(rels_filename: Path, data_filename: Path):
    with open(rels_filename, 'r') as frels, open(data_filename, 'rb') as fdata:
        symbol_addrs = {}
        rsymbol_addrs = {}
        fdata.seek(12)
        sec_sym_amount = read_uint32(fdata)
        fdata.seek(36)
        symbol_amount = read_uint32(fdata)
        fdata.seek(84)
        syms_off = read_uint32(fdata)
        sym_names = read_uint32(fdata)
        for i in range(sec_sym_amount+1, symbol_amount):
            fdata.seek(syms_off + i * 16)
            name_off = read_uint32(fdata)
            addr = read_uint32(fdata)
            name = read_string(fdata, sym_names + name_off)
            symbol_addrs[addr] = name
            rsymbol_addrs[name] = addr
            #print(f"{i}: name:{name} addr:{addr:0x}")
        
        
        line_i = 1
        for rline in frels:
            args = rline.split(' ')
            cmds = {}
            for a in args:
                a = a.split(':')
                cmds[a[0]] = a[1].strip()
            cmds['rom'] = int(cmds['rom'], 16)
            if cmds['symbol'].startswith("D_") or cmds['symbol'].startswith("func_"):
                cmds['symbol'] = int(cmds['symbol'].split('_')[1], 16)
            if 'addend' in cmds:
                cmds['addend'] = int(cmds['addend'], 16)
            
            fdata.seek(cmds['rom'])
            val = read_uint32(fdata)
            do_print = False
            
            match cmds['reloc']:
                case 'MIPS_HI16':
                    val = val & 0xffff
                    symbol = cmds['symbol']
                    if type(symbol) == str:
                        symbol = rsymbol_addrs[symbol]
                    
                    if symbol & 0xffff > 0x7fff:
                        symbol += 0x10000
                    
                    if symbol >> 16 != val:
                        do_print = True
                case 'MIPS_LO16':
                    val = val & 0xffff
                    symbol = cmds['symbol']
                    if type(symbol) == str:
                        symbol = rsymbol_addrs[symbol]
                    if 'addend' in cmds:
                        symbol += cmds['addend']
                    
                    if symbol & 0xffff != val:
                        do_print = True
                    
                case 'MIPS_26':
                    val = ((val & 0x3ffffff) << 2) + VRAM
                    val = symbol_addrs.get(val, val)
                    
                    if cmds['symbol'] != val:
                        do_print
            
            if do_print:
                s = f"{line_i}: rom:0x{cmds['rom']:0x}"
                if type(cmds['symbol']) == str:
                    s += f" symbol:{cmds['symbol']}"
                else:
                    s += f" symbol:0x{cmds['symbol']:0x}"
                if 'addend' in cmds:
                    s += f" addend:{cmds['addend']}"
                if type(val) == str:
                    s += f" - val:{val}"
                else:
                    s += f" - val:0x{val:0x}"
                print(s)
            
            line_i += 1
            





def main(rels_filename, data_filename):
    rels_filename = Path(rels_filename)
    if not rels_filename.exists() and rels_filename.is_file():
        print(f"File not found: {rels_filename}")
        sys.exit(-1)
    if not rels_filename.is_file():
        print(f"Path supplied is a folder: {rels_filename}")
        sys.exit(-1)
    
    data_filename = Path(data_filename)
    if not data_filename.exists() and data_filename.is_file():
        print(f"File not found: {data_filename}")
        sys.exit(-1)
    if not data_filename.is_file():
        print(f"Path supplied is a folder: {data_filename}")
        sys.exit(-1)
    
    
    test_rels(rels_filename, data_filename)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python script.py <rels_filename> <data_filename>")
        sys.exit(1)

    main(sys.argv[1], sys.argv[2])
