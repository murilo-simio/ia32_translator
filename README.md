# Trabalho 2 da disciplina de Software Básico

## Implementação de um tradutor


### Para buildar o programa
```
cd translator && make clean all
```

### Para rodar o programa
```
./build/apps/translator <arquivo_de_entrada.asm>

nasm -f elf -o saida.o <arquivo_de_saida.s>

ld -m elf_i386 -o saida saida.o
```

### Alunos

* Aline Mitiko Otsubo - 17/0004601
* Murilo Simionato Arnemann - 18/0107453

### Ambiente de desenvolvimento
```
gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
```