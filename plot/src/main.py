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
    filter_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/11025_frequency_filter.txt",
    )
    real_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/11025_real_signal.txt",
    )

    graph = Graph()

    # Load csv data into data frames
    # before = graph.load_into_df(before_path)
    # after_fft = graph.load_into_df(fft_path)
    # after_fft_negative = graph.load_into_df(fft_negative_path)
    # result = graph.load_into_df(result_path)
    # filter = graph.load_into_df(filter_path)
    # real = graph.load_into_df(real_path)

    # Generate graphs
    # graph.generate_before_graph(before)
    # graph.generate_after_fft_graph(after_fft)
    # graph.generate_after_fft_negative_graph(after_fft_negative)
    # graph.generate_result_graph(result)
    # graph.generate_filter_graph(filter)
    # graph.generate_real_graph(real)

    # 8-13 tests
    am_demod_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/11025_am_demod.txt",
    )
    am_demod_filter_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/11025_filter_am_demod.txt",
    )
    am_demod_real_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/11025_real_am_demod.txt",
    )
    am_demod_mag_path: str = os.path.join(
        os.getcwd(),
        "..",
        "..",
        "documentation/output/plots/11025_mag_am_demod.txt",
    )
    am_demod = graph.load_into_df(am_demod_path)
    am_demod_filter = graph.load_into_df(am_demod_filter_path)
    am_demod_real = graph.load_into_df(am_demod_real_path)
    am_mag_demod = graph.load_into_df(am_demod_mag_path)
    graph.generate_graphs(am_demod, "am_demod", "frequency")
    graph.generate_graphs(am_demod_filter, "am_demod_path", "frequency")
    graph.generate_graphs(am_demod_real, "am_demod_real", "time")
    graph.generate_graphs(am_mag_demod, "am_demod_mag", "time")


if __name__ == "__main__":
    main()
