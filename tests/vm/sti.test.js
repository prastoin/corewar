const { OP, runVm, ocp, dumpReg, dumpCarry, direct, indirect, idx} = require("../utils/vm")

function sti(a, b) {
    return async () => {
        const code = [
            OP.LD.code, ocp("DIRECT", "REGISTER"), 42, 0, 0, 0, 1,
            OP.STI.code, ocp("REGISTER", "DIRECT", "DIRECT"), 1, ...indirect(a), ...indirect(b),
        ]
        const dump = await runVm(code, OP.STI.cycles + OP.LD.cycles);
        expect(dump[idx(a + b + 7)]).toBe(42);
    }
} 

describe("sti", () => {
    test("basic", sti(0, 0));
    test("basic2", sti(10, 10));
    test("Overflow IDX_MOD", sti(1000, 1000));
    test("Overflow MEM", sti(4000, 1000));
    test("basic neg", sti(-10, 0));
    test("neg overflow IDX", sti(-3330, 330));
})