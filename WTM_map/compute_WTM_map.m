%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% function [persistence_points,rho] = WTM_map_v2(net,T,foldername)
%
% input: net - is a struct giving properties of the network
%           net.A - is the adjacency matrix
%           net.A_geo - is the adjacency matrix of the geo net before
%           noisy links
%           net.N - is the number of nodes
%           net.M is the number of links
%           net.geometry is the node locations on the manifold
%        T - WTM threshold
%        foldername - where to save stuff
%
%
% output: activation_times 
% 
% DRT 1-17-2014
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function [runtime] = compute_WTM_map(T,compile,filename,AT_filename)

%% main line to change

%input:  T:          threshold
%        filename:   name of linklist, such as "LinkList.tsv"
%              this file is assumed to be in the same folder as the WTMmap
%              folder
%        compile:     if true then make "makefile" and compile


if compile
   % compile c++ code to do WTM map
   fid = fopen('WTM_map/Makefile', 'w');   
   %fprintf(fid,'# uncomment this code to convert node indices for the linklists\n\n');
   fprintf(fid,'all: compute_WTM_map_v3.cpp\n');
   fprintf(fid,'\tg++ -g -Wall -o compute_WTM_map_v3 compute_WTM_map_v3.cpp\n');
   fprintf(fid,'clean:\n');
   fprintf(fid,'\t$(RM) compute_WTM_map_v3');

   %run c++ code to generate file with activation times 
   cd WTM_map
   %uncomment first time
   unix('make');
   %unix(['./compute_WTM_map -t ',num2str(T),...
   %   ' -i ../../',foldername,'/LinkList.tsv -o ',...
   %   '../../',foldername,'/T=',num2str(T),'/activation_times.tsv']);
   cd ..
end

   cd WTM_map%set current folder as WTM_map
   tic
   unix(['./compute_WTM_map_v3 -t ',num2str(T),...
      ' -i ../',filename,...
      ' -o ../',AT_filename]);
   runtime = toc;% calculate run time for creating the WTM map
   cd ..%return out of the current folder, WTM_map

  % activation_time = load('activation_times.tsv');
   
   
%    q = zeros(1,net.N);
%    NUM_cluster = zeros(1,net.N);
%    activation_time = net.N*ones(1,net.N);
%    activation_time(find(s)) = zeros(size(find(s)));

% 
% 
% 
% 
%    % simulate contagion starting from each node      
%    seeds = 1:net.N;   
%    
%    % simulate contagion from only 1 node
%    %seeds = floor(net.N/2-sqrt(net.N)/2);
%          
%    
%    Runs=length(seeds);% the number of simulations to run
% 
%    
%    activation_time=zeros(Runs,net.N);%data matrix for activation times
%    q=zeros(Runs,net.N);%data matrix for contagion size as a function of time
%    NUM_cluster=zeros(Runs,net.N);%data matrix for number of clusters as a function of time
% 
%    parfor i=1:Runs       
%        %build initial condition
%        D=zeros(1,net.N); 
%        D([seeds(i);find(net.A(:,seeds(i)))])=1; %cluster seeding
%        
%        %simulate contagion
%        [D,q(i,:),activation_time(i,:),NUM_cluster(i,:)] = watts_model_binary(net,T,D); % Run the Watts' dynamics
% 
% %        if (i==length(Runs/2))
% %          disp('half done')
% %        end
%        
%    end
%    q = mean(q);
%    NUM_cluster = mean(NUM_cluster);   
% 
% 
%    save([foldername,'/data.mat']);
% 
% 




end









