.PHONY: all clean lexer

all: lexer

lexer:
	$(MAKE) -C src/07-analisador-lexico

clean:
	$(MAKE) -C src/07-analisador-lexico clean