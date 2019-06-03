const { OP, runVm, ocp, dumpReg, dumpCarry, direct } = require("../utils/vm")

describe("ld", () => {
    test("basic", async () => {
        const {
            code: cCode,
            readCarry,
            cycles: cCycles,
        } = dumpCarry([
            OP.LD.code, ocp("DIRECT", "REGISTER"), ...direct(42), 1,
        ])
        const {
            code,
            readRegValue,
            cycles
        } = dumpReg(cCode, 1);
        const dump = await runVm(code, OP.AND.cycles + cycles + cCycles);
        expect(readCarry(dump)).toBe(false);
    })
    test("basic", async () => {
        const {
            code: cCode,
            readCarry,
            cycles: cCycles,
        } = dumpCarry([
            OP.LD.code, ocp("DIRECT", "REGISTER"), ...direct(0), 1,
        ])
        const {
            code,
            readRegValue,
            cycles
        } = dumpReg(cCode, 1);
        const dump = await runVm(code, OP.AND.cycles + cycles + cCycles);
        expect(readCarry(dump)).toBe(true);
    })
})