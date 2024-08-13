import matplotlib
import matplotlib.pyplot as plt
import matplotlib.style as mplstyle
import matplotlib.style
import numpy as np
import pandas as pd
import os


class Graph:
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

    def __init__(self):
        self

    def open_file(self):
        print(self.log_path)
        with open(self.log_path, "r") as file:
            count: int = 0
            for i in file:
                line = i.strip()
                print(f"{count}: {line}")
                count += 1

    def load_into_df(self, path: str):
        df = pd.read_csv(path, dtype={"Real": float, "Imaginary": float})
        row_count: int = len(df.index)
        print(f"row count: {row_count}")
        print(df)
        return df

    def generate_before_graph(self, df):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        ax.set(xlabel="Sample", ylabel="Amplitude")
        ax.grid()
        fig.savefig("base.png")
        plt.show()

    # In the frequency domain
    def generate_after_fft_graph(self, df):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        ax.set(xlabel="Frequency Bin", ylabel="Amount")
        ax.grid()
        fig.savefig("fft.png")
        plt.show()

    # In the frequency domain
    def generate_after_fft_negative_graph(self, df):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        ax.set(xlabel="Frequency Bin", ylabel="Amount")
        ax.grid()
        fig.savefig("fft_negative.png")
        plt.show()

    def generate_result_graph(self, df):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        ax.plot(df[" Imaginary"])
        ax.set(xlabel="Sample", ylabel="Amplitude")
        ax.grid()
        fig.savefig("result.png")
        plt.show()

    def generate_filter_graph(self, df):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        ax.set(xlabel="Frequency Bin", ylabel="Count")
        ax.grid()
        fig.savefig("filter_result.png")
        plt.show()

    def generate_real_graph(self, df):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        ax.set(xlabel="Sample", ylabel="amplitude")
        ax.grid()
        fig.savefig("images/real_result.png")
        plt.show()

    def generate_graphs(self, df, name: str, domain: str):
        fig, ax = plt.subplots()
        ax.plot(df["Real"])
        if domain == "time":
            ax.set(xlabel="Sample", ylabel="amplitude")
        else:
            ax.set(xlabel="Frequency Bin", ylabel="Count")
        ax.grid()
        fig.savefig(f"images/{name}.png")
        plt.show()
