const { OP, runVm, ocp, dumpReg, dumpCarry, direct } = require("../utils/vm")

function ldi(a, b) {
    return async () => {
        const {
            code,
            readCarry,
            cycles
        } = dumpReg([
            OP.LDI.code, ocp("DIRECT", "DIRECT", "REGISTER"), ...direct(a), ...direct(b), 1,
        ], 1)
        const dump = await runVm(code, OP.LDI.cycles + cycles);
    }
} 

describe("ldi", () => {
    test("basic", ldi(0, 0));
})