const tmp = require('tmp-promise')
const { promises: fs } = require('fs')
const { join } = require('path')
const child_process = require('child_process')

const VM_BIN = join(__dirname, '..', '..', 'corewar')

const OP = {
    LIVE: { code: 0x01, cycles: 10 },
    LD: { code: 0x02, cycles: 5 },
    ST: { code: 0x03, cycles: 5 },
    ADD: { code: 0x04, cycles: 10 },
    SUB: { code: 0x05, cycles: 10 },
    AND: { code: 0x06, cycles: 6 },
    OR: { code: 0x07, cycles: 6 },
    XOR: { code: 0x08, cycles: 6 },
    ZJMP: { code: 0x09, cycles: 20 },
    LDI: { code: 0x0a, cycles: 25 },
    STI: { code: 0x0b, cycles: 25 },
    FORK: { code: 0x0c, cycles: 800 },
    LLD: { code: 0x0d, cycles: 10 },
    LLDI: { code: 0x0e, cycles: 50 },
    LFORK: { code: 0x0f, cycles: 1000 },
    AFF: { code: 0x10, cycles: 2 }
};

function ocp(...types) {
    if (types.length > 4)
        throw new Error("Too much arguments");
    let ocp = 0;
    for (const [i, type] of types.entries())
    {
        switch (type){
            case "DIRECT":
                ocp |= 0b10 << ((3 - i) * 2);
                break;
            case "INDIRECT":
                ocp |= 0b11 << ((3 - i) * 2);
                break;
            case "REGISTER":
                ocp |= 0b01 << ((3 - i) * 2);
                break;
            default:
                throw new Error(`Invalid type "${type}"`);
        }
    }
    return ocp;
}

function execVm(args) {
    return new Promise((resolve, reject) => {
        child_process.exec(`${VM_BIN} ${args}`, (err, stdout, stderr) => {
            if (err) reject(err)
            else {
                if (stderr) reject(new Error(stderr));
                else {
                    resolve(new Uint8Array(stdout
                        .split('\n')
                        .filter(l => l.length > 0)
                        .map(l => l.slice(l.indexOf(":") + 1).trim().split(" ").map(n => parseInt(n, 16)))
                        .reduce((c,v) => c.concat(v), [])))
                }
            }
        })
    })
}

const NAME_LENGTH = 128
const NAME_PADDING = 4
const COMMENT_LENGTH = 2048
const COMMENT_PADDING = 4
const MAGIC = [0x00, 0xea, 0x83, 0xf3] // Magic
const NAME = Array.from({ length: NAME_LENGTH + NAME_PADDING }).fill(0)
const COMMENT = Array.from({ length: COMMENT_LENGTH + COMMENT_PADDING }).fill(0)

async function runVm(memory, dumpAt) {
    const file = await tmp.file()
    if (!memory.every(e => typeof e === 'number'))
        throw new Error("Invalid memory not all elem are numbers")

    const length = new Uint8Array(4)
    new DataView(length.buffer).setUint32(0, memory.length);
    await fs.writeFile(file.path, new Uint8Array([...MAGIC, ...NAME, ...Array.from(length), ...COMMENT, ...memory]));
    try {
        return await execVm(`-d ${dumpAt} ${file.path}`)
    } finally {
        file.cleanup();
    }
}

function dumpReg(code, reg) {
    const offset = code.length;
    return {
        code: code.concat([OP.ST.code, ocp("REGISTER", "INDIRECT"), reg, 0, 0]),
        readRegValue(dump) {
            return new DataView(dump.buffer).getUint32(offset);
        },
        cycles: OP.ST.cycles
    }
}

module.exports = {
    OP,
    ocp,
    runVm,
    dumpReg
};