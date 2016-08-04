
%% Preliminaries

addpath(genpath('WTM_map'))


%% Define Experiment parameters

%compute WTM map for T=0.2;
T =0.1;

%yes compile the c++ code that
compile = 1;%set to 0 after you run it once on your computer to speed code

%the linklist for the network is in file "LinkList.tsv
linklist_filename ='LinkList.tsv';

%save the activation times in file "NoisyRingLattice_T=02.tsv"
activationtime_filename = 'NoisyRingLattice_T=02.tsv';

%% Compute WTM
disp(' ');
disp('Running compute_WTM_map')
run_time = compute_WTM_map(T,compile,linklist_filename,activationtime_filename);
disp(['time_elapsed = ',num2str(run_time)]);
disp(' ');

%% Load Results
disp('Loading activation times')

%load activation times for this experiment
activation_times = load(activationtime_filename);

%view the activation time matrix as an image
imagesc(activation_times);
