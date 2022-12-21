CC := clang
CCFLAGS := -g -lm -Icommon #-lefence #-O3

VM_SRC := vm/vm.c
VM_HDR := vm/instructions.h common/mem_mac.h vm/vm.h
VM_TARGET := vm/tvm

vm_build_test:
	$(CC) $(CCFLAGS) $(VM_HDR) $(VM_SRC) vm/test.c
	@rm -rf vm/*.gch vm/*.o vm/*.out vm/*.so
	@mv a.out $(VM_TARGET)

vm_run_test:
	@make vm_build_test
	vm/tvm

vm_clean:
	rm -rf vm/*.gch vm/*.o vm/*.out vm/*.so $(VM_TARGET)

HASH_SRC := hash/hash.c
HASH_HDR := common/mem_mac.h hash/hash.h
HASH_TARGET := hash/hast_test

hash_build_test:
	$(CC) $(CCFLAGS) $(HASH_HDR) $(HASH_SRC) hash/test.c
	@rm -rf hash/*.gch hash/*.o hash/*.out hash/*.so
	@mv a.out $(HASH_TARGET)

hash_run_test:
	@make hash_build_test
	$(HASH_TARGET)

hash_clean:
	rm -rf hash/*.gch hash/*.o hash/*.out hash/*.so $(HASH_TARGET)

LEXER_SRC := lexer/lexer.c hash/hash.c
LEXER_HDR := lexer/lexer.h hash/hash.h
LEXER_TARGET := lexer/lexer_test

lexer_build_test:
	$(CC) $(CCFLAGS) -Ihash $(LEXER_HDR) $(LEXER_SRC) lexer/test.c
	@rm -rf lexer/*.gch lexer/*.o lexer/*.out lexer/*.so
	@mv a.out $(LEXER_TARGET)

lexer_run_test:
	@make lexer_build_test
	$(LEXER_TARGET)

lexer_clean:
	rm -rf lexer/*.gch lexer/*.o lexer/*.out lexer/*.so $(LEXER_TARGET)

TBIN_SRC := tbin/tbin.c common/test.c
TBIN_HDR := tbin/tbin.h vm/instructions.h common/test.h
TBIN_TARGET := tbin/tbin

tbin_build_test:
	$(CC) $(CCFLAGS) -Ivm $(TBIN_HDR) $(TBIN_SRC) tbin/test.c
	@rm -rf tbin/*.gch tbin/*.o tbin/*.out tbin/*.so
	@mv a.out $(TBIN_TARGET)

tbin_run_test:
	@make tbin_build_test
	$(TBIN_TARGET)

tbin_clean:
	rm -rf tbin/*.gch tbin/*.o tbin/*.out tbin/*.so $(LEXER_TARGET)

TDB_SRC := tdb/tdb.c
TDB_TARGET := tdb/tdb

tdb_make:
	$(CC) $(CCFLAGS) -Itvm $(TDB_SRC) $(VM_HDR) $(VM_SRC)
	@rm -rf vm/*.gch vm/*.o vm/*.out vm/*.so tdb/*.o tdb/*.out tdb/*.so tdb/*.gch
	@mv a.out $(TDB_TARGET)

tdb_make_run:
	@make tdb_make
	$(TDB_TARGET)

tdb_clean:
	@rm -rf vm/*.gch vm/*.o vm/*.out vm/*.so tdb/*.o tdb/*.out tdb/*.so tdb/*.gch
	@rm -rf $(TDB_TARGET)

PRS_SRC := parser/parser.c parser/ast.c lexer/lexer.c hash/hash.c common/test.c parser/type_check.c parser/symtab.c
PRS_HDR := parser/ast.h parser/parser.h
PRS_TARGET := parser/test

prs_make_test:
	$(CC) $(CCFLAGS) -Ilexer -Ihash -Icommon $(PRS_HDR) $(PRS_SRC) parser/test.c
	@rm -rf lexer/*.gch lexer/*.o lexer/*.out lexer/*.so hash/*.gch parser/*.gch
	@mv a.out $(PRS_TARGET)

prs_make_run: prs_make_test
	$(PRS_TARGET)

GEN_SRC := codegen/codegen.c  $(TDB_SRC) $(VM_SRC) $(PRS_SRC)
GEN_HDR := codegen/symtab.h 
GEN_INC := -Ihash -Icommon -Ivm -Iparser -Ilexer -Itdb
GEN_TARGET := codegen/test

gen_make_test:
	$(CC) $(CCFLAGS) $(GEN_INC) $(GEN_HDR) $(GEN_SRC) codegen/test.c
	@mv a.out $(GEN_TARGET)
	@rm -rf $(find .. -name "*.gch" 2>/dev/null) $(find .. -name "*.o" 2>/dev/null) $(find .. -name "*.out" 2>/dev/null)

gen_run_test: gen_make_test
	$(GEN_TARGET)
	@rm -rf $(GEN_TARGET)

MAIN_SRC := main.c $(GEN_SRC) tbin/tbin.c
MAIN_HDR := 
MAIN_INC := -Icodegen -Iparser -Ilexer -Ihash -Icommon -Ivm -Itdb -Itbin
MAIN_TARGET := cturtle

default:
	$(CC) $(CCFLAGS) $(MAIN_INC) $(MAIN_HDR) $(MAIN_SRC)
	@mv a.out $(MAIN_TARGET)
	@rm -rf $(find . -name "*.gch" 2>/dev/null) $(find . -name "*.o" 2>/dev/null) $(find . -name "*.out" 2>/dev/null)

run: default
	./$(MAIN_TARGET) 
	@rm -rf $(MAIN_TARGET)

