% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v2.02, Jan 25, 2017


\title{Visual Analysis the Traversability of Protein Tunnels by Flexible Ligands}

% for anonymous conference submission please enter your SUBMISSION ID
% instead of the author's name (and leave the affiliation blank) !!
% for final version: please provide your *own* ORCID in the brackets following \orcid; see https://orcid.org/ for more details.
\author[A. Jur\v{c}\'{i}k et al.]
{\parbox{\textwidth}{\centering Adam Jur\v{c}\'{i}k$^{1}$,
        Vojt\v{e}ch Von\'{a}sek$^{2}$,
				Martin Ma\v{n}\'{a}k$^{3}$,
				and Barbora Kozl\'{i}kov\'{a}$^{1}$
%        S. Spencer$^2$\thanks{Chairman Siggraph Publications Board}
        }
        \\
% For Computer Graphics Forum: Please use the abbreviation of your first name.
{\parbox{\textwidth}{\centering $^1$Masaryk University, Brno, Czech Republic\\
         $^2$Czech Technical University, Prague, Czech Republic\\
        $^3$University of West Bohemia, Pilsen, Czech Republic
%             with different affiliations
       }
}
}
% ------------------------------------------------------------------------

% if the Editors-in-Chief have given you the data, you may uncomment
% the following five lines and insert it here
%
% \volume{36}   % the volume in which the issue will be published;
% \issue{1}     % the issue number of the publication
% \pStartPage{1}      % set starting page


%-------------------------------------------------------------------------
\begin{document}
% uncomment for using teaser
% \teaser{
%  \includegraphics[width=\linewidth]{eg_new}
%  \centering
%   \caption{New EG Logo}
% \label{fig:teaser}
%}
\maketitle
%-------------------------------------------------------------------------
\begin{abstract}
Analysis of accessibility of protein inner space by a ligand helps the biochemists to understand the protein structure and behavior and design desired chemical reactions.
In our research, we develop methods for planning almost collision-avoiding paths for flexible ligands in dynamic protein models. 
We join computational geometry concepts, path-planning approaches, and advanced visualization techniques to target this problem.
\end{abstract}
%-------------------------------------------------------------------------
%  ACM CCS 1998
%  (see http://www.acm.org/about/class/1998)
% \begin{classification} % according to http:http://www.acm.org/about/class/1998
% \CCScat{Computer Graphics}{I.3.3}{Picture/Image Generation}{Line and curve generation}
% \end{classification}
%-------------------------------------------------------------------------
%  ACM CCS 2012   (see http://www.acm.org/about/class/class/2012)
%The tool at \url{http://dl.acm.org/ccs.cfm} can be used to generate
% CCS codes.
%Example:
%\begin{CCSXML}
%<ccs2012>
%<concept>
%<concept_id>10010147.10010371.10010352.10010381</concept_id>
%<concept_desc>Computing methodologies~Collision detection</concept_desc>
%<concept_significance>300</concept_significance>
%</concept>
%<concept>
%<concept_id>10010583.10010588.10010559</concept_id>
%<concept_desc>Hardware~Sensors and actuators</concept_desc>
%<concept_significance>300</concept_significance>
%</concept>
%<concept>
%<concept_id>10010583.10010584.10010587</concept_id>
%<concept_desc>Hardware~PCB design and layout</concept_desc>
%<concept_significance>100</concept_significance>
%</concept>
%</ccs2012>
%\end{CCSXML}

%\ccsdesc[300]{Computing methodologies~Collision detection}
%\ccsdesc[300]{Hardware~Sensors and actuators}
%\ccsdesc[100]{Hardware~PCB design and layout}


%\printccsdesc   
%\end{abstract}  
%-------------------------------------------------------------------------
\section{Introduction}
Chemical interactions between proteins and other molecules (ligands) take place in specific locations, called active sites, that are often buried deep inside the protein structure.
%These active sites are accessible through one or more void paths, called tunnels.
We present a motion-planning approach to the calculation of the transportation path for the ligand to the active site.
The main benefit is that we consider the flexibility of ligand, i.e., its conformational changes when traversing the protein.
Our solution utilizes the additively weighted Voronoi diagrams for localizing the configuration space for the motion planner.
The calculated trajectories have to be subsequently analyzed by the biochemists to assess their biochemical relevance.
For this, we calculate the energy function of each trajectory and design specific visualization techniques, helping to understand the trajectory properties.
We demonstrate the usability of our approach on an exemplary dataset, obtained from our domain experts.


\section{Our Approach}
Our proposed solution works with the van der Waals model, which is given by centers $\mathbf{c}_i \in R^3$ and radii $r_i \in R$ of individual atoms. 
An example with a ligand bound to the protein is depicted in Fig.~\ref{fig:vd}. 
Dynamic models add the movement of atoms by specifying positions in subsequent time-snapshots.			  

We utilize the additively weighted Voronoi diagram (AWD), which is a collection of regions $V\!R_i$ assigned to the corresponding atoms. 
Voronoi vertices and edges provide us with the description of free space between atoms with respect to a spherical probe, see Fig.~\ref{fig:vd}.
\[V\!R_i = \{x \in \mathbf{R}^3 \,:\,|\mathbf{x}-\mathbf{c}_i|-r_i \leq |\mathbf{x}-\mathbf{c_j}|-r_j\,\forall j \in I_n \}\]

\begin{figure}[tb]
\centering
\includegraphics[height=10em]{figures/4b1m}
\hspace{1em}
\includegraphics[height=10em]{figures/awd}
\caption{Van der Waals model with a ligand (PDB ID: 4B1M); \mbox{Voronoi} diagram for the analysis of free space by spherical probes}
\label{fig:vd}
\end{figure}

Our program first computes AWD for each time-snapshot of the dynamic model and then offers this data to the motion planning part, because paths that are feasible for a single probe can also be feasible for more complex ligands. 
The path planning method launched on the constructed AWD can be used for the identification of tunnels in proteins that represent possible pathways for a single probe. 
To compute the trajectories for non-spherical ligands, considering their flexibility, we can apply the motion planning techniques, originally studied in the field of robotics.
The ligand is considered as a flexible robot moving among the obstacles represented by protein atoms.
To model the ligand shape and flexibility, it is necessary to consider ligand translation, rotation, and also additional degrees of freedom (DOF) defined by the flexible dihedral angles.
Finding trajectories for such a system leads to a search in a high-dimensional configuration space, which can be efficiently solved using sampling-based planners~\cite{Lav06}.											  

																									   
We utilize a modified Rapidly Exploring Random Tree Planner (RRT)~\cite{vonasek2017tunnel} that builds a tree of collision-free configurations. 
Each configuration defines the position, translation, and internal DOFs of the ligand.
Unlike classic RRT-based planners that sample the space uniformly, we utilize AWD to sample the position of the ligand only in the vicinity of the AWD vertices.											
This allows us to find trajectories along the tunnels and it is also faster than sampling the whole configuration space uniformly.
The method is iterative. 
In each iteration, a random sample is generated around a vertex of AWD and its nearest node in the tree is found.
Then, the tree is expanded towards the random sample from the nearest node. 
During the expansion, collisions between the ligand and the protein are checked and the tree is expanded only by the collision-free nodes.
The method terminates if the tree reaches the desired goal state, which is defined, e.g., as the position of the active site.

																														   


The computed trajectories are then evaluated using an energy function (using Vina Autodock tool~\cite{trott2009autodock}) and visualized, in order to help biochemists evaluate the relevance of the trajectories.
For that, we propose a visual analysis approach, consisting of 2D and 3D views onto the trajectories and helping the user to filter out trajectories according to different properties, such as the ligand energy.

%\section{Visualization}						
We describe and demonstrate the proposed visual analysis of the ligand trajectories on a dataset consisting of 10,000 trajectories that screen the binding of 1,2-dibromoethane (DBE) to the LinB86 protein (PDB ID 5LKA).
The trajectories were computed using 200 snapshots (20 ns) from a longer molecular dynamics (MD) simulation (5,380 ns) which was performed without the ligand.
Then, in each snapshot, 50 trajectories were computed for the DBE using motion planning.
The trajectories were assigned with the binding energies.

In order to visualize the results of the screening, we focus on the binding energy profiles as they drive the biological relevance of the trajectories.
We plot the trajectories as energy profiles to a chart, which depicts the system's binding energy vs. the distance of the ligand to the a priori known protein binding site (centroid of the Asp108 amino acid in our case).
We enable detailed exploration of the individual trajectories by clicking on a trajectory within the energies chart.
In this manner, a respective ligand position along its trajectory within a given MD snapshot is visualized using a 3D view (see Fig.~\ref{fig:energy}).

\begin{figure}[htb]
\centering
%\includegraphics[height=9em]{figures/energy}
%\hspace{1em}
%\includegraphics[height=9em]{figures/3d}
\includegraphics[height=10em]{figures/crop/energy}
\includegraphics[height=10em]{figures/crop/3d}
\caption{Trajectory energy chart; 3D visualization of ligand}
\label{fig:energy}
\end{figure}

Note that the energy chart becomes very dense even when plotting much less than 10,000 trajectories.
Therefore, we enable the aggregation of the energy profiles using the functional boxplot visualization~\cite{sun2011functional}.
In this way, a representative (median) profile can be extracted, as well as an overview of the whole profiles ensemble that can be conveyed by depicting particular data percentiles (see Fig.~\ref{fig:fbplot}).
Moreover, possible outlying profiles can be easily included into the visualization while keeping it clear and comprehensible.

\begin{figure}[htb]
\centering
%\includegraphics[height=8em]{figures/energy-all}
%\hspace{0.75em}
%\includegraphics[height=8em]{figures/energy-fbplot}
\includegraphics[height=9.5em]{figures/crop/energy-all}
\includegraphics[height=9.5em]{figures/crop/energy-fbplot}
\caption{Trajectory energy profiles; Functional boxplot of the profiles: median (black), 50. percentile (blue), outliers (gray circles)}
\label{fig:fbplot}
\end{figure}

While the aggregated visualization of the energy profiles helps to learn the overall trend from the data, it does not allow to learn about important differences among trajectory groups, e.g., clusters.
In order to address this issue, we provide the users with the scatter plot visualization of the following trajectory properties: i) minimum distance to the ligand binding site, and ii) area under energy curve, i.e., the total binding energy of a trajectory.
Using the scatterplot visualization of the properties, the user can define one or more selections of trajectories.
Furthermore, we enable to compare the user-defined selections by superimposing their functional boxplot representations within a common chart (see Fig.~\ref{fig:superfbplot}).

\begin{figure}[htb]
\centering
\includegraphics[height=7.5em]{figures/scatterplots}
\hspace{0.2em}
\includegraphics[height=8em]{figures/fbplots}
\caption{Two trajectory groups selected using scatterplot; Superimposed energy boxplots of the groups}
\label{fig:superfbplot}
\end{figure}

\section{Conclusion and Future Work}

We plan to extend our computation and visual analysis framework such that we will be able to cover whole MD simulation data (>50,000 snapshots).

\section*{Acknowledgments}
This work was supported by the project 17-07690S of the Czech Science Foundation.

%-------------------------------------------------------------------------
\bibliographystyle{eg-alpha}
\bibliography{poster}

\end{document}
