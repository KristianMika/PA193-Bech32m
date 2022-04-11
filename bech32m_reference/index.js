const bech32 = require('bech32-buffer');

const argv = process.argv;
var mode = process.argv[2]

if (mode == 'encode') {
    const hrp = argv[3]
    let hex = argv[4]
    hex = Uint8Array.from(Buffer.from(hex, 'hex'));
    console.log(bech32.encode(hrp, hex, 'bech32m'));
} else if (mode == 'decode') {
    bech = argv[3]
    const plain = bech32.decode(bech, 'bech32m');
    console.log(`${plain.prefix} ${Buffer.from(plain.data).toString('hex')}`);
} else {
    console.log("Usage: \n\tnode . encode [hrp] [hex]\n\tnode . decode [bech32m]")
}
