const { OP, runVm, ocp, dumpReg, dumpCarry, direct } = require("../utils/vm")

function add(a, b, value = a + b) {
    return async () => {
        const {
            code: cCode,
            readCarry,
            cycles: cCycles
        } = dumpCarry([
            OP.LD.code, ocp("DIRECT", "REGISTER"), ...direct(a), 1,
            OP.LD.code, ocp("DIRECT", "REGISTER"), ...direct(b), 2,
            OP.ADD.code, ocp("REGISTER", "REGISTER", "REGISTER"), 1, 2, 3,
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

describe("add", () => {
    test("basic", add(42, 36));
    test("negative argument", add(42, -36));
    test("negative result", add(-42, 36));
    test("int max", add(2 ** 31 - 2, 1));
    test("int max overflow", add(2 ** 31 - 1, 1, -(2 ** 31)));
    test("int min", add(-(2 ** 31), 0, -(2 ** 31)));
    test("int min underflow", add(-(2 ** 31), -1, (2 ** 31) - 1));
    test("zero", add(0, 0));
    test("zero2", add(-2, 2));
    test("zero3", add(2 ** 31 - 1, -(2 ** 31) + 1));
})