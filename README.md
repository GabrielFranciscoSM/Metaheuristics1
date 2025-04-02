# 📢 SNIMP - Social Network Influence Maximization Problem

## 🚀 Overview

This project tackles the **Social Network Influence Maximization Problem (SNIMP)**. Imagine trying to model how a virus spreads or how influential people impact public opinion. That's essentially what SNIMP is about!

The core challenge is to identify a small group of "seed" nodes within a large network. These seed nodes, when activated, should maximize the spread of "influence" (think of it as a cascade effect) across the entire network.

### 🧩 Problem Components

* **Influence Graph:** The network itself, represented as a directed graph. Connections between individuals (nodes) are the edges.
* **Influential Subset:** The crucial set of nodes we select to start the influence process.
* **Influence Probability:** The likelihood that one node will "influence" a connected node.

### 💡 Solution Representation

We represent potential solutions as a list of unique node IDs. It's all about picking the right nodes, and we don't want to pick the same node twice!

### 📈 Solution Evaluation

To see how well a solution performs, we use a cascade model. We simulate how influence spreads step-by-step and measure the final number of "influenced" nodes.

## 🤖 Algorithms

This project implements and compares the following algorithms:

1.  **🎲 Random:** A baseline approach that generates solutions randomly.
2.  **🔍 Local Search All (LSall):** A local search algorithm that exhaustively explores neighboring solutions.
3.  **⚡ Local Search Small (LSsmall):** An optimized local search that tries to avoid getting stuck by detecting when it's not improving.
4.  **🧠 Greedy:** A smart algorithm that builds a solution step-by-step, making the best choice at each step based on a heuristic.

### 🔍 Algorithm Details

####   🎲 Random

This algorithm is straightforward: generate random solutions and see how they perform. It helps us understand how much better (or worse) the other algorithms are.

* **Complexity:** O(1)

####   🔍 Local Search All

Local Search All starts with a random solution and then tries to improve it by making small changes. It keeps searching as long as it can find better solutions.

* **Complexity:** O(1) per iteration (but each iteration can be computationally intensive).

####   ⚡ Local Search Small

Local Search Small is a variant of Local Search All that tries to be more efficient. It stops searching if it doesn't find improvements for a while, assuming it has reached a good-enough solution.

* **Complexity:** O(1) (and generally more efficient than Local Search All).

####   🧠 Greedy

The Greedy algorithm takes a different approach. It builds a solution by repeatedly selecting the node that seems most promising based on a smart rule (heuristic). In our case, the heuristic prioritizes nodes with many connections.

* **Complexity:** O(n) on average, but up to O(n²) in the worst-case (very densely connected networks).

## 🗂️ Project Structure

Here's how the project files are organized:

* `build/`:  Where the compiled program ends up.
* `common/`: Shared code elements.
* `inc/`:   Header files for the algorithms and problem definition.
* `src/`:   The actual code (.cpp files) for the algorithms and problem.
* `data/`:  The network data files.

## 🧑‍💻 Usage

To get the project running:

1.  Go to the `build/` directory.
2.  Compile the code: `make`
3.  Run the program: `./main`

### ⚙️ Parameters

You can customize how the program runs with these options:

* `-f <dataset>`: Choose the network dataset:
    * `0` (default): `ca-GrQc.txt`
    * `1`: `p2p-Gnutella05.txt`
    * `2`: `p2p-Gnutella08.txt`
    * `3`: `p2p-Gnutella25.txt`
    * `4`: Run with all datasets
* `-a <algorithm>`: Select the algorithm:
    * `"random"` (default)
    * `"local"`
    * `"localsmall"`
    * `"greedy"`
    * `"all"`
* `-t`: Enable output to the terminal in addition to files.

Output files are named `output {dataset name}` and are saved in the `build/` directory.

### 🚀 Examples

* `./main -f 4 -a all -t`: Run all algorithms on all datasets, with both file and terminal output.
* `./main`: Run the Random algorithm on the default dataset (`ca-GrQc.txt`), with file output.
* `./main -f 1 -a all -t`: Run all algorithms on the `p2p-Gnutella05.txt` dataset, with both file and terminal output.
* `./main -f 4 -a greedy`: Run the Greedy algorithm on all datasets.

## 📊 Results

### 📂 Datasets

We used these network datasets for our experiments:

| Dataset        | Nodes | Edges | Edges/Node |
| :------------- | :---- | :---- | :--------- |
| ca-GrQc        | 5242  | 28980 | 5.5284     |
| p2pGnutella05  | 8846  | 31839 | 3.5992     |
| p2pGnutella08  | 6301  | 20777 | 3.2974     |
| p2pGnutella25  | 22687 | 54705 | 2.4112     |

### 🧪 Experimental Setup

To make sure our results were reliable:

* We ran each algorithm 5 times with different starting points (seeds).
* The seeds used were: 452987, 34655, 32346789, 2854, 2359834.
* We simulated the influence spread 10 times for each solution and used the average result.
* We used a local random number generator with a seed of 234670.
* We generated random numbers using a uniform distribution.
* We used the `chrono` library for precise time measurements.

### 📈 Results Summary

####   🎲 Random

| Dataset        | Fitness | Time (secs) | Evaluations |
| :------------- | :------ | :---------- | :---------- |
| ca-GrQc        | 12.28   | 0.248025    | 1000        |
| p2pGnutella05  | 11.52   | 0.109603    | 1000        |
| p2pGnutella08  | 11.24   | 0.168408    | 1000        |
| p2pGnutella25  | 11      | 0.157762    | 1000        |

####   🔍 Local Search All

| Dataset        | Fitness | Time (secs) | Evaluations |
| :------------- | :------ | :---------- | :---------- |
| ca-GrQc        | 15.42   | 1.06486     | 1000        |
| p2pGnutella05  | 11.9    | 0.394242    | 1000        |
| p2pGnutella08  | 11.58   | 0.394101    | 1000        |
| p2pGnutella25  | 11.74   | 0.400543    | 1000        |

####   ⚡ Local Search Small

| Dataset        | Fitness | Time (secs) | Evaluations |
| :------------- | :------ | :---------- | :---------- |
| ca-GrQc        | 11.98   | 1.07604     | 45.6        |
| p2pGnutella05  | 11.26   | 0.403046    | 48.4        |
| p2pGnutella08  | 10.62   | 0.399495    | 37.4        |
| p2pGnutella25  | 10.66   | 0.40564     | 40.4        |

####   🧠 Greedy

| Dataset        | Fitness | Time (secs) | Evaluations |
| :------------- | :------ | :---------- | :---------- |
| ca-GrQc        | 17.8    | 1.08279     | 1           |
| p2pGnutella05  | 13.6    | 0.409677    | 1           |
| p2pGnutella08  | 12.6    | 0.404025    | 1           |
| p2pGnutella25  | 13.3    | 0.419634    | 1           |

### 🧐 Analysis

* The `ca-GrQc` dataset stands out with different results, likely because it has more connections between nodes.
* The number of neighbors a node has is a key factor in how the algorithms perform.
* The influence spread tends to be localized.
* Greedy generally achieves the best results.
* Local Search Small often underperforms compared to Random due to stopping too early.

### 🏆 Algorithm Comparison

| Algorithm        | Average Position | Average Time (secs) | Average Evaluations |
| :--------------- | :--------------- | :------------------ | :------------------ |
| Random           | 4                | 0.170925            | 1000                |
| Local Search All | 2                | 0.563425            | 1000                |
| Local Search Small| 4                | 0.571025            | 42.65               |
| Greedy           | 1                | 0.5790              | 1                   |

## 🎉 Conclusions

* The Greedy algorithm is the overall winner.
* Local search methods can struggle to find significant improvements quickly.
* The structure of the network (how nodes are connected) greatly impacts the results.
