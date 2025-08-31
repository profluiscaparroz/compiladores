CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCDIR = src
INCDIR = include
BUILDDIR = build
TESTDIR = tests

SOURCES = $(SRCDIR)/lexer.c $(SRCDIR)/main.c
OBJECTS = $(BUILDDIR)/lexer.o $(BUILDDIR)/main.o
TARGET = $(BUILDDIR)/lexer

.PHONY: all clean test

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

test: $(TARGET)
	@echo "Testando o analisador léxico..."
	@for file in $(TESTDIR)/*.tl; do \
		if [ -f "$$file" ]; then \
			echo ""; \
			echo "=== Testando $$file ==="; \
			./$(TARGET) "$$file"; \
		fi; \
	done

clean:
	rm -rf $(BUILDDIR)

help:
	@echo "Comandos disponíveis:"
	@echo "  make        - compila o analisador léxico"
	@echo "  make test   - executa todos os testes"
	@echo "  make clean  - remove arquivos de compilação"
	@echo "  make help   - mostra esta ajuda"
	@echo ""
	@echo "Uso manual:"
	@echo "  ./$(TARGET) arquivo.tl"