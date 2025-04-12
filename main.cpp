#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <fstream>
#include <sstream>
#include <chrono>

// Real problem class
#include "pinfluencer.h"

// All all algorithms
#include "greedy.h"
#include "randomsearch.h"
#include "lsall.h"
#include "BLsmall.h"

//P2

#include "population.h"
#include "selectop.h"
#include "cross_unordered.h"
#include "cross_ordered.h"
#include "AGG.h"
#include "mutateOp.h"

#include "AGE.h"
#include "AG.h"
#include "AM.h"

using namespace std;

// Function to print usage instructions
void printUsage(const char* progName) {
  std::cerr << "Usage: " << progName << " [options]\n\n"
            << "Options:\n"
            << "  -f <file_spec>   Specify file number (0, 1, 2, 3) or 4 for all. Default: 0\n"
            << "  -a <algo_spec>   Specify algorithm (random, local, local_optimized, greedy) or 'all'. Default: random\n"
            << "  -t <output_spec> Specify output mode (terminal, document, both). Default: terminal\n"
            << "  -h, --help       Show this help message\n\n"
            << "Example:\n"
            << "  " << progName << " -f 1 -a greedy -o both\n";
}

int main(int argc, char *argv[]) {
  
  long int seed = 42;
  int nfile = 0;
  std::string algoSpec = "random";
  std::string outputMode = "";

  //Parameter input processing
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
        printUsage(argv[0]);
        return 0; 
    } else if (arg == "-f") {
        if (i + 1 < argc) {
          nfile = atoi(argv[++i]);
        }
    } else if (arg == "-a") {
        if (i + 1 < argc) {
            algoSpec = argv[++i];
        }
    } else if (arg == "-t") {
        outputMode = "terminal";
    } else {
        std::cerr << "Error: Unknown option '" << arg << "'" << std::endl;
        printUsage(argv[0]);
        return 1; 
    }
  }

  //files for study
  string files[] = {
    "../datos/ca-GrQc.txt",
    "../datos/p2p-Gnutella05.txt",
    "../datos/p2p-Gnutella08.txt",
    "../datos/p2p-Gnutella25.txt"
  };

  //Prefixed seeds for deterministic results
  int seeds[] = {452987,34655,32346789,2854,2359834};

  //Algorithms
  RandomSearch ralg = RandomSearch();
  LocalSearchLl lsall = LocalSearchLl();
  LocalSearchEarlyStop blsmall = LocalSearchEarlyStop();
  GreedySearch rgreedy = GreedySearch();
  
  Random::seed(seed); 

  //File choosing logic
  int from_file;
  int to_file = nfile;
  if(nfile == 4){
    from_file=0;
    to_file--;
  }
  else  
    from_file=nfile;

  //Procesing of algorithms for each files
  for(int f = from_file; f <= to_file; ++f){
    
    ifstream file(files[f]);
    //Inizialize problem for ythe given file 
    ProblemInflu pi = ProblemInflu(10,file);

    //Logic for algorithms choosing
    vector<pair<string, MH *>> algoritmos;
    if(algoSpec == "random"){
      algoritmos.push_back(make_pair("RandomSearch", &ralg));
    }else if(algoSpec == "local"){
      algoritmos.push_back(make_pair("LocalSearchLl",&lsall));
    }else if(algoSpec == "localsmall"){
      algoritmos.push_back(make_pair("LocalSearchEarlyStop",&blsmall));
    }else if(algoSpec == "greedy"){
      algoritmos.push_back(make_pair("Greedy",&rgreedy));
    }else{
      algoritmos.push_back(make_pair("RandomSearch", &ralg));
      algoritmos.push_back(make_pair("LocalSearchLl",&lsall));
      algoritmos.push_back(make_pair("LocalSearchEarlyStop",&blsmall));
      algoritmos.push_back(make_pair("Greedy",&rgreedy));
    }

    Problem *problem = dynamic_cast<Problem *>(&pi);
    
    //Output for the file
    string output = "../datos/output_";
    output.append(files[f].substr(9));
    ofstream out(output);


    /*
    
    //Compute algorithm
    for (int i = 0; i < algoritmos.size(); i++) {    

      std::cout << algoritmos[i].first << endl;

      //Select algorithm
      MH *mh = algoritmos[i].second;
      
      tFitness meanFit = 0;
      tSolution bestSol ;
      tFitness bestFitness = 0;
      int meanEvals = 0;

      std::chrono::duration<double> meanTime;

      //Compute 5 times and take mean to minimize randomization
      for(int j = 0; j < 5; ++j){
        //Set of random seed
        Random::seed(seeds[j]);

        //Compute algorithm and get time info
        auto inicio = std::chrono::system_clock::now();
        ResultMH res(mh->optimize(problem, 1000));
        auto fin = std::chrono::system_clock::now();
        meanTime += fin - inicio;
    
        meanFit += res.fitness;
        meanEvals += res.evaluations;

        if(res.fitness > bestFitness){
          bestFitness = res.fitness;
          bestSol = res.solution;
        }
      }

      //File output
      out << algoritmos[i].first << endl;
      out << "Best solution:    " << bestSol << endl;
      out << "Best fitness:     " << bestFitness << endl;
      out << "Mean fitness:     " << (float)meanFit/5.0 << endl;
      out << "Computation time: " << meanTime.count()/5.0 << " seconds" << endl;
      out << "Evaluations:      " << (float)meanEvals/5.0 << endl;

      //Terminal output
      if(outputMode == "terminal"){
        std::cout << "File:             " << files[f].substr(9) << endl;
        std::cout << "Best solution:    " << bestSol << endl;
        std::cout << "Best fitness:     " << bestFitness << endl;
        std::cout << "Mean fitness:     " << (float)meanFit/5.0 << endl;
        std::cout << "Computation time: " << meanTime.count()/5.0 << " seconds" << endl;
        std::cout << "Evaluations:      " << (float)meanEvals/5.0 << endl << endl;  
      }
    }

    */
/*
    Population pop(10);

    for(int i = 0; i < 10; ++i){
      tSolution sol = pi.createSolution();
      pop.push_back_sol(sol,pi.fitness(sol));
    }

    uniform_int_distribution<int> dist_int(0, pop.getPopulationSize() - 1);
    SelectOp sel;
    for(int i = 0; i < 10; ++i){
      std::cout << sel.select(pop) << endl;
    }
*/
/*
    tSolution sol1 = pi.createSolution();
    tSolution sol2 = pi.createSolution();

    cout << sol1 << " " << sol2 << endl;

    CrossUnordered cross;
    CrossOrdered ordCross;

    ordCross.cross(sol1,sol2);

    cout << sol1 << " " << sol2 << endl;

        AG ag(cross_operators::unordered,problem);

    cout << "POP0 inic" << endl;
    for(int i = 0; i < ag.getPopulation(0).getPopulationSize(); ++i){
      cout << ag.getPopulation(0).getSolution(i) << endl;
    }
    ag.select_population();

    cout << "POP1 cross" << endl;
    for(int i = 0; i < ag.getPopulation(1).getPopulationSize(); ++i){
      cout << ag.getPopulation(1).getSolution(i) << endl;
    }
    cout << endl;
    ag.cross_population(problem);

    cout << "POP1 cross" << endl;

    for(int i = 0; i < ag.getPopulation(1).getPopulationSize(); ++i){
      cout << ag.getPopulation(1).getSolution(i) << endl;
    }
  }
*/

    //    Fase, Percentage of pop,  ordered,  evals per LS, crossover,                    problem
    AM am(10,   1,                  true,     20,           AG::cross_operators::ordered, problem);

    cout << endl << "am: " << am.optimize(problem,1000).fitness;
    //AGE ageu(AG::cross_operators::unordered,problem);

    //cout << endl << "age unord: " << ageu.optimize(problem,1000).fitness;

    //AM ago(AG::cross_operators::ordered,problem);

    //cout << endl << "agg ord: " << ago.optimize(problem,1000).fitness;

    //AGE ageo(AG::cross_operators::ordered,problem);

    //cout << endl << "age ord: " << ageo.optimize(problem,1000).fitness;
  }

  return 0;
}
