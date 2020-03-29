Compile_RootIncludeDir := ${Compile_RootIncludeDir_Default}
CompileHeaderFiles := ${CompileHeaderFiles_Default}
CompileSrcFiles := ${CompileSrcFiles_Default}
SplittedCompileSrcFiles := ${SplittedCompileSrcFiles_Default}
CompileCCFlags := ${CompileCCFlags_Default}
CompileCCLinkFlags := ${CompileCCLinkFlags_Default}
CompileCPPFlags := ${CompileCPPFlags_Default}
CompileCPPLinkFlags := ${CompileCPPLinkFlags_Default}
CompileTarget := ${CompileTarget_Default}
CompileCleanDirs := ${CompileCleanDirs_Default}
CompilePhonyAdditionalTarget :=

include ${ENV_COMPILE_FUNCTIONS}
