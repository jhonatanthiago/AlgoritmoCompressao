#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>

using namespace std;

// Nó da árvore de Huffman
struct Node {
    unsigned char ch;
    int freq;
    Node* left;
    Node* right;
    Node(unsigned char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(Node* l, Node* r) : ch(0), freq(l->freq + r->freq), left(l), right(r) {}
};

// Comparador para fila de prioridade
struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

// Gera códigos binários
void buildCodes(Node* root, unordered_map<unsigned char, string>& codes, string str) {
    if (!root) return;
    if (!root->left && !root->right) { codes[root->ch] = str; return; }
    buildCodes(root->left, codes, str + "0");
    buildCodes(root->right, codes, str + "1");
}

// Libera memória da árvore
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Compressão
void compress(const string& inputFile, const string& outputFile) {
    // Leitura binária do arquivo original
    ifstream in(inputFile, ios::binary);
    vector<unsigned char> data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    // Contagem de frequência
    unordered_map<unsigned char, int> freq;
    for (auto c : data) freq[c]++;

    // Construção da árvore de Huffman
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& p : freq) pq.push(new Node(p.first, p.second));

    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a, b));
    }
    Node* root = pq.top();

    // Geração de códigos
    unordered_map<unsigned char, string> codes;
    buildCodes(root, codes, "");

    // Codificação em bits
    string bitStr;
    for (auto c : data) bitStr += codes[c];

    // Agrupa em bytes
    vector<unsigned char> outBytes;
    for (size_t i = 0; i < bitStr.size(); i += 8) {
        string byteStr = bitStr.substr(i, 8);
        while (byteStr.size() < 8) byteStr += '0';
        outBytes.push_back(bitset<8>(byteStr).to_ulong());
    }

    // Grava arquivo binário
    ofstream out(outputFile, ios::binary);
    out.write((char*)outBytes.data(), outBytes.size());
    out.close();

    cout << "Compressao concluida: " << inputFile << " -> " << outputFile << endl;
    freeTree(root);
}

// Função para decodificar bits usando a árvore
vector<unsigned char> decodeBits(const vector<unsigned char>& binData, Node* root, size_t originalSize) {
    vector<unsigned char> result;
    Node* curr = root;
    for (size_t i = 0; i < binData.size(); ++i) {
        bitset<8> b(binData[i]);
        for (int j = 7; j >= 0; --j) {
            if (b[j] == 0) curr = curr->left;
            else curr = curr->right;
            if (!curr->left && !curr->right) {
                result.push_back(curr->ch);
                curr = root;
                if (result.size() == originalSize) return result;
            }
        }
    }
    return result;
}

// Descompressão
void decompress(const string& inputFile, const string& outputFile, const unordered_map<unsigned char, int>& freq, size_t originalSize) {
    // Reconstrução da árvore
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& p : freq) pq.push(new Node(p.first, p.second));
    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a, b));
    }
    Node* root = pq.top();

    // Leitura binária do arquivo comprimido
    ifstream in(inputFile, ios::binary);
    vector<unsigned char> binData((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    // Decodificação
    vector<unsigned char> decoded = decodeBits(binData, root, originalSize);

    // Grava descomprimido
    ofstream out(outputFile, ios::binary);
    out.write((char*)decoded.data(), decoded.size());
    out.close();

    cout << "Descompressao concluida: " << inputFile << " -> " << outputFile << endl;
    freeTree(root);
}

// Exemplo de uso
int main() {
    string arquivoOriginal = "teste.txt";
    string arquivoComprimido = "teste.bin";
    string arquivoDescomprimido = "teste_descomprimido.txt";

    // Primeiro, criamos frequência do arquivo
    ifstream in(arquivoOriginal, ios::binary);
    vector<unsigned char> data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();
    unordered_map<unsigned char, int> freq;
    for (auto c : data) freq[c]++;
    size_t originalSize = data.size();

    compress(arquivoOriginal, arquivoComprimido);
    decompress(arquivoComprimido, arquivoDescomprimido, freq, originalSize);

    return 0;
}
