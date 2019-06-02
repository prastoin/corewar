const { OP, runVm, ocp, dumpReg, dumpCarry, direct } = require("../utils/vm")

describe("and", () => {
    test("basic", async () => {
        const {
            code: cCode,
            readCarry,
            cycles: cCycles,
        } = dumpCarry([
            OP.AND.code, ocp("DIRECT", "DIRECT", "REGISTER"), ...direct(42), ...direct(36), 1,
        ])

        const {
            code,
            readRegValue,
            cycles
        } = dumpReg(cCode, 1);

        const dump = await runVm(code, OP.AND.cycles + cycles + cCycles);
        expect(readCarry(dump)).toBe(false);
        expect(readRegValue(dump)).toBe(42 & 36);
    })
    test("carry up", async () => {
        const {
            code: cCode,
            readCarry,
            cycles: cCycles,
        } = dumpCarry([
            OP.AND.code, ocp("DIRECT", "DIRECT", "REGISTER"), ...direct(0), ...direct(0), 1,
        ])

        const {
            code,
            readRegValue,
            cycles
        } = dumpReg(cCode, 1);

        const dump = await runVm(code, OP.AND.cycles + cycles + cCycles);
        expect(readCarry(dump)).toBe(true);
        expect(readRegValue(dump)).toBe(0 & 0);
    })
})