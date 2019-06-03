const { OP, runVm, ocp, dumpReg, dumpCarry, direct, indirect, idx} = require("../utils/vm")

function ldi(a, b) {
    return async () => {
        const {
            code,
            readRegValue,
            cycles
        } = dumpReg
        ([
            OP.LD.code, ocp("DIRECT", "REGISTER"), 0, 0, 0, 42, 1,
            OP.STI.code, ocp("REGISTER", "DIRECT", "DIRECT"), 1, ...indirect(a), ...indirect(b),
            OP.LDI.code, ocp("DIRECT", "DIRECT", "REGISTER"), ...indirect(a), ...indirect(b - 7), 1
        ], 1)
        const dump = await runVm(code, OP.LD.cycles + OP.STI.cycles + OP.LDI.cycles + cycles);
        expect(readRegValue(dump)).toBe(42);
    }
} 

describe("ldi", () => {
    test("basic", ldi(0, 0));
    test("basic2", ldi(10, 10));
    test("Overflow IDX_MOD", ldi(1000, 1000));
    test("Overflow MEM", ldi(4000, 1000));
    test("basic neg", ldi(-10, 0));
    test("neg overflow IDX", ldi(-3330, 330));
})