from pathlib import Path
import matplotlib
import numpy as np
import typing
import types
import os

class Graph:   
    log_path: str = os.path.join(os.getcwd(),"..","..","documentation/output/plots/before_fft_11025_real_signal.txt")
    
    def __init__(self):
        self


    def open_file(self):
        print(self.log_path)
        with open(self.log_path,"r") as file:
            count:int = 0
            for i in file:
                line = i.strip()
                print(f"{count}: {line}")
                count+=1
    
    def load_into_df(self):
        print("hello")
