## TinyScript

#### A javascript compiler designed to work with JSI to speed-up React Native application logic.

Steps complete:

- Phase 0 - AST logic.
  - [x] Tokenize JS files
  - [] Construct AST from available tokens
  - [] (Phase 1 & 2) Work on optimizing AST
- Phase 1 - Transpiling.
  - [] Transpile available AST to C
  - [] Add crude JS -> BIN compiling CLI utilities.
- Phase 2 - React Native support
  - [] Add TS, TSX support
  - [] Implement JSI interface
  - [] Implement automatic RN -> BIN conversion
- Phase 3 - Tooling.
  - [] Add better CLI tools to streamline TinyScript usage
- Phase 4 - Compiling.
  - [] Rewrite C transpiler to LLVM IR compiler
