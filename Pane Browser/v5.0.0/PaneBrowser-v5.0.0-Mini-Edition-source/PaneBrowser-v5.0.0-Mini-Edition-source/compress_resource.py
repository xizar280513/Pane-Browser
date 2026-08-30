#!/usr/bin/env python3
from pathlib import Path
import struct
import sys

def compress(data: bytes) -> bytes:
    n = len(data)
    out = bytearray(struct.pack('<I', n))
    table = {}
    anchor = 0
    i = 0

    def emit(lit: bytes, offset: int, match_len: int) -> None:
        lit_len = len(lit)
        token = (min(lit_len, 15) << 4) | min(max(match_len - 4, 0), 15)
        out.append(token)
        if lit_len >= 15:
            extra = lit_len - 15
            while extra >= 255:
                out.append(255); extra -= 255
            out.append(extra)
        out.extend(lit)
        if offset:
            out.extend(struct.pack('<H', offset))
            extra = match_len - 4 - 15
            if match_len - 4 >= 15:
                while extra >= 255:
                    out.append(255); extra -= 255
                out.append(extra)

    while i + 4 <= n:
        key = data[i:i + 4]
        previous = table.get(key)
        table[key] = i
        if previous is None or i - previous > 65535:
            i += 1
            continue
        if data[previous:previous + 4] != key:
            i += 1
            continue
        match = 4
        while i + match < n and data[previous + match] == data[i + match]:
            match += 1
        emit(data[anchor:i], i - previous, match)
        end = i + match
        for p in range(i + 1, min(end, n - 3)):
            table[data[p:p + 4]] = p
        i = end
        anchor = i

    literal = data[anchor:]
    lit_len = len(literal)
    token = min(lit_len, 15) << 4
    out.append(token)
    if lit_len >= 15:
        extra = lit_len - 15
        while extra >= 255:
            out.append(255); extra -= 255
        out.append(extra)
    out.extend(literal)
    return bytes(out)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        raise SystemExit('usage: compress_resource.py input output')
    source = Path(sys.argv[1]).read_bytes()
    Path(sys.argv[2]).write_bytes(compress(source))
    print(f'{len(source)} -> {len(compress(source))} bytes')
