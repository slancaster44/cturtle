CC := clang
CCFLAGS := -g -lm -O3 -Icommon

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

vm_lib:

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