# Thank you to joshlory for this file

import struct
import sys
from enum import IntEnum
from pathlib import Path

VRAM = 0x40010000

class RelocType(IntEnum):
    R_MIPS_NONE  = 0
    R_MIPS_16    = 1
    R_MIPS_32    = 2
    R_MIPS_REL32 = 3
    R_MIPS_26    = 4
    R_MIPS_HI16  = 5
    R_MIPS_LO16  = 6

def sign_extend16(value):
    sign_bit = 1 << (16 - 1)
    return (value & (sign_bit - 1)) - (value & sign_bit)

def read_uint32(file):
    return struct.unpack('<I', file.read(4))[0]

def read_uint8(file):
    return struct.unpack('B', file.read(1))[0]

def read_xffEntPntHdr(file):
    file.seek(0)
    hdr = {}
    hdr['ident'] = file.read(4)
    file.seek(0xc)
    hdr['sec_syms'] = read_uint32(file)
    file.seek(0x14)
    hdr['fileSize'] = read_uint32(file)
    file.seek(0x1C)
    hdr['impSymIxsNrE'] = read_uint32(file)
    hdr['impSymIxs'] = read_uint32(file)
    hdr['symTabNrE'] = read_uint32(file)
    hdr['symTab'] = read_uint32(file)
    hdr['symTabStr'] = read_uint32(file)
    hdr['sectTab'] = read_uint32(file)
    file.seek(0x38)
    hdr['relocTabNrE'] = read_uint32(file)
    hdr['relocTab'] = read_uint32(file)
    hdr['sectNrE'] = read_uint32(file)
    file.seek(0x50)
    hdr['impSymIxs_Rel'] = read_uint32(file)
    hdr['symTab_Rel'] = read_uint32(file)
    hdr['symTabStr_Rel'] = read_uint32(file)
    hdr['sectTab_Rel'] = read_uint32(file)
    file.seek(0x64)
    hdr['relocTab_Rel'] = read_uint32(file)
    hdr['ssNamesOffs_Rel'] = read_uint32(file)
    hdr['ssNamesBase_Rel'] = read_uint32(file)
    return hdr

def read_xffRelocEnt(file, offset):
    file.seek(offset)
    entry = {}
    entry['type'] = read_uint32(file)
    entry['nrEnt'] = read_uint32(file)
    entry['sect'] = read_uint32(file)
    entry['addr'] = read_uint32(file)
    entry['inst'] = read_uint32(file)
    entry['addr_Rel'] = read_uint32(file)
    entry['inst_Rel'] = read_uint32(file)
    return entry

def read_xffRelocAddrEnt(file, offset):
    file.seek(offset)
    addr = read_uint32(file)
    tyIx = read_uint32(file)
    return addr, tyIx & 0xFF, tyIx >> 8

def read_xffRelocAddendEnt(file, offset):
    file.seek(offset)
    addend = read_uint32(file)
    unk = read_uint32(file)
    return addend, unk

def read_xffSymEnt(file, offset):
    file.seek(offset)
    entry = {}
    entry['nameOffs'] = read_uint32(file)
    entry['addr'] = read_uint32(file)
    entry['size'] = read_uint32(file)
    info = read_uint8(file)
    entry['type'] = info & 0xF
    entry['bindAttr'] = info >> 4
    entry['unk0D'] = read_uint8(file)
    entry['sect'] = struct.unpack('<H', file.read(2))[0]
    return entry

def read_xffSectEnt(file, offset):
    file.seek(offset)
    entry = {}
    entry['memPt'] = read_uint32(file)
    entry['filePt'] = read_uint32(file)
    entry['size'] = read_uint32(file)
    entry['align'] = read_uint32(file)
    entry['type'] = read_uint32(file)
    entry['flags'] = read_uint32(file)
    entry['moved'] = read_uint32(file)
    entry['offs_Rel'] = read_uint32(file)
    return entry

def read_string(file, offset):
    file.seek(offset)
    result = bytearray()
    while True:
        char = file.read(1)
        if char == b'\0':
            return result.decode('utf-8')
        result.extend(char)

def get_symbol_info(file, sym_table, sym_str_table, sym_idx, section_names):
    sym_offset = sym_table + sym_idx * 0x10
    sym = read_xffSymEnt(file, sym_offset)
    name = read_string(file, sym_str_table + sym['nameOffs'])
    return name if name else section_names[sym['sect']], sym['sect'], sym['addr'], sym['type']

def format_reloc_output(addr, relType, sym_name, text_offs_rel):
    rom_addr = text_offs_rel + addr
    reloc_type = {
        2: "MIPS_32",
        4: "MIPS_26",
        5: "MIPS_HI16",
        6: "MIPS_LO16"
    }.get(relType)
    
    _sym_name = sym_name
    addend = "0x0"
    if "+" in sym_name:
        off = sym_name.find("+")
        _sym_name = sym_name[:off]
        addend = sym_name[off:]
    elif "-" in sym_name:
        off = sym_name.find("-")
        _sym_name = sym_name[:off]
        addend = sym_name[off:]
    
    if relType in [2, 4, 5, 6]:
        if addend == "0x0":
            return f"rom:0x{rom_addr:X} symbol:{_sym_name} reloc:{reloc_type}"
        else:
            return f"rom:0x{rom_addr:X} symbol:{_sym_name} reloc:{reloc_type} addend:{addend}"
    else:
        return None

def sort_rel_addr(e):
    return e['addr']


bss_start = 0


def extract_reloc_and_symbols(filename: Path):
    xff_name = filename.stem.lower()
    relocs_output_file = Path("config") / f"{xff_name}/{xff_name}.relocs.txt"
    symbols_output_file = Path("config") / f"{xff_name}/{xff_name}.symbols.txt"

    with open(filename, 'rb') as file, open(relocs_output_file, 'w') as relocs_out, open(symbols_output_file, 'w') as symbols_out:
        hdr = read_xffEntPntHdr(file)

        if hdr['ident'] != b'xff2':
            print("Not a valid XFF2 file")
            return


        # Extract section headers
        section_names = {}
        section_offsets = {}
        text_offs_rel = None
        for i in range(hdr['sectNrE']):
            sect_offset = hdr['sectTab_Rel'] + i * 0x20
            sect = read_xffSectEnt(file, sect_offset)
            if sect['type'] == 8:
                sect['offs_Rel'] = (hdr['fileSize'] + 63) & ~63
                bss_start = sect['offs_Rel']
            file.seek(hdr['ssNamesOffs_Rel'] + i * 4)
            name_offset = read_uint32(file)
            section_names[i] = read_string(file, hdr['ssNamesBase_Rel'] + name_offset)
            section_offsets[i] = sect['offs_Rel']

            if section_names[i] == '.text':
                text_offs_rel = sect['offs_Rel']

        # Extract symbols and build symbol table
        symbol_table = {}
        symbol_secs = [0] * hdr['symTabNrE']
        
        for i in range(1, hdr['symTabNrE']):
            symbol_offset = hdr['symTab_Rel'] + i * 0x10
            sym = read_xffSymEnt(file, symbol_offset)
            name = read_string(file, hdr['symTabStr_Rel'] + sym['nameOffs'])
            name = name if name else f"UNKNOWN_{i}"

            # Calculate the actual address by adding the section offset
            actual_address = sym['addr'] + section_offsets.get(sym['sect'], 0)

            symbol_secs[i] = sym['sect']
            # Skip absolute section symbols as those belong to the main elf
            if sym['sect'] == 0xfff1 and sym['bindAttr'] != 0:
                pass
            
            elif sym['sect'] != 0 and sym['bindAttr'] != 0:
                symbols_out.write(f"{name} = 0x{(VRAM + actual_address):X};\n")# // size:{sym['size']}\n")

                # Add symbol to the symbol table
                symbol_table[VRAM + actual_address] = name

        # Not all relocs resolve to a symbol, so we make a generic symbol in those instances
        # these generic symbols need to also be in symbol_addrs so we collect them here
        noname_syms = dict()

        # Extract relocation tables
        for i in range(hdr['relocTabNrE']):
            reloc_entry_offset = hdr['relocTab_Rel'] + i * 0x1C
            reloc_entry = read_xffRelocEnt(file, reloc_entry_offset)
            addr_table_offset = reloc_entry['addr_Rel']
            inst_table_offset = reloc_entry['inst_Rel']
            
            # Preload all relocations for this table
            rel_table = []
            for j in range(reloc_entry['nrEnt']):
                rel = {}
                rel['addr'], rel['type'], rel['symIx'] = read_xffRelocAddrEnt(file, addr_table_offset + j * 8)
                # Get the addend for this relocation
                rel['addend'], _ = read_xffRelocAddendEnt(file, inst_table_offset + j * 8)
                # Get the section id for the relocation symbol
                rel['sec'] = symbol_secs[rel['symIx']]
                rel_table.append(rel)
            # Sort the table by address
            rel_table.sort(key=sort_rel_addr)
            

            for j in range(reloc_entry['nrEnt']):
                addr, relType, tgSymIx = rel_table[j]['addr'], rel_table[j]['type'], rel_table[j]['symIx']
                addend = rel_table[j]['addend']
                
                # Offset of this reloc section (mainly for R_MIPS_32 relocs, which aren't in .text)
                sect_offs = section_offsets[reloc_entry["sect"]]

                if relType == RelocType.R_MIPS_NONE:
                    # Do nothing
                    pass
                elif relType == RelocType.R_MIPS_32:
                    _sym_name, sym_sect, sym_addr, sym_type = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    _addr = VRAM + section_offsets[sym_sect] + sym_addr + addend
                    if sym_type == 3 or _sym_name not in symbol_table:
                        if _sym_name == ".text":
                            sym_name = symbol_table.get(_addr, f"func_{_addr:08X}")
                        else:
                            sym_name = symbol_table.get(_addr, f"D_{_addr:08X}")
                    # These aren't added to the 'noname_syms' table because most are going to be jump table entries
                        
                elif relType == RelocType.R_MIPS_26:
                    sym_name, _, sym_addr, __ = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    if tgSymIx == 1:
                        func_offset = (addend & 0x3FFFFFF) * 4
                        func_addr = VRAM + text_offs_rel + func_offset
                        sym_name = symbol_table.get(func_addr, f"func_{func_addr:08X}")
                        _addr = func_addr
                        
                        if func_addr not in symbol_table:
                            noname_syms[_addr] = sym_name
                    if sym_name.startswith("."):
                        print("WRONG MIPS26!")

                elif relType == RelocType.R_MIPS_HI16:
                    _sym_name, sym_sect, sym_addr, sym_type = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    lo_i = search_reloc_lo(rel_table, j)
                    loAddend = rel_table[lo_i]['addend']
                    
                    # hi/lo pairs
                    hi = (addend << 0x10) & 0xFFFF0000
                    lo = sign_extend16(loAddend)
                    
                    # If the symbol already has a name, then we don't need to create one
                    if not _sym_name.startswith("."):
                        sym_name = _sym_name

                    # No symbol, create one
                    else:
                        _addr = VRAM + sym_addr + hi + lo + section_offsets[sym_sect]
                        _addr &= 0xFFFFFFFF

                        if sym_type == 2:
                            sym_name = symbol_table.get(_addr, f"func_{_addr:08X}")
                        elif sym_type == 3:
                            if _sym_name == ".text":
                                sym_name = symbol_table.get(_addr, f"func_{_addr:08X}")
                            else:
                                sym_name = symbol_table.get(_addr, f"D_{_addr:08X}")
                        else:
                            sym_name = symbol_table.get(_addr, f"D_{_addr:08X}")
                        
                        if _addr not in symbol_table and _addr not in noname_syms:
                            noname_syms[_addr] = sym_name

                elif relType == RelocType.R_MIPS_LO16:
                    _sym_name, sym_sect, sym_addr, sym_type = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    hi_i = search_reloc_hi(rel_table, j)
                    hiAddend = rel_table[hi_i]['addend']
                    
                    addr_hi, relType_hi, tgSymIx_hi = rel_table[hi_i]['addr'], rel_table[hi_i]['type'], rel_table[hi_i]['symIx']
                    _sym_name_hi, sym_sect_hi, sym_addr_hi, sym_type_hi = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx_hi, section_names)
                    
                    _addr  = (VRAM + sign_extend16(addend) + sym_addr) + (hiAddend << 16)
                    s_off = 0
                    if _sym_name.startswith(".") or sym_sect < 0xfff1:
                        s_off = section_offsets[sym_sect]
                    
                    _addr += s_off
                    _addr &= 0xFFFFFFFF
                    
                    addend = 0
                    
                    # Symbol already has a name
                    if not _sym_name.startswith("."):
                        sym_name = _sym_name
                        s_off_hi = 0
                        if sym_sect_hi < 0xfff1:
                            s_off_hi = section_offsets[sym_sect_hi]
                        _addr_hi = (VRAM + s_off_hi + sym_addr_hi) & 0xFFFFFFFF
                        addend = _addr - _addr_hi
                    
                    # No symbol, create one
                    else:
                        if sym_sect != sym_sect_hi:
                            s_off_hi = 0
                            if sym_sect_hi < 0xfff1:
                                s_off_hi = section_offsets[sym_sect_hi]
                            _addr_hi = (VRAM + s_off_hi + sym_addr_hi) & 0xFFFFFFFF
                            addend = _addr - _addr_hi
                        
                        if _addr in symbol_table:
                            sym_name = symbol_table[_addr]
                        elif sym_type == 2:
                            sym_name = f"func_{_addr:08X}"
                        elif sym_type == 3:
                            if _sym_name == ".text":
                                sym_name = f"func_{_addr:08X}"
                            else:
                                sym_name = f"D_{_addr:08X}"
                        else:
                            sym_name = f"D_{_addr:08X}"
                        
                        
                        if addend == 0 and _addr not in symbol_table and _addr not in noname_syms:
                            noname_syms[_addr] = sym_name
                    
                    # Some relocations are offset into a symbol
                    if addend != 0:
                        if addend < 0:
                            addend = abs(addend)
                            sym_name = sym_name + f"-0x{addend:X}"
                        else:
                            sym_name = sym_name + f"+0x{addend:X}"

                else:
                    print(f"xff_parser: Unknown reloc type ({relType}) found at 0x{addr:04X}")
                    sys.exit(-1)
                
                formatted_output = format_reloc_output(addr, relType, sym_name, sect_offs)
                if formatted_output:
                    relocs_out.write(f"{formatted_output}\n")
            
        # Save all found noname_syms
        for k,v in noname_syms.items():
            symbols_out.write(f"{v} = 0x{k:X};\n")



def search_reloc_lo(rel_table, index: int):
    reg = (rel_table[index]['addend'] >> 16) & 0x1f
    sec = rel_table[index]['sec']
    sym = rel_table[index]['symIx']
    check_reg = True
    check_sym = True
    
    # Tries to find the corresponding lo reloc in 3 ways
    # 1: Looks for an identical match; symbol/section and register
    # 2: Tries just checking the symbol/section
    # 3: Tries just checking the register
    for t in range(3):
        for i in range(index+1, len(rel_table)):
            rel = rel_table[i]
            if rel['sec'] == sec and rel['type'] == RelocType.R_MIPS_LO16:
                if not check_sym or rel['symIx'] == sym:
                    if not check_reg or (rel['addend'] >> 21) & 0x1f == reg:
                        return i
        check_reg = t & 1 == 1
        check_sym = t == 2
    print(f"xff_parser: Can't find LO16 pair for HI16 ({index})")
    return 0

def search_reloc_hi(rel_table, index: int):
    reg = (rel_table[index]['addend'] >> 21) & 0x1f
    sec = rel_table[index]['sec']
    sym = rel_table[index]['symIx']
    check_reg = True
    check_sym = True
    
    # Tries to find the corresponding hi reloc in 3 ways
    # 1: Looks for an identical match; symbol/section and register
    # 2: Tries just checking the symbol/section
    # 3: Tries just checking the register
    for t in range(3):
        i = index-1
        while i >= 0:
            rel = rel_table[i]
            if rel['type'] == RelocType.R_MIPS_HI16 and rel['addend'] & 0xfc000000 == 0x3c000000 and rel['sec'] == sec:
                if not check_sym or rel['symIx'] == sym:
                    if not check_reg or (rel['addend'] >> 16) & 0x1f == reg:
                        return i
            i -= 1
        check_reg = t & 1 == 1
        check_sym = t == 2
    print(f"xff_parser: Can't find HI16 pair for LO16 ({index})")
    return 0



def main(filename):
    filename = Path(filename) # kept getting errors about 'no attribute .exists in object str' so i just moved the Path creation here
    if not filename.exists() and filename.is_file():
        print(f"File not found: {filename}")
        sys.exit(-1)

    if not filename.is_file():
        print(f"Path supplied is a folder: {filename}")
        sys.exit(-1)
    
    extract_reloc_and_symbols(filename)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    main(sys.argv[1])
