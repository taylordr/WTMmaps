//Dane R Taylor December 3, 2014
//
// This code computes the full WTM map for a given network and threshold T
//
// input: -i linklist.tsv - contains the edge list with node indices 0,1,2,3,...
//        -t T - this is the threhsold value
//
// output -o activation_times.tsv - the matrix of activation times, where row j corresponds to a contagion initialized at node j
//
//
//    ./compute_WTM_map_v3 -t 0.1 -i LinkList.tsv -o activation_times_v3.tsv

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

char *infile   = NULL;
char *outfile  = NULL;
char *Tvalue = NULL;




double strict_str2double(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}





// output how to use this file
void usage(char *prog_name, const char *more) {
    cerr << more << endl;
    cerr << "usage: " << prog_name << " -t T -i input_file -j input_node_file -o outfile" << endl << endl;
    cerr << "read the graph and convert it to binary format." << endl;
    exit(0);
}

// save inputs to variables
void parse_args(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            switch(argv[i][1]) {
                case 'i':
                    if (i==argc-1)
                        usage(argv[0], "Infile missing\n");
                    infile = argv[i+1];
                    i++;
                    break;
                case 't':
                    if (i==argc-1)
                        usage(argv[0], "T value missing\n");
                    Tvalue = argv[i+1];
                    i++;
                    break;
                    //          case 'j':
                    //              if (i==argc-1)
                    //                  usage(argv[0], "Node Infile missing\n");
                    //              infile_node = argv[i+1];
                    //              i++;
                    //              break;
                case 'o':
                    if (i==argc-1)
                        usage(argv[0], "Outfile missing\n");
                    outfile = argv[i+1];
                    i++;
                    break;
                default:
                    usage(argv[0], "Unknown option\n");
            }
        } else {
            usage(argv[0], "More than one filename\n");
        }
    }
    if (infile==NULL || outfile==NULL)
        usage(argv[0], "In or outfile missing\n");
}

// main to run
int main(int argc, char **argv) {
    clock_t t;
    
    parse_args(argc, argv);
    
    double T = strict_str2double(Tvalue);
    //cerr << endl << "computing WTM maps for T = " << T << endl;
    
    ////////////////////////////////////////////////////////////////////////////////
    // first load Wiki_IDS into a vector called nodeNumbers
    ////////////////////////////////////////////////////////////////////////////////
    //    ifstream finput_node;
    //    finput_node.open(infile_node,fstream::in);
    //    char str[1256];
    //    vector <int> nodeID_list;
    //    int nodeNumber, nodeID, max_nodeID;
    //    max_nodeID=0;
    //    while (!finput_node.eof()) {
    //        finput_node >> nodeNumber >> nodeID >> str;
    //        nodeID_list.push_back(nodeID);
    //        if (max_nodeID < nodeID )
    //            max_nodeID = nodeID;
    //    }
    //    finput_node.close();
    //
    //    cerr << endl << "we loaded " << nodeID_list.size() << " node IDs into a vector" << endl ;
    //    cerr << "max node id =  " << max_nodeID << endl ;
    //
    //    vector <int> id_conversion_list;//huge vector where i-th entry is the new id of node i
    //    for (int i=0; i<max_nodeID; ++i) {
    //        id_conversion_list.push_back(0);
    //    }
    //    for (int i=0; i<nodeID_list.size(); ++i) {
    //        id_conversion_list[nodeID_list[i]] = i;
    //        //cerr << nodeID_list[i] << endl;
    //    }
    //    cerr << endl << "we created the conversion vector" << endl << endl;
    
    ////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    ////////////////////////////////////////////////////////////////////////////////
    // load link list and generate network
    ////////////////////////////////////////////////////////////////////////////////
    //open input and output link list files
    ifstream finput;
    finput.open(infile,fstream::in);
    
    //char str[1256];
    //finput.get(str, 1256);uncomment if first line of link list is "from, to"
    int max_nodeID=0;
    int src, dest;
    int count = 0;;
    vector < int> nodeID_list;
    vector < vector <int> > links;
    
    
    while (!finput.eof()) {
        finput >> src >> dest ;
        //cerr << src << " " << dest << endl;
        if (max_nodeID < src | max_nodeID < dest ) {
            max_nodeID = max(src,dest);
            links.resize(max_nodeID+1);
        }
        links[src].push_back(dest);
        links[dest].push_back(src);
        count++;
    }
    count = count-1;
    
    // cerr << "loaded: " << count << " undirected links and " << links.size() << " nodes" << endl;
    finput.close();
    
    
    
    
    ////////////////////////////////////////////////////////////////////////////////
    // run WTM model to generate WTM maps
    ////////////////////////////////////////////////////////////////////////////////
    
    
    //vector < unsigned short> boundary_nodes;// node ids for those on the boundary
    //vector < unsigned short> boundary_state;// 1 if node is in the boundary
    
    vector < unsigned short> changed_nodes;// node ids for those that changed state
    vector < unsigned short> old_changed_nodes;// node ids for those that changed state
    
    
    vector < vector <int> > activation_times;// activation times
    vector < unsigned short> adopted;// boolean vector 1/0 of whos adopted
    vector < unsigned short> adopted_old;//vector < int> unadopt_node_list;// list of nodes to check = possible
    bool no_change;// determine if nothing has changed
    //vector < vector <int> > contagion_sizes;// count number of changes versus time
    
    activation_times.resize(links.size());
    //contagion_sizes.resize(links.size());
    
    t = clock();
    
    
    
    //conduct filtrations starting at each node j
    for (int j=0; j<links.size(); ++j) {
    //for (int j=0; j<1; ++j) {
        
        //int j=0;
        
        //clear contagion size data
        //contagion_sizes[j].resize(links.size());
        
        //set adopted to be all zeros
        adopted.resize(links.size());
        for (int i=0; i<links.size(); ++i)
            adopted[i] = 0;
        
        //set up activation times vector for seed at node j
        for (int i=0; i<links.size(); ++i)
            activation_times[j].push_back(links.size()*2);
        
        
        //make the cluster seed include node j and its neighbors
        activation_times[j][j] = 0;
        adopted[j] = 1;
        for (int neighb=0; neighb<links[j].size(); neighb++) {
            adopted[ links[j][neighb] ] = 1;
            activation_times[j][ links[j][neighb] ] = 0;
        }
        //save the size of initial contagion at time t=0
        
        //contagion_sizes[j][0] = (1+links[j].size());
        //boundary_nodes.clear();
        changed_nodes.clear();
        old_changed_nodes.clear();
        
        ////////////////////////////////////////////////////////
        //added on Dec. 2, 1014 DRT
        ////////////////////////////////////////////////////////
        
        //clear boundary state variable to zeros
        //boundary_state.resize(links.size());
        //for (int i=0; i<links.size(); ++i)
        //    boundary_state[i] = 0;
        
        //mark seed nodes as having been in boundary
        //boundary_state[j] = 1;//obviously the seed has been in the boundary
        //for (int neighb=0; neighb<links[j].size(); neighb++) //node j's neighbors
        //    boundary_state[ links[j][neighb] ] = 1;//obviously the seed has been in the boundary
        
        //add neighbors of contagion seed to boundary
        for (int neighb=0; neighb<links[j].size(); neighb++) {//node j's neighbors
            //  cerr << "    neighbor to check " << links[j][neighb] << endl;
            changed_nodes.push_back( links[j][neighb] );
        }
//            for (int neighb2=0; neighb2<links[ links[j][neighb] ].size(); neighb2++) {//their neighbors
//                int node2check = links[ links[j][neighb] ][neighb2];
//                if ( boundary_state[ node2check ] == 0 ) {//if yet to be in boundary
//                    changed_nodes.push_back( node2check );//add to changed nodes
//                    //boundary_nodes.push_back( node2check );//add to boundary
//                    //boundary_state[ node2check ] = 1;//mark that they've been in boundary
//                    //  cerr << "       neighbors to added to boundary " << links[links[j][neighb]][neighb2] << endl;
//                }
//            }
//        }
        ////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////
        
        //
        //        ////////////////////////////////////////////////////////
        //        //monitoring
        //        ////////////////////////////////////////////////////////
        //        cerr  << "    infected nodes = [";
        //        for (int i=0; i<links.size(); i++) {
        //            if (adopted[i]==1)
        //                cerr  << i << ", ";
        //        }
        //        cerr  << "]" << endl << "    boundary nodes = [" ;
        //        for (int i=0; i<boundary_nodes.size(); i++)
        //            cerr  << boundary_nodes[i]  << ", ";
        //        //cerr  << "]" << endl << "    boundary nodes2 = [" ;
        //        //for (int i=0; i<links.size(); i++)
        //        //    if (boundary_state[i]==1)
        //        //        cerr  << i << ", ";
        //        cerr << endl ;
        //        ////////////////////////////////////////////////////////
        //        ////////////////////////////////////////////////////////
        //
        //
        
        
        //simulate WTM to get activation times
        for (int t=1; t<links.size(); ++t) {
        //for (int t=1; t<4; ++t) {//time t
            
            
            no_change = true;//assume there will be no change
            adopted_old = adopted;
            
            //cerr  << endl << changed_nodes.size() << " nodes changed at t=" << t << ":  " ;

            //            vector < unsigned short> boundary_nodes = boundary_nodes;
            
            old_changed_nodes = changed_nodes;
            //cerr  << endl << changed_nodes.size() << " nodes changed at t=" << t << ":  " ;

            changed_nodes.clear();
            //cerr  << endl << changed_nodes.size() << " nodes changed at t=" << t << ":  " ;
            //unsigned int qwer = old_changed_nodes.size();
            //cerr  << endl << "qwer=" << qwer << ":  " ;
            if (old_changed_nodes.size() > 0) {
                
                //cerr  << endl << old_changed_nodes.size() << " nodes changed at t=" << t << ":  " ;
               
                //go through nodes that changed stat during last timestep
                for (int k=0; k<old_changed_nodes.size(); ++k) {
                    //cerr  << old_changed_nodes[k] << ", ";

                    //consider his neighbors
                    for (int l=0; l<links[ old_changed_nodes[k] ].size(); ++l) {
                        
                    int node2check = links[ old_changed_nodes[k] ][l];
                        
                        if (adopted[node2check]==0) {//if not adopted
                            
                            //computed number of neighbors that have adopted
                            int num_neighb_adopted = 0;
                            for (int neighb=0; neighb<links[ node2check ].size(); neighb++) {
                                num_neighb_adopted += adopted_old[ links[ node2check ][neighb]];
                            }
                            
                            //if fraction surpasses threshold
                            if ( ((double)num_neighb_adopted / (double)links[node2check].size() ) >T) {
                                adopted[node2check] = 1;//node adopts contagion
                                //adopted[ boundary_nodes[k] ] = 1;//node k adopts contagion
                                changed_nodes.push_back(node2check);//add node to set that changed this timestep
                                activation_times[j][node2check] = t;//save activation time
                                no_change = false;
                                
    //                            //add its neighbors to the boundary
    //                            for (int neighb=0; neighb<links[ boundary_nodes[k] ].size(); neighb++) {//their neighbors
    //                                node2check = links[ boundary_nodes[k] ][neighb];
    //                                if (boundary_state[  node2check ]==0) {//if yet to be in boundary
    //                                    //                            if (node2check == j){
    //                                    //                                cerr << " something happened 2 !!! " << endl;
    //                                    //                            }
    //                                    boundary_nodes.push_back(node2check);//add to boundary
    //                                    boundary_state[node2check] = 1;//mark that its been in boundary
    //                                }
    //                            }
                                
                                //remove node boundary_nodes[k] from the list of boundary nodes
                                //boundary_nodes.erase(boundary_nodes.begin()+k);
                                
                            }
                            
                        }
                    }
                }

                //cerr  << endl << changed_nodes.size() << " nodes changed at t=" << t << ":  " ;

                
            }
            //if (no_change) {
            //    break;
            //}
            
            //            ////////////////////////////////////////////////////////
            //            //monitoring
            //            ////////////////////////////////////////////////////////
            //            cerr  << endl << "t = " << t << endl;
            //            cerr  << "    infected nodes = [";
            //            for (int i=0; i<links.size(); i++) {
            //                if (adopted[i]==1)
            //                    cerr  << i << ", ";
            //            }
            //            cerr  << "]" << endl << "    boundary nodes = [" ;
            //            for (int i=0; i<boundary_nodes.size(); i++)
            //                cerr  << boundary_nodes[i] << ", ";
            //            //cerr  << "]" << endl << "    boundary nodes2 = [" ;
            //            //for (int i=0; i<links.size(); i++)
            //            //    if (boundary_state[i]==1)
            //            //        cerr  << i << ", ";
            //            cerr << endl ;
            //            ////////////////////////////////////////////////////////
            //            ////////////////////////////////////////////////////////
            
            
            
        }
    }
    //cerr  << endl << "adsfasdf";


    //save activation time matrix to a file
    ofstream foutput;
    foutput.open(outfile);
    for (int j=0; j<links.size(); ++j) {
        for (int i=0; i<links.size(); ++i) {
            foutput << activation_times[j][i];
            if (i<links.size()-1)
                foutput << "\t";
            else
                foutput << endl;
        }
    }
    foutput.close();
    //cerr  << endl << "adsfasdf";
    
    //cerr  << "saved WTM mapped coordinates in " << (clock() - t)/CLOCKS_PER_SEC << " seconds" << endl << endl;
    
    
    return 0;
}

