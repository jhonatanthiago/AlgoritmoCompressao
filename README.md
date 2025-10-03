# Algoritmo de Compressão Huffman

Este projeto implementa um **algoritmo de compressão e descompressão baseado no método de Huffman**, otimizado para arquivos de código-fonte ou arquivos de texto em geral.  

O projeto contém os seguintes arquivos:

- `huffman.cpp` → Código do compressor/descompressor e implementação da árvore de Huffman  
- `tabela.txt` → Arquivo de tabela de frequência de símbolos  
- `teste.txt` → Arquivo de teste para compressão  
- `teste_descomprimido.txt` → Arquivo resultante da descompressão

---

## Como Rodar

### 1. Compile o programa

Abra o CMD na pasta do projeto e execute:

```cmd
g++ -std=c++11 huffman.cpp -o huffman.exe
````

Isso criará o executável `huffman.exe` na mesma pasta.

---

### 2. Executando o programa

O programa aceita três argumentos:

```
huffman.exe <modo> <arquivo_entrada> <arquivo_saida>
```

* `<modo>`: `c` para compressão, `d` para descompressão
* `<arquivo_entrada>`: arquivo a ser processado
* `<arquivo_saida>`: arquivo de saída

#### Exemplo 1: Compressão

```cmd
huffman.exe c teste.txt teste_comprimido.txt
```

#### Exemplo 2: Descompressão

```cmd
huffman.exe d teste_comprimido.txt teste_descomprimido.txt
```

---

### 3. Testando a integridade

Para verificar se a descompressão foi correta, use:

```cmd
fc teste.txt teste_descomprimido.txt
```

Se não aparecer nenhuma diferença, os arquivos são **iguais**.

---

## Estrutura Interna

* **Árvore de Huffman**: cada símbolo do arquivo é um nó-folha. Símbolos mais frequentes recebem códigos binários menores.
* **Compressão**: substitui cada símbolo pelo seu código da árvore.
* **Descompressão**: percorre a árvore bit a bit para recuperar os símbolos originais.

---

## Complexidade

* **Criação da árvore**: `O(n log n)` (n = número de símbolos distintos)
* **Compressão/Descompressão**: `O(m)` (m = tamanho do arquivo em caracteres)
* **Espaço**: `O(n)` para armazenar a árvore e tabela de códigos

---

## Autor

**Jhonatan Thiago**
Disciplina: Estruturas de Dados Básicas II – Projeto 1
