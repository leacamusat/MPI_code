import os
import numpy as np
import seaborn as sb 
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

# Les données doivent être dans un fichier csv sous la forme 
# nb_processus,temps_maximum,taille
# 3,0.000085,8
# 3,0.000312,16
# 2,0.000043,8

csv_folder = input("subfolder:")
csv_file = input("csv file:")

try:
    main_path = Path.cwd().parents[1]
    file_path=f"{main_path}/shared_drive/{csv_folder}/{csv_file}.csv"
    num=pd.read_csv(file_path, delimiter=",")
except ValueError as e:
    print(e)

nom_algo="merge_sort"
size=4

try : 
    fig,ax=plt.subplots()
    sns=sb.barplot(data=num, x="nb_processus", y="temps_maximum", errorbar="sd", capsize=.4, palette="Blues")
    sns.axes.set_title("Barplot du temps d'exécution en fonction du nombre de processus "+nom_algo, fontsize=14)
    sns.set_xlabel("nombre de processus", fontsize=14)
    m=num.groupby('nb_processus')['temps_maximum'].mean()
    sns.set_ylabel("Temps d'exécution (en s)", fontsize=14)
    ax.plot(m.values,'r-o',linewidth=1)

    script_dir = os.path.dirname(__file__)
    results_dir = os.path.join(script_dir, 'output/')
    sample_file_name = "perf_"+nom_algo+"_size="+str(size)+".png"

    if not os.path.isdir(results_dir):
        os.makedirs(results_dir)

    plt.savefig(results_dir + sample_file_name)
except ValueError as e:
    print(e)
