# Makefile para compilar exemplos do módulo 00-fundamentos
#
# Uso:
#   make              - Compila todos os exemplos
#   make exemplos     - Compila e executa os exemplos básicos
#   make clean        - Remove arquivos gerados
#   make help         - Mostra ajuda

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lm

# Diretórios
BUILD_DIR = build

# Arquivos fonte e executáveis
SOURCES = exemplos_basicos.c
EXECUTABLES = $(BUILD_DIR)/exemplos_basicos

# Cores para output (opcional)
COLOR_RESET = \033[0m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m

# Regra padrão
.PHONY: all
all: $(BUILD_DIR) $(EXECUTABLES)
	@echo "$(COLOR_GREEN)✓ Compilação concluída com sucesso!$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)Execute 'make exemplos' para rodar os exemplos$(COLOR_RESET)"

# Criar diretório de build
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compilar exemplos básicos
$(BUILD_DIR)/exemplos_basicos: exemplos_basicos.c
	@echo "$(COLOR_YELLOW)Compilando exemplos_basicos.c...$(COLOR_RESET)"
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Executar exemplos
.PHONY: exemplos
exemplos: $(BUILD_DIR)/exemplos_basicos
	@echo "$(COLOR_GREEN)Executando exemplos básicos:$(COLOR_RESET)"
	@echo "============================================"
	@./$(BUILD_DIR)/exemplos_basicos

# Limpar arquivos gerados
.PHONY: clean
clean:
	@echo "$(COLOR_YELLOW)Limpando arquivos gerados...$(COLOR_RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(COLOR_GREEN)✓ Limpeza concluída!$(COLOR_RESET)"

# Recompilar tudo
.PHONY: rebuild
rebuild: clean all

# Ajuda
.PHONY: help
help:
	@echo "$(COLOR_BLUE)Makefile para módulo 00-fundamentos$(COLOR_RESET)"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  make            - Compila todos os exemplos"
	@echo "  make exemplos   - Compila e executa os exemplos básicos"
	@echo "  make clean      - Remove arquivos gerados"
	@echo "  make rebuild    - Limpa e recompila tudo"
	@echo "  make help       - Mostra esta ajuda"
	@echo ""
	@echo "Exemplos de uso:"
	@echo "  make && make exemplos    - Compilar e executar"
	@echo "  make clean               - Limpar antes de commit"
