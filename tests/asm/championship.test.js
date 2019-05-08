const tmp = require('tmp-promise')
const fetch = require('node-fetch')
const tar = require('tar')
const child_process = require('child_process')
const { promises: fs, constants } = require('fs')
const { join } = require('path')

const RESSOURCES = 'https://projects.intra.42.fr/uploads/document/document/391/vm_champs.tar'

const TESTED_BINARY = join(__dirname, '..', '..', 'asm')

const {
    EXPORT_CHAMPIONSHIP
} = process.env;

function run_process(command, args) {
    return new Promise((resolve, reject) => {
        const process = child_process.spawn(command, args, {
            stdio: 'ignore'
        })
        process.once('close', code => {
            if (code !== 0) reject(code ? new Error(`Process failed with code: ${code}`) : new Error("Process timeout"))
            else resolve();
        })
        process.once('error', reject)
        setTimeout(() => {
            process.kill()
        }, 1000)
    })
}

async function run_asm(asm, source, output) {
    expect(source.endsWith('.s')).toBe(true)
    const out_path = source.slice(0, -2) + '.cor'
    await fs.symlink(output, out_path)
    await run_process(asm, [source])
    await fs.unlink(out_path)
}

const decoder = new (require('util').TextDecoder)()
const NAME_LENGTH = 128

function read_name(buffer) {
    return `${decoder.decode(buffer.slice(4, 4 + NAME_LENGTH)).replace(/\0*$/, "")}.cor`
}

async function compare_asm(asm, file, year) {
    await fs.access(asm, constants.X_OK);
    const out_ref = await tmp.file()
    await run_asm(asm, file, out_ref.path)
    const out_our = await tmp.file()
    await run_asm(TESTED_BINARY, file, out_our.path)
    const our_content = await fs.readFile(out_our.path)
    expect(our_content).toEqual(await fs.readFile(out_ref.path))
    if (EXPORT_CHAMPIONSHIP) {
        const path = join(EXPORT_CHAMPIONSHIP, year)
        await fs.mkdir(path).catch(() => {})
        await fs.writeFile(join(path, read_name(our_content)), our_content);
    }
    out_ref.cleanup()
    out_our.cleanup()
}

it('Compiles champions of Corewar Championships', async () => {
    await fs.access(TESTED_BINARY, constants.X_OK);
    const dir = await tmp.dir({ unsafeCleanup: true });
    try {
        const res = await fetch(RESSOURCES)

        await new Promise((resolve, reject) => {
            res.body.pipe(tar.extract({ cwd: dir.path }))
                .once('finish', resolve)
                .once('error', reject)
        })

        const path = join(dir.path, 'champs', 'championships')

        const years = (await fs.readdir(path))
            .filter(path => !path.startsWith('.'))

        const championships = await Promise.all(years.map(async year => {
            return {
                year,
                champions: await Promise.all((await fs.readdir(join(path, year), { withFileTypes: true }))
                    .filter(path => path.isDirectory() && !path.name.startsWith('.'))
                    .map(async ({ name: student }) => {
                        const champs = (await fs.readdir(join(path, year, student)))
                            .filter(path => !path.startsWith('.'))
                        return {
                            student,
                            champs
                        }
                    }))
            }
        }))

        const suites = championships.map(({ year, champions }) => describe(year, () => {
            for (const champion of champions) {
                for (const champ of champion.champs) {
                    fit(`${champion.student}'s ${champ}`, async () => {
                        const yearAsm = join(path, year, 'asm') 
                        const asm = await fs.access(yearAsm).then(() => yearAsm, () => join(dir.path, 'asm'))
                        await compare_asm(asm, join(path, year, champion.student, champ), year)
                    })
                }
            }
        }))

        for (const suite of suites) {
            await jasmine
                .getEnv()
                .execute([suite.id], suite)
        }
    } finally {
        dir.cleanup()
    }
})