#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
std::stack<Position> valid_positions;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    // TODO: Implemente esta função seguindo estes passos:
    // 1. Abra o arquivo especificado por file_name usando std::ifstream: OK
    // 2. Leia o número de linhas e colunas do labirinto: OK
    // 3. Redimensione a matriz 'maze' de acordo (use maze.resize()): OK
    // 4. Leia o conteúdo do labirinto do arquivo, caractere por caractere: OK
    // 5. Encontre e retorne a posição inicial ('e'): OK
    // 6. Trate possíveis erros (arquivo não encontrado, formato inválido, etc.): OK
    // 7. Feche o arquivo após a leitura: OK

    std::ifstream maze_file(file_name);

    if (!maze_file.is_open()) {
        std::cout << "Erro ao abrir o arquivo!" << std::endl;
        exit(1);
    }

    maze_file >> num_rows >> num_cols;

    maze.resize(num_rows);
    
    for (int i = 0; i < num_rows; i++) {
        maze[i].resize(num_cols);
    }

    char caractere;
    int row = 0, col = 0, row_in = -1, col_in = -1;
    maze_file.get(caractere); // Pega o '\n' da primeira linha
    while (maze_file.get(caractere)) {
        if (caractere == '\n') { // Ignora quebras de linha
            row++;
            col = 0;
        } else if (caractere == 'e') { // Acha a entrada do labirinto
            row_in = row;
            col_in = col;
            maze[row][col] = caractere;
            col++;
        } else if (caractere == 'x' || caractere == '#' || caractere == 's'){ // Preenche a matriz maze
            maze[row][col] = caractere;
            col++;
        } else {
            // Ignora qualquer outro tipo de caractere
        }
    }

    maze_file.close();
    
    return {row_in, col_in};
}

// Função para imprimir o labirinto
void print_maze() {
    // TODO: Implemente esta função
    // 1. Percorra a matriz 'maze' usando um loop aninhado: OK
    // 2. Imprima cada caractere usando std::cout: OK
    // 3. Adicione uma quebra de linha (std::cout << '\n') ao final de cada linha do labirinto: OK

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            std::cout << maze[i][j];
        }
        std::cout << '\n';
    }
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    // TODO: Implemente esta função
    // 1. Verifique se a posição está dentro dos limites do labirinto
    //    (row >= 0 && row < num_rows && col >= 0 && col < num_cols): OK
    // 2. Verifique se a posição é um caminho válido (maze[row][col] == 'x'): OK
    // 3. Retorne true se ambas as condições forem verdadeiras, false caso contrário: OK

    if ((row >= 0) && (row < num_rows) && (col >= 0) && (col < num_cols)) {
        if (maze[row][col] == 'x' || maze[row][col] == 's') {
            return true;
        } else{
            return false;
        }
    } else {
        return false;
    }
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    // TODO: Implemente a lógica de navegação aqui
    // 1. Marque a posição atual como visitada (maze[pos.row][pos.col] = '.'): OK
    // 2. Chame print_maze() para mostrar o estado atual do labirinto: OK
    // 3. Adicione um pequeno atraso para visualização:
    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // 4. Verifique se a posição atual é a saída (maze[pos.row][pos.col] == 's')
    //    Se for, retorne true: OK
    // 5. Verifique as posições adjacentes (cima, baixo, esquerda, direita)
    //    Para cada posição adjacente:
    //    a. Se for uma posição válida (use is_valid_position()), adicione-a à pilha valid_positions: OK
    // 6. Enquanto houver posições válidas na pilha (!valid_positions.empty()):
    //    a. Remova a próxima posição da pilha (valid_positions.top() e valid_positions.pop()): OK
    //    b. Chame walk recursivamente para esta posição: OK
    //    c. Se walk retornar true, propague o retorno (retorne true): OK
    // 7. Se todas as posições foram exploradas sem encontrar a saída, retorne false: OK

    bool is_exit = maze[pos.row][pos.col] == 's';

    maze[pos.row][pos.col] = '.';

    print_maze();

    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    system("cls");

    if (is_exit) {
        return true;
    }

    if (is_valid_position(pos.row-1, pos.col)) { // Verifica posição de cima
        valid_positions.push({pos.row-1, pos.col});
    }
    
    if (is_valid_position(pos.row+1, pos.col)) { // Verifica posição de baixo
        valid_positions.push({pos.row+1, pos.col});
    }
    
    if (is_valid_position(pos.row, pos.col-1)) { // Verifica posição da esquerda
        valid_positions.push({pos.row, pos.col-1});
    }
    
    if (is_valid_position(pos.row, pos.col+1)) { // Verifica posição da direita
        valid_positions.push({pos.row, pos.col+1});
    }
    
    while (!valid_positions.empty()) {
        Position next_pos = valid_positions.top();
        valid_positions.pop();

        if (walk(next_pos)) {
            return true;
        }
    }
    
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);

    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    // print_maze();

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}

// Nota sobre o uso de std::this_thread::sleep_for:
// 
// A função std::this_thread::sleep_for é parte da biblioteca <thread> do C++11 e posteriores.
// Ela permite que você pause a execução do thread atual por um período especificado.
// 
// Para usar std::this_thread::sleep_for, você precisa:
// 1. Incluir as bibliotecas <thread> e <chrono>
// 2. Usar o namespace std::chrono para as unidades de tempo
// 
// Exemplo de uso:
// std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 
// Isso pausará a execução por 50 milissegundos.
// 
// Você pode ajustar o tempo de pausa conforme necessário para uma melhor visualização
// do processo de exploração do labirinto.
