import random
import argparse

def generate_adjacency_matrix(n, multigraph=True, max_multiedge=3, undirected=False):
    matrix = [[0]*n for _ in range(n)]

    for i in range(n):
        for j in range(n):
            if i == j:
                matrix[i][j] = 0
            else:
                if multigraph:
                    matrix[i][j] = random.randint(0, max_multiedge)
                else:
                    matrix[i][j] = random.randint(0, 1)

                if undirected:
                    matrix[j][i] = matrix[i][j]

    return matrix


def write_graph_file(
    filename,
    n1,
    n2,
    multigraph=True,
    max_multiedge=10,
    undirected=False,
    extra_data=None
):
    with open(filename, "w") as f:
        # First graph
        f.write(f"{n1}\n")
        g1 = generate_adjacency_matrix(n1, multigraph, max_multiedge, undirected)
        for row in g1:
            f.write(" ".join(map(str, row)) + "\n")

        # Second graph
        f.write(f"{n2}\n")
        g2 = generate_adjacency_matrix(n2, multigraph, max_multiedge, undirected)
        for row in g2:
            f.write(" ".join(map(str, row)) + "\n")

        # Optional extra lines
        if extra_data:
            for line in extra_data:
                f.write(str(line) + "\n")

    print(f"Generated file: {filename}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Graph input file generator")

    parser.add_argument("--n1", type=int, required=True, help="Number of vertices of first graph")
    parser.add_argument("--n2", type=int, required=True, help="Number of vertices of second graph")
    parser.add_argument("--outfile", type=str, required=True, help="Output filename")

    parser.add_argument("--max", type=int, default=3,
                        help="Max number of edges between nodes (for multigraphs)")

    parser.add_argument("--multigraph", action="store_true",
                        help="Enable multigraph mode (default: simple graph)")

    parser.add_argument("--undirected", action="store_false",
                        help="Make graphs undirected")

    parser.add_argument("--extra", nargs="*", default=[],
                        help="Optional extra lines added at the end")

    args = parser.parse_args()

    write_graph_file(
        filename=args.outfile,
        n1=args.n1,
        n2=args.n2,
        multigraph=args.multigraph,
        max_multiedge=args.max,
        undirected=args.undirected,
        extra_data=args.extra
    )