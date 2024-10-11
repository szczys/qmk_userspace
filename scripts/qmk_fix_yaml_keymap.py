#!/usr/bin/python3

keymap_yaml = 'szczys_corne_mx.yaml'

import sys

layernames = [
    'QWERTY',
    'LOWER',
    'RAISE',
    'NUMPAD',
    'FUNC',
    ]

combos = [
    [3, 4, "'+'"],
    [13, 14, "'~'"],
    [15, 16, "'-'"],
    [16, 17, "'_'"],
    [27, 28, "'='"],
    [1, 10, 'Sft+ PRINTSCREEN'],
    [25, 10, 'PRINTSCREEN'],
    [0, 12, '{t: numpad, h: toggle}'],
]

combo_pattern = "combos:\n"

def get_key_pattern(key_name):
    return f'    - {key_name}\n'

def get_inline_key_pattern(key_name):
    return f', {key_name},'

def get_combo_string(combo_list):
    return f'- p: [{combo_list[0]}, {combo_list[1]}]\n  k: {combo_list[2]}\n'

def get_LT_string(s):
    # avoid complex mapping
    if s.count('(') != 1 or s.count(')') != 1:
        return s
    start = s.index("LT(")
    start_inner = start + len("LT(")
    end = s.index(")")
    inner = s[start_inner:end].strip()
    tokens = inner.split(',')

    if "'LT(" in s:
        # Account for LT wrapped in single quotes
        start = start - len("'")
        end = end + len("'")
    return s[:start] + f'{{t: {tokens[1]}, h: {tokens[0]} }}' + s[end+1:]

layer_indices = [f'  L{x}:\n' for x in range(len(layernames))]
layer_patterns = [get_key_pattern(x) for x in layernames]
layer_pattern_inline = [get_inline_key_pattern(x) for x in layernames]

def has_inline_pattern(line):
    for i,e in enumerate(layer_pattern_inline):
        if e in line:
            return i
    return None

def read_keymap(infile=keymap_yaml):
    with open(infile, 'r') as f:
        keymap = f.readlines()
    return keymap

def print_replacement(type: str, replacement: str):
    r_formatted = f'\t{replacement.rstrip()}'
    print(f'Replaced {type}:')
    print(r_formatted)

def update_and_write_keymap(keymap: list, outfile=keymap_yaml):
    with open(keymap_yaml, 'w') as f:
        for line in keymap:
            replacement = None

            if line == combo_pattern:
                '''
                All combos are added by this script so don't
                copy them if the script is re-run
                '''
                break

            if line in layer_indices:
                idx = layer_indices.index(line)
                replacement = f'  {layernames[idx]}:\n'
                print_replacement('layer name', replacement)

            elif line in layer_patterns:
                idx = layer_patterns.index(line)
                replacement = '    - { type: held }\n'
                print_replacement('layer key', replacement)

            elif 'LT(' in line:
                replacement = get_LT_string(line)
                print_replacement('layer tap macro', replacement)

            elif 'h: L0' in line:
                replacement = line.replace("h: L0", "h: i3")
                print_replacement('psuedo-layer name', replacement)

            else:
                idx = has_inline_pattern(line)
                if idx:
                    replacement = line.replace(f', {layernames[idx]},', ', { type: held },')
                    print_replacement('inline layer key', replacement)

            if replacement:
                f.write(replacement)
            else:
                f.write(line)
        if len(combos) > 0:
            print("Adding combo pattern:")
            print(combo_pattern)
            f.write(combo_pattern)
            for c in combos:
                combo = get_combo_string(c)
                print("Adding combo:")
                print(combo)
                f.write(combo)

def main() -> int:
    print(f'### Reading {keymap_yaml}')
    km = read_keymap()
    print(f'### Writing {keymap_yaml}')
    update_and_write_keymap(km)
    print('### YAML processing complete')
    return 0

if __name__ == '__main__':
    sys.exit(main())  # next section explains the use of sys.exit
