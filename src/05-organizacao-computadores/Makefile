# Makefile para exemplos de Organização de Computadores
# Compila exemplos demonstrando conceitos de arquitetura

CC = gcc
CFLAGS = -Wall -g -O0
TARGETS = exemploSomaSimplesAssmbly exemploRegistradores exemploModosEnderecamento exemploCallingConvention

.PHONY: all clean run-all

all: $(TARGETS)

exemploSomaSimplesAssmbly: exemploSomaSimplesAssmbly.c
	$(CC) $(CFLAGS) -o $@ $<

exemploRegistradores: exemploRegistradores.c
	$(CC) $(CFLAGS) -o $@ $<

exemploModosEnderecamento: exemploModosEnderecamento.c
	$(CC) $(CFLAGS) -o $@ $<

exemploCallingConvention: exemploCallingConvention.c
	$(CC) $(CFLAGS) -o $@ $<

# Executa todos os exemplos
run-all: all
	@echo "=== Executando exemploSomaSimplesAssmbly ==="
	@./exemploSomaSimplesAssmbly
	@echo ""
	@echo "=== Executando exemploRegistradores ==="
	@./exemploRegistradores
	@echo ""
	@echo "=== Executando exemploModosEnderecamento ==="
	@./exemploModosEnderecamento
	@echo ""
	@echo "=== Executando exemploCallingConvention ==="
	@./exemploCallingConvention

# Gera assembly dos exemplos para análise
assembly: exemploSomaSimplesAssmbly.c exemploRegistradores.c exemploModosEnderecamento.c exemploCallingConvention.c
	$(CC) -S -masm=intel -O0 -o exemploSomaSimplesAssmbly.s exemploSomaSimplesAssmbly.c
	$(CC) -S -masm=intel -O0 -o exemploRegistradores.s exemploRegistradores.c
	$(CC) -S -masm=intel -O0 -o exemploModosEnderecamento.s exemploModosEnderecamento.c
	$(CC) -S -masm=intel -O0 -o exemploCallingConvention.s exemploCallingConvention.c
	@echo "Arquivos .s gerados com código assembly"

# Gera assembly otimizado para comparação
assembly-opt: exemploSomaSimplesAssmbly.c exemploRegistradores.c exemploModosEnderecamento.c exemploCallingConvention.c
	$(CC) -S -masm=intel -O2 -o exemploSomaSimplesAssmbly-O2.s exemploSomaSimplesAssmbly.c
	$(CC) -S -masm=intel -O2 -o exemploRegistradores-O2.s exemploRegistradores.c
	$(CC) -S -masm=intel -O2 -o exemploModosEnderecamento-O2.s exemploModosEnderecamento.c
	$(CC) -S -masm=intel -O2 -o exemploCallingConvention-O2.s exemploCallingConvention.c
	@echo "Arquivos .s otimizados gerados"

clean:
	rm -f $(TARGETS) *.s *.o
	@echo "Arquivos de compilação removidos"

help:
	@echo "Makefile para Exemplos de Organização de Computadores"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  all          - Compila todos os exemplos"
	@echo "  run-all      - Executa todos os exemplos"
	@echo "  assembly     - Gera código assembly dos exemplos (sem otimização)"
	@echo "  assembly-opt - Gera código assembly otimizado (-O2)"
	@echo "  clean        - Remove arquivos compilados"
	@echo "  help         - Mostra esta ajuda"
	@echo ""
	@echo "Exemplos individuais:"
	@echo "  ./exemploSomaSimplesAssmbly  - Soma simples com inline assembly"
	@echo "  ./exemploRegistradores       - Operações com registradores"
	@echo "  ./exemploModosEnderecamento  - Modos de endereçamento"
	@echo "  ./exemploCallingConvention   - Calling conventions e stack frames"
