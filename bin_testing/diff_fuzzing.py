import base64
import binascii
import os
import subprocess
import random
import sys

BECH_SYMBOLS = "qpzry9x8gf2tvdw0s3jn54khce6mua7l"

OUR_BINARY = None
LIBBECH32ENC_BINARY = None
LIBBECH32DEC_BINARY = None
NODE_REF = "node . "


# region Encoding

def node_encode(hrp, data_hex):
    str_in = NODE_REF + f"encode {hrp} {data_hex}"
    proc = subprocess.Popen(str_in, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    err = proc.stderr.read().decode(encoding='ASCII').strip()
    if err != '':
        print("*******")
        print("Node error:\n" + err)
        with open("fuzzing_results.txt", "a") as f:
            f.write("*******\n")
            f.write(f"HRP: {hrp}\n")
            f.write(f"HEX: {data_hex}\n")
            f.write("Node error:\n" + err + "\n")
            f.write("*******\n")
    return proc.stdout.read().decode(encoding='ASCII').strip(), err != ''


def external_encode(hrp, bech):
    indexes = get_indexes(bech)
    indexes_str = indexes_to_string(indexes)
    str_in = f"{LIBBECH32ENC_BINARY} {hrp} {indexes_str}"
    proc = subprocess.Popen(str_in, stdout=subprocess.PIPE)
    proc.wait()
    return proc.stdout.read().decode(encoding='ASCII').strip()


def hex_encode(hrp, data_hex):
    proc = subprocess.Popen(f"{OUR_BINARY} --input-text {data_hex} --input-format hex --hrp {hrp}",
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    err = proc.stderr.read().decode(encoding='ASCII').strip()
    if err != '':
        print("*******")
        print("OUR error:\n" + err)
        with open("fuzzing_results.txt", "a") as f:
            f.write("*******\n")
            f.write(f"HRP: {hrp}\n")
            f.write(f"HEX: {data_hex}\n")
            f.write("OUR error:\n" + err + "\n")
            f.write("*******\n")
    return proc.stdout.read().decode(encoding='ASCII').strip()


def base64_encode(hrp, data_base64):
    proc = subprocess.Popen(f"{OUR_BINARY} --input-text {data_base64} --input-format base64 --hrp {hrp}",
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    err = proc.stderr.read().decode(encoding='ASCII').strip()
    if err != '':
        print("*******")
        print("OUR error:\n" + err)
        with open("fuzzing_results.txt", "a") as f:
            f.write("*******\n")
            f.write(f"HRP: {hrp}\n")
            f.write(f"B64: {data_base64}\n")
            f.write("OUR error:\n" + err + "\n")
            f.write("*******\n")
    return proc.stdout.read().decode(encoding='ASCII').strip()


def bin_encode(hrp, data_hex):
    try:
        with open('b.bin', 'wb') as f:
            f.write(binascii.unhexlify(data_hex))

        proc = subprocess.Popen(f"{OUR_BINARY} --input-file b.bin --input-format bin --hrp {hrp}",
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        proc.wait()
        err = proc.stderr.read().decode(encoding='ASCII').strip()

        if err != '':
            print("*******")
            print("OUR error:\n" + err)
            with open("fuzzing_results.txt", "a") as f:
                f.write("*******\n")
                f.write(f"HRP: {hrp}\n")
                f.write(f"BIN: {data_hex} (as binary)\n")
                f.write("OUR error:\n" + err + "\n")
                f.write("*******\n")
        return proc.stdout.read().decode(encoding='ASCII').strip()
    finally:
        os.remove('b.bin')


# endregion


# region Decoding

def node_decode(code):
    str_in = NODE_REF + f"decode {code}"
    proc = subprocess.Popen(str_in, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    err = proc.stderr.read().decode(encoding='ASCII').strip()
    if err != '':
        print("*******")
        print("Node error:\n" + err)
        with open("fuzzing_results.txt", "a") as f:
            f.write("*******\n")
            f.write(f"CODE: {code}\n")
            f.write("Node error:\n" + err + "\n")
            f.write("*******\n")
    return proc.stdout.read().decode(encoding='ASCII').strip(), err != ''


def external_decode(code):
    str_in = f"{LIBBECH32DEC_BINARY} {code}"
    proc = subprocess.Popen(str_in, stdout=subprocess.PIPE)
    proc.wait()
    return proc.stdout.read().decode(encoding='ASCII').strip()


def hex_decode(code):
    proc = subprocess.Popen(f"{OUR_BINARY} --decode --input-text {code} --output-format hex --allow-empty-hrp",
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    err = proc.stderr.read().decode(encoding='ASCII').strip()
    if err != '':
        print("*******")
        print("OUR error:\n" + err)
        with open("fuzzing_results.txt", "a") as f:
            f.write("*******\n")
            f.write(f"CODE: {code}\n")
            f.write("OUR error:\n" + err + "\n")
            f.write("*******\n")
    return proc.stdout.read().decode(encoding='ASCII').strip()


# endregion

def generate_hrp():
    chars = [chr(x) for x in range(33, 127)]
    length = random.randint(1, 81)
    ret = "".join(random.choice(chars) for _ in range(length)).lower()
    return ret.replace("'", "").replace('"', '')


def generate_hex(hrp):
    max_len = 83 - len(hrp)
    chars = "0123456789abcdef"
    length = random.randint(2, max_len)
    if length % 2 == 1:
        length -= 1
    return "".join(random.choice(chars) for _ in range(length))


def to_base64(hex_str):
    return base64.b64encode(base64.b16decode(hex_str)).decode(encoding='utf-8')


# Adapted from
# https://stackoverflow.com/questions/1425493/convert-hex-to-binary
def to_bin(hex_code):
    return bin(int(hex_code, 16))[2:]


def extract_bech(code):
    return code[code.rfind('1') + 1:-6]


def get_indexes(s):
    return [BECH_SYMBOLS.index(c) for c in s]


def indexes_to_string(indexes):
    return " ".join(str(i) for i in indexes)


def process(hrp, hex_str, base64_str):
    try:
        our_res = hex_encode(hrp, hex_str)
        our_res_64 = base64_encode(hrp, base64_str)
        our_res_bin = bin_encode(hrp, hex_str)
        node_res, node_enc_err = node_encode(hrp, hex_str)
        extract_our = extract_bech(our_res)
        external_res = external_encode(hrp, extract_our)

        dec_our = hex_decode(our_res)
        dec_ext = external_decode(our_res)
        _node_dec = node_decode(our_res)
        hrp, dec_node = _node_dec[0].split(' ')
        node_dec_err = _node_dec[1]

        if our_res_bin != our_res or \
                our_res_64 != our_res or \
                our_res != external_res or \
                (our_res != node_res and not node_enc_err):
            print("ERROR: Our ENCODED result does not match reference result:")
            print(f"HRP: {hrp}")
            print(f"HEX: {hex_str}")
            print(f"B64: {base64_str}")
            print(f"BIN: {to_bin(hex_str)}")
            print(f"  Our result:\t\t{our_res}")
            print(f"  Our result B64:\t{our_res_64}")
            print(f"  Our result BIN:\t{our_res_bin}")
            print(f"  External result:\t{external_res}")
            print(f"  Node result:\t\t{node_res}")
            with open("fuzzing_results.txt", "a") as f:
                f.write("ERROR: Our ENCODED result does not match reference result:\n")
                f.write(f"HRP: {hrp}\n")
                f.write(f"HEX: {hex_str}\n")
                f.write(f"B64: {base64_str}\n")
                f.write(f"BIN: {to_bin(hex_str)}\n")
                f.write(f"  Our result:\t\t{our_res}\n")
                f.write(f"  Our result B64:\t{our_res_64}\n")
                f.write(f"  Our result BIN:\t{our_res_bin}\n")
                f.write(f"  External result:\t{external_res}\n")
                f.write(f"  Node result:\t\t{node_res}\n")
                f.write("\n")
        if dec_ext not in extract_our or (dec_our != dec_node and not node_dec_err):
            print("ERROR: Our DECODED result does not match node result:")
            print(f"  Our result:\t\t{dec_our}")
            if dec_ext not in extract_our:
                print(f"  External result:\t{dec_ext} not in {extract_our}")
            print(f"  Node result:\t\t{dec_node}")
            with open("fuzzing_results.txt", "a") as f:
                f.write("ERROR: Our DECODED result does not match node result:\n")
                f.write(f"  Our result:\t\t{dec_our}\n")
                if dec_ext not in extract_our:
                    f.write(f"  External result: {dec_ext} not in {extract_our}\n")
                f.write(f"  Node result:\t\t{dec_node}\n")
                f.write("\n")
    except Exception as e:
        print(e)
        with open("fuzzing_results.txt", "a") as f:
            f.write(f"{hrp}\n")
            f.write(f"{e}\n")
            f.write("\n")


if __name__ == '__main__':
    OUR_BINARY = sys.argv[1]
    LIBBECH32ENC_BINARY = sys.argv[2]
    LIBBECH32DEC_BINARY = sys.argv[3]
    FUZZ_ITERATIONS = int(sys.argv[4])

    _hrp = 'v)zeod9[qg.ns)+}r}'
    _hex_str = '857e'
    _b64_str = to_base64(_hex_str.upper())

    process('a', 'ff', to_base64('FF'))

    for _ in range(0, FUZZ_ITERATIONS):
        process(_hrp, _hex_str, _b64_str)
        _hrp = generate_hrp()
        _hex_str = generate_hex(_hrp)
        _b64_str = to_base64(_hex_str.upper())

    print("DONE")
