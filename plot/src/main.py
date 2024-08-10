from graph import Graph
import os


def main():
    # File paths
    before_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/before_fft_11025_real_signal.txt",
    )
    fft_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/after_fft_11025.txt",
    )
    fft_negative_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/after_fft_11025_negative.txt",
    )
    result_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/after_fft_11025_real_signal.txt",
    )

    graph = Graph()

    # Load csv data into data frames
    before = graph.load_into_df(before_path)
    after_fft = graph.load_into_df(fft_path)
    after_fft_negative = graph.load_into_df(fft_negative_path)
    result = graph.load_into_df(result_path)

    # Generate graphs
    graph.generate_before_graph(before)
    graph.generate_after_fft_graph(after_fft)
    graph.generate_after_fft_negative_graph(after_fft_negative)
    graph.generate_result_graph(result)


if __name__ == "__main__":
    main()
