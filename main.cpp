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
            << "  -a <algo_spec>   Specify algorithm (AGG,AGE,AM) or 'all'. Default: random \n"
            << "  -o <cross_op>    Specify cross operator (ordered,unordered) or 'all'. Default: all \n"
            << "  -t               Enable terminal output                                   \n"
            << "  -s <seed file>   Set the problem seeds to the one given in the seed file. \n"
            << "  -h, --help       Show this help message                                 \n\n"
            << "Example:                                                                    \n"
            << "  " << progName << " -f 1 -a AGG                                            \n";
}

void usualPrint(std::ostream &out, string name, tSolution bestSol, tFitness bestFitness, float meanFit, std::chrono::duration<double> meanTime, float meanEvals ){
  out << name << endl;
  out << "Best solution:    " << bestSol << endl;
  out << "Best fitness:     " << bestFitness << endl;
  out << "Mean fitness:     " << meanFit/5.0 << endl;
  out << "Computation time: " << meanTime.count()/5.0 << " seconds" << endl;
  out << "Evaluations:      " << meanEvals/5.0 << endl << endl;
}

void tablePrint(std::ostream &out, string name, tFitness bestFitness, float meanFit, std::chrono::duration<double> meanTime, float meanEvals ){
  out << 
  name << "," << 
  bestFitness << "," << 
  meanFit/5.0 << "," << 
  meanTime.count()/5.0 << "," <<
  meanEvals/5.0 << endl; 
}

int main(int argc, char *argv[]) {
  
  long int seed = 42;
  int nfile = 0;
  std::string algoSpec = "all";
  std::string outputMode = "";

  std::string seedsFile = "../datos/seeds.txt";
  std::string cross_op = "ordered";

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
    } else if (arg == "-s") {
        if (i + 1 < argc) {
            seedsFile = argv[++i];
        }
    } else if (arg == "-o") {
        if (i + 1 < argc) {
          cross_op = argv[++i];
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

  //Prefixed seeds for deterministic results
  std::vector<int> seeds;
  std::ifstream file(seedsFile);

  if (!file.is_open()) {
      std::cerr << "Error: Unable to open file " << seedsFile << std::endl;
      return 1; // Return empty vector on error
  }

  int num;
  while (file >> num) {
      seeds.push_back(num);
  }

  file.close();

  if(seeds.size() != 5){
    std::cerr << "Error: Wrong number of seeds given. There are "  << seeds.size() << "seeds but has to be 5" << std::endl;
    return 1;
  }

  //File choosing logic
  int from_file;
  int to_file = nfile;
  if(nfile == 4){
    from_file=0;
    to_file--;
  }
  else  
    from_file=nfile;


    int LSfase = 10;
    float LSpercentage = 1;
    bool orderPob = true;
    int LSmaxEvals = 20; 

  //Procesing of algorithms for each files
  for(int f = from_file; f <= to_file; ++f){
    
    ifstream file(files[f]);
    //Inizialize problem for ythe given file 
    ProblemInflu pi = ProblemInflu(10,file);

    Problem *problem = dynamic_cast<Problem *>(&pi);

    AGG agg_o(AG::cross_operators::ordered,problem);
    AGE age_o(AG::cross_operators::ordered,problem);
    AM  am_o(LSfase,LSpercentage,orderPob,LSmaxEvals,AG::cross_operators::ordered,problem);
    AGG agg_u(AG::cross_operators::unordered,problem);
    AGE age_u(AG::cross_operators::unordered,problem);
    AM  am_u(LSfase,LSpercentage,orderPob,LSmaxEvals,AG::cross_operators::unordered,problem);
  
    //Logic for algorithms choosing
    vector<pair<string, MH *>> algoritmos;
    if(algoSpec == "AGG"){
      if(cross_op == "ordered" || cross_op == "all")
        algoritmos.push_back(make_pair("Genetic generational ordered", &agg_o));
      if(cross_op == "unordered" || cross_op == "all")
        algoritmos.push_back(make_pair("Genetic generational unordered", &agg_u));
    }else if(algoSpec == "AGE"){
      if(cross_op == "ordered" || cross_op == "all")
        algoritmos.push_back(make_pair("Genetic stationary ordered",&age_o));
      if(cross_op == "unordered" || cross_op == "all")
        algoritmos.push_back(make_pair("Genetic stationary unordered",&age_u));
    }else if(algoSpec == "AM"){
      if(cross_op == "ordered" || cross_op == "all")
        algoritmos.push_back(make_pair("Memetic ordered",&am_o));
      if(cross_op == "ordered" || cross_op == "all")
        algoritmos.push_back(make_pair("Memetic unoredered",&am_u));
    }else{
      if(cross_op == "ordered" || cross_op == "all"){
        algoritmos.push_back(make_pair("Genetic generational ordered", &agg_o));
        algoritmos.push_back(make_pair("Genetic stationary ordered",&age_o));
        algoritmos.push_back(make_pair("Memetic ordered",&am_o));
      }

      if(cross_op == "ordered" || cross_op == "all"){
        algoritmos.push_back(make_pair("Genetic generational unordered", &agg_u));
        algoritmos.push_back(make_pair("Genetic stationary unordered",&age_u));
        algoritmos.push_back(make_pair("Memetic unoredered",&am_u));
      }
    }
    
    //Output for the file
    string output = "../datos/output_";
    output.append(files[f].substr(9));
    ofstream out(output);
    out << "name,bestFitness,meanFit,meanTime,meanEvals" << endl;

    //Compute algorithm
    for (int i = 0; i < algoritmos.size(); i++) {    
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

      //usualPrint(out,algoritmos[i].first,bestSol,bestFitness,(float)meanFit,meanTime,(float)meanEvals);
      tablePrint(out,algoritmos[i].first,bestFitness,(float)meanFit,meanTime,(float)meanEvals);
      //Terminal output
      if(outputMode == "terminal"){
        usualPrint(std::cout,algoritmos[i].first,bestSol,bestFitness,(float)meanFit,meanTime,(float)meanEvals);
      }
    }

  }

  return 0;
}
