import numpy as np
import random


def create_sudoku(N):
    if N < 4 or not np.sqrt(N).is_integer():
        print("O tamanho deve ser um quadrado perfeito maior ou igual a 4.")
        return

    n = int(np.sqrt(N))

    def pattern(r, c):
        return (n * (r % n) + r // n + c) % N

    base = np.zeros((N, N), dtype=int)

    for r in range(N):
        for c in range(N):
            base[r, c] = pattern(r, c)

    def shuffle_sudoku(s):
        rng = np.random.default_rng()

        for _ in range(2):
            for b in range(n):
                rows = rng.permutation(n)
                cols = rng.permutation(n)
                s[n * b:n * (b + 1), :] = s[n * b + rows, :]
                s[:, n * b:n * (b + 1)] = s[:, n * b + cols]

        return s

    sudoku = shuffle_sudoku(base)
    return sudoku + 1


def print_sudoku(sudoku):
    N = sudoku.shape[0]
    print(f"{N}x{N}")
    for row in sudoku:
        print(" ".join(str(n) for n in row))


if __name__ == "__main__":
    N = 9
    Raiz = int(np.sqrt(N))
    sudoku = create_sudoku(N)
    with open('input.txt', 'w') as f:
        print(f"{N}x{N}", file=f)
        print(f"{Raiz}x{Raiz}", file=f)
        for row in sudoku:
            print(" ".join(str(n) for n in row), file=f)