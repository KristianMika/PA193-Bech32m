# Bech32m
[![Default CI](https://github.com/KristianMika/PA193-Bech32m/actions/workflows/default.yaml/badge.svg)](https://github.com/KristianMika/PA193-Bech32m/actions/workflows/default.yaml)
<a href="https://scan.coverity.com/projects/kristianmika-pa193-bech32m">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/24784/badge.svg"/>
</a>
## Build
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

# Usage

## Help
```bash
bech32m --help
```

## Encoding

With input text:
```bash
bech32m --input-text [program-input]
```
Inputs will be read from stdin
```bash
bech32m
```

Inputs will be read from file
```bash
bech32m --input-file [file-name]
```

Changing the input format
```bash
bech32m --input-format [hex/bin/base64]
```

Sending output to the file
```bash
bech32m --output-file [file-name]
```

## Decoding
With input read from stdin
```bash
bech32m --decode --output-format [hex/bin/base64]
```

With input as an argument
```bash
bech32m --decode --output-format [hex/bin/base64] --input-text [input]
```

With input from file
```bash
bech32m --decode --output-format [hex/bin/base64] --input-file [input-file]
```

Setting both input and output files
```bash
bech32m --decode --output-format [hex/bin/base64] --input-file [input-file] --output-file [output-file]
```


## Other flags

Trimming the trailing bits instead of padding them
```bash
bech32m --decode --output-format [hex/bin/base64] --trim

bech32m --decode --input-text a1424qmnnf92 --output-format hex
aaaa0

bech32m --decode --input-text a1424qmnnf92 --output-format hex --trim
aaaa
```

Allowing empty hrp
```bash
bech32m --allow-empty-hrp
```

Setting default hrp
```bash
bech32m --hrp [default-hrp]
```
