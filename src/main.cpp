#include "NSGAII.h"
#include "input.h"
#include "distance.h"
#include <ctime>
using namespace std;
extern string problem;
extern string resultsrc;
extern string outputfilename;
extern string outputparetofilename;
extern string outputlog;
extern string outputgraph;
extern string outputparetoselectionfilename;
extern string outputtblog;
extern int total_node;
extern int canuseddrone;
extern int drone_max_tracks;
extern double maxobj1;
extern double maxobj2;
extern double minobj1;
extern double minobj2;
extern double obj1norm;
extern double obj2norm;
extern string testcount;
extern int crossoverMod;
extern vector<double> crossoverProportion;
int main(){
    srand(time(nullptr));
    cout<<"Test count:";
    //int testcountint;
    //cin>>testcountint;
    //cout<<outputlog<<endl;
    time_t start,end;
    ios_base::sync_with_stdio(false);
    input();
    drone_max_tracks=(int)(canuseddrone/num_drones)+1;
    total_node=num_cus+num_trucks+drone_max_tracks*num_drones-1;
    cout<<total_node<<" "<<drone_max_tracks<<endl;
    initMatrix(); 
    vector<Individual> population;
    population.clear();
    double timesave[6]={0,0,0,0,0,0};
    //population=selectPopulation(populationSize);

    //testcount=to_string(testcountint);
    for(int testcountint=1;testcountint<=5;testcountint++){
        testcount=to_string(testcountint);
        srand(time(nullptr));
        population=selectPopulation(populationSize);
    for(int i=6;i<12;i++){
        srand(time(nullptr));
        maxobj1=1000000000;
        maxobj2=1000000000;
        minobj1=0;
        minobj2=0;
        crossoverMod=50;
        crossoverProportion={1.0/3.0,1.0/3.0,1.0/3.0};
        time(&start);
        outputfilename="../result"+to_string(i+1)+"/"+problem+"/re"+testcount+txt;
        outputparetofilename="../result"+to_string(i+1)+"/"+problem +"/pa"+testcount+txt;
        outputparetoselectionfilename="../result"+to_string(i+1)+"/"+problem +"/se"+testcount+txt;
        outputlog="../result"+to_string(i+1)+"/"+problem +"/log"+testcount+txt;
        outputgraph="../result"+to_string(i+1)+"/"+problem +"/graphdata"+testcount+txt;
        outputtblog="../result"+to_string(i+1)+"/"+problem +"/tblog"+testcount+txt;
        std::ofstream outputFile3(outputtblog);
        std::ofstream outputFile(outputlog); 
        std::ofstream outputFile1(outputgraph);
        std::ofstream outputFile2(outputparetoselectionfilename);
        minobj1=population[0].fitness1;
        maxobj1=population[0].fitness1;
        minobj2=population[0].fitness2;
        maxobj2=population[0].fitness2;
        for(int j=0;j<population.size();j++){
            if(population[j].fitness1<minobj1) minobj1=population[j].fitness1;
            if(population[j].fitness2<minobj2) minobj2=population[j].fitness2;
            if(population[j].fitness1>maxobj1) maxobj1=population[j].fitness1;
            if(population[j].fitness2>maxobj1) maxobj1=population[j].fitness2;
        }
        obj1norm=maxobj1-minobj1;
        obj2norm=maxobj2-minobj2;
        vector<Individual> populationcopy;
        populationcopy.clear();
        populationcopy=population;
        vector<Individual> pareto;
        pareto.clear();
        pareto= NSGA2(populationcopy,i);;
        int paretosize=pareto.size();
        double minfit1=pareto[0].fitness1;
        double minfit2=pareto[paretosize-1].fitness2;
        double maxfit1=pareto[paretosize-1].fitness1;
        double maxfit2=pareto[0].fitness2;
        vector<ParetoRecord> paretoRecords;
        paretoRecords.clear(); 
        for(int j=0;j<pareto.size();j++){
            ParetoRecord paretoRecord;
            paretoRecord.indi=pareto[j];
            paretoRecord.normFit1=(pareto[j].fitness1-minfit1)/(maxfit1-minfit1);
            paretoRecord.normFit2=(pareto[j].fitness2-minfit2)/(maxfit2-minfit2);
            paretoRecords.push_back(paretoRecord);
        }   
        for(int j=0;j<3;j++){
            for(int k=0;k<paretosize;k++){
                paretoRecords[k].criteria=paretoRecords[k].normFit1*(j+1)*0.25+paretoRecords[k].normFit2*(3-j)*0.25;
            }
            sort(paretoRecords.begin(),paretoRecords.end(),compareCriteria);
            for(int k=0;k<paretosize;k++){
                paretoRecords[k].ranking+=k;
            }
            outputParetoResult(paretoRecords,j+1);
        }
        sort(paretoRecords.begin(),paretoRecords.end(),compareRanking);
        outputParetoResult(paretoRecords,0);
        time(&end);
        timesave[i-6]+=double(end-start);
        cout<<double(end-start)<<endl;
    }
    cout<<endl;
    }
    for(int i=0;i<6;i++){
        cout<<double(timesave[i]/5.0)<<" ";
    }
    cout<<endl;
    
    return 0;
}