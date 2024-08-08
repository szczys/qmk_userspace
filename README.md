# Corne QMK Mapping by Mike Szczys

Corne split keyboard mapping driven by [Frood RP2040 Pro Micro
Controllers](https://42keebs.eu/shop/parts/controllers/frood-rp2040-pro-micro-controller/).

![Szczys Corne keymap](./img/szczys_corne_mx.svg)

Keymap originally based on the Corne layout [by Mark
Stosberg](https://mark.stosberg.com/markstos-corne-3x5-1-keyboard-layout/).

## Compile

```
qmk compile -kb crkbd/rev1 -km szczys_corne_mx -e CONVERT_TO=promicro_rp2040
```

## Visualize keymap

Generate an SVG using [keymap
drawer](https://github.com/caksoylar/keymap-drawer)

### Prerequisite: Install keymap drawer

```
pip install keymap-drawer
```

Alternatively you may use [the web app](https://keymap-drawer.streamlit.app/).

### Generate the visualization

```
qmk c2json -kb crkbd/rev1 -km szczys_corne_mx -o szczys_corne_mx.json
keymap parse -c 10 -q szczys_corne_mx.json > szczys_corne_mx.yaml
./scripts/qmk_fix_yaml_keymap.py
keymap -c scripts/keymap_drawer_config.yaml draw szczys_corne_mx.yaml > img/szczys_corne_mx.svg
```

The `qmk_fix_yaml_keymap.py` is a hacky way of post-processing the yaml to
achieve the following results:
- Give the layers names
- Highlight the "held" key for each layer in pink
- Convert the `LT()` macros to tap/hold assignments (not sure why keymap-drawer
  is failing to parse these)
- Add combos

Layer names and all combo assignments must be manually added to this python
file when changes to those aspects are made on the keymap.
