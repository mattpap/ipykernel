#include <cport.h>
#include <opsys.h>
#include <strops.h>
#include <cmdline.h>
#include <abcheck.h>
#include <abnorm.h>
#include <abuse.h>
#include <axltop.h>
#include <bloop.h>
#include <ccomp.h>
#include <compcfg.h>
#include <debug.h>
#include <emit.h>
#include <fint.h>
#include <fintphase.h>
#include <fluid.h>
#include <stab.h>
#include <comsg.h>

extern String compRootDir;
extern Bool   compIsDebug;
extern String cmdOptionArg;

extern void compGLoopInit   (int, char**, FILE*, FileName*, EmitInfo*);
extern void compGLoopEval   (FILE*, FILE*, EmitInfo);
extern int  compGLoopFinish (FileName, EmitInfo);

extern void osSetStreams(FILE*, FILE*, FILE*);

JmpBuf compFintJmpBuf;

static void compFintBreakHandler0(int signo) {
    comsgFPrintf(osStdout, ALDOR_M_FintBreakHandler0);
}

static void feval(FILE* fin, FILE* fout, EmitInfo finfo) {
    Stab stab = stabFile();
    AbSyn ab;
    Foam foam;
    int lineno = 0;

    comsgInit();

    while (!osFEof(fin)) {
        Bool ok;
        intStepNo += 1;
        comsgFini();
        comsgInit();
        car(stab)->isChecked = false;

        if (SetJmp(compFintJmpBuf))
            if (osFEof(fin))
                break;

        osSetBreakHandler(compFintBreakHandler0);
        ab = compFileFront(finfo, stab, fin, &lineno);
        breakSetRoot(ab);

        if (!compIsMoreAfterFront(finfo) || abIsEmptySequence(ab))
            continue;

        ab = (AbSyn) fintWrap(ab, intStepNo);
        foam = compFileMiddle(finfo, stab, ab);

        if (!foam)
            continue;

        ok = fint(foam);
        foamFree(foam);

        if (!ok)
            fprintf(fout, "Unhandled Exception!\n");

        if (fintVerbose && ok)
            fintPrintType(fout, ab);
    }

    comsgFini();
}

static FileName fn;
static EmitInfo finfo;

void evaluate(const char* code, char** out, char** err) {
    size_t out_size, err_size;

    FILE* fin = fmemopen((char*)code, strlen(code), "r");
    FILE* fout = open_memstream(out, &out_size);
    FILE* ferr = open_memstream(err, &err_size);

    osSetStreams(osStdin, fout, ferr);
    feval(fin, fout, finfo);
    osSetStreams(osStdin, osStdout, osStderr);

    fclose(fin);
    fclose(fout);
    fclose(ferr);
}

void complete(const char* line, int pos, char*** matches, int* count, char** text) {
    // Stab stab = stabFile();
    *matches = NULL;
    *count = 0;
    *text = malloc(0);
}

void init_interpreter(int argc, char** argv) {
    /*Bool echo;*/

    osInit();
    osObtainLicense();

    // /*echo =*/ cmdSubsumeResponseFiles(1, &argc, &argv);

    if (cmdHasRootOption(argc, argv))
        compRootDir = cmdOptionArg;
    if (!compRootDir)
        compRootDir = strCopyIf(osGetEnv("ALDORROOT"));
    //if (!compRootDir)
    //    compRootDir = compRootFromCmdLine(osCurDirName(), argv[0]);

    if (cmdHasCfgFileOption(argc, argv))
        compCfgSetConfigFile(cmdOptionArg);
    if (cmdHasCfgNameOption(argc, argv))
        compCfgSetSysName(cmdOptionArg);

    if (cmdHasDebugOption(argc, argv))
        compIsDebug = true;

    fintMode = FINT_LOOP;
    emitSetInterp(false);
    emitSetRun(false);
    comsgSetInteractiveOption();

    compGLoopInit(argc, argv, osStdout, &fn, &finfo);
}

void free_interpreter() {
    compGLoopFinish(fn, finfo);
}
