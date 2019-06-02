const { OP, runVm, ocp, dumpReg } = require("../utils/vm")

describe("xor", () => {
    test("basic", async () => {
        const {
            code,
            readRegValue,
            cycles
        } = dumpReg([
            OP.XOR.code, ocp("DIRECT", "DIRECT", "REGISTER"), 0, 0, 0, 42, 0, 0, 0, 36, 1,
        ], 1)

        const dump = await runVm(code, OP.XOR.cycles + cycles);
        expect(readRegValue(dump)).toBe(42 ^ 36);
    })
})