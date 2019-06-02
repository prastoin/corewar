const { OP, runVm, ocp, dumpReg, dumpCarry, direct } = require("../utils/vm")

function sub(a, b, value = a - b) {
    return async () => {
        const {
            code: cCode,
            readCarry,
            cycles: cCycles
        } = dumpCarry([
            OP.LD.code, ocp("DIRECT", "REGISTER"), ...direct(a), 1,
            OP.LD.code, ocp("DIRECT", "REGISTER"), ...direct(b), 2,
            OP.SUB.code, ocp("REGISTER", "REGISTER", "REGISTER"), 1, 2, 3,
        ])

        const {
            code,
            readRegValue,
            cycles
        } = dumpReg(cCode, 3);

        const dump = await runVm(code, OP.LD.cycles * 2 + OP.ADD.cycles + cCycles + cycles);
        expect(readCarry(dump)).toBe(value === 0);
        expect(readRegValue(dump)).toBe(value);
    }
}

describe("sub", () => {
    test("basic", sub(42, 36));
    test("negative argument", sub(42, -36));
    test("negative result", sub(-42, 36));
    test("int max", sub(2 ** 31 - 2, 1));
    test("int max overflow", sub(2 ** 31 - 1, 1, 2 ** 31 - 2));
    test("int min", sub(-(2 ** 31), 0, -(2 ** 31)));
    test("int min underflow", sub(-(2 ** 31), -1, -(2 ** 31) + 1));
    test("zero", sub(0, 0));
    test("zero2", sub(-2, 2));
    test("zero3", sub(2 ** 31 - 1, -(2 ** 31) + 1, -2));
})