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
#include <cmdline.h>
#include <compcfg.h>
#include <debug.h>
#include <emit.h>
#include <file.h>
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

void feval(FILE* fin, FILE* fout, EmitInfo finfo) {
    Stab stab = stabFile();
    // stabPrint(stdout, stab);
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

void eval(EmitInfo finfo, char* code) {
    String out, err;
    size_t out_size, err_size;

    FILE* fin = fmemopen(code, strlen(code), "r");
    FILE* fout = open_memstream(&out, &out_size);
    FILE* ferr = open_memstream(&err, &err_size);

    osSetStreams(osStdin, fout, ferr);
    feval(fin, fout, finfo);
    //compGLoopEval(fin, fout, finfo);
    osSetStreams(osStdin, osStdout, osStderr);

    fclose(fin);
    fclose(fout);
    fclose(ferr);

    puts("===================");
    puts(out);
    puts("===================");
    puts(err);
    puts("+++++++++++++++++++");
}

void _compGLoopInit(int argc, char **argv, FILE *fout, FileName *pfn, EmitInfo *pfinfo) {
    compInit();

    *pfn = fnameStdin();
    *pfinfo = emitInfoNew(*pfn);

    compFileInit(*pfinfo);
    comsgFini();
    fintInit();
}

void _compGLoopFinish(FileName fn, EmitInfo finfo) {
    fintFini();
    compFileFini(finfo);
    emitAllDone();
    emitInfoFree(finfo);
    fnameFree(fn);
    compFini();
}

void init_interpreter(int argc, char** argv) {
    Bool echo;

    osInit();
    osObtainLicense();

    echo = cmdSubsumeResponseFiles(1, &argc, &argv);

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

    FileName fn;
    EmitInfo finfo;

    compGLoopInit(argc, argv, osStdout, &fn, &finfo);
    eval(finfo, "#include \"aldor\"");
    eval(finfo, "#include \"aldorio\"");
    //eval(finfo, "print := stdout");
    //eval(finfo, "Int ==> MachineInteger;");
    //eval(finfo, "import from Int;");
    //eval(finfo, "f(x: Int): Int == x + x;");
    //eval(finfo, "stdout << f(10);");
    //eval(finfo, "for i in 1..10 repeat { stdout << i << newline; for j in 1..500000 repeat {} }")
    compGLoopFinish(fn, finfo);

    //fintMode = FINT_LOOP;
    //emitSetInterp(false);
    //emitSetRun(false);
    //comsgSetInteractiveOption();

    //_compGLoopInit(argc, argv, osStdout, &fn, &finfo);
    //eval(finfo, "#include \"aldor\"");
    //eval(finfo, "#include \"aldorio\"");
    //eval(finfo, "Int ==> MachineInteger;");
    //eval(finfo, "import from Int;");
    //eval(finfo, "f(x: Int): Int == x + x;");
    //eval(finfo, "stdout << f(10);");
    //_compGLoopFinish(fn, finfo);
}
