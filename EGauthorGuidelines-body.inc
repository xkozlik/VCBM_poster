% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v2.02, Jan 25, 2017


\title{Analysis the Traversability of Protein Tunnels by Flexible Ligands}

% for anonymous conference submission please enter your SUBMISSION ID
% instead of the author's name (and leave the affiliation blank) !!
% for final version: please provide your *own* ORCID in the brackets following \orcid; see https://orcid.org/ for more details.
\author[A. Jur\v{c}\'{i}k et al.]
{\parbox{\textwidth}{\centering Adam Jur\v{c}\'{i}k$^{1}$
        and Vojt\v{e}ch Von\'{a}sek$^{2}$
				and Martin Ma\v{n}\'{a}k$^{3}$
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
TODO

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
The van der Waals model of a protein is given by centers $\mathbf{c}_i \in R^3$ and radii $r_i \in R^+$ of individual atoms. 
An example with a ligand bound to the protein is depicted in Figure~\ref{fig:vd}. 
Dynamic models add the movement of atoms by specifying positions in subsequent time-snapshots. 
In our research, we develop methods for planning almost collision-avoiding paths for flexible ligands in dynamic protein models. 
We join computational geometry concepts, path-planning approaches, and advanced visualization techniques to target this problem.

\section{Geometry}

The additively weighted Voronoi diagram (AWD) is a collection of regions $V\!R_i$ assigned to the corresponding atoms.
\[V\!R_i = \{x \in \mathbf{R}^3 \,:\,|\mathbf{x}-\mathbf{c}_i|-r_i \leq |\mathbf{x}-\mathbf{c_j}|-r_j\,\forall j \in I_n \}\]
Voronoi vertices and edges provide us a description of free space among atoms with respect to a spherical probe as it is depicted on a 2D analogy in Figure~\ref{fig:vd}. 
Our program computes AWD for each time-snapshot of the dynamic model in a pre-processing phase and then offers this data to the motion-planning part. 
The rationale is that paths that are good for a single probe can also be good for a complex ligand.

\begin{figure}[htb]
\centering
\includegraphics[height=10em]{figures/4b1m}
\hspace{1em}
\includegraphics[height=10em]{figures/awd}
\caption{The protein 4B1M \cite{Cuskin2012} and its van der Waals model with a ligand; The~\mbox{aw-Voronoi} diagram for the analysis of free space by spherical probes}
\label{fig:vd}
\end{figure}

\section{Path-planning}

By searching paths in the constructed AWD, tunnels can be identified in the protein.
These tunnels however describe possible pathways for single atom.

To compute trajectories for non-spherical ligands, considering their shape and conformational changes, 
motion planning techniques, originally studied in the field of robotics, are applied.
The ligand is considered as a flexible robot moving among obstacles defined by the protein atoms.
It is necessary to consider ligand translation, rotation and also additional degrees of freedom defined by the flexible dihedral angles.
Finding of trajectories for such a system leads to a search in a high-dimensional configuration space, which can be efficiently
solved using sampling-based planners~\cite{Lav06}.

We utilize a modified Rapidly Exploring Random Tree Planner (RRT)~\cite{vonasek2017tunnel}, that builds
a tree of collision-free configurations of the ligand (a configuration defines position, translation and internal DOFs of the ligand).
Unlike classic RRT-based planners, that sample the space uniformly, we utilize the constructed AWD to sample the position
of the ligands near the vertices of the AWD.
This allows us to find trajectories along the tunnels and it is also faster than sampling the whole configuration space uniformly.
In each iteration, a random sample is generated around a vertex of AWD and its nearest node in the tree is found.
Then, the tree is expanded towards the random sample from the nearest node. 
During the expansion, collision-detection between the ligand and the protein is checked and the tree is expanded only by the 
collision-free nodes.
The method terminates if the tree reaches the desired goal state, which is  defined e.g. as the position of the active site.

The resulting trajectories are then evaluated using an energy function (using Vina Autodock tool~\cite{trott2009autodock}).


\section{Visualization}

We demonstrate the proposed visual analysis of the ligand trajectories on a dataset consisting of 10,000 trajectories that screen binding of 1,2-dibromoethane (DBE) to the LinB86 protein (PDB ID 5LKA).
The trajectories were computed using 200 snapshots (20 ns) from a longer MD (5,380 ns) which was simulated without the ligand.
Then, in each snapshot 50 trajectories were computed for the DBE using motion-planning.
The trajectories were assigned with binding energies.

In order to visualialize the results of the screening we focus on the binding energy profiles as they drive the biological relevancy of the trajectories.
We plot the trajectories as energy profiles to a chart which depicts system's binding energy vs. ligand's distance to the a priori known protein's binding site (centroid of Asp108).
We enable detailed exploration of the individual trajectories by clicking on a trajectory within the energies chart.
In this manner, a respective ligand position along its trajectory within the repsective MD snapshot is visualized using a 3D view (see Figure~\ref{fig:energy}).

\begin{figure}[htb]
\centering
\includegraphics[height=9em]{figures/energy}
\hspace{1em}
\includegraphics[height=9em]{figures/3d}
\caption{TODO caption, quality}
\label{fig:energy}
\end{figure}

Note, the energy chart becomes easily crowded even when plotting much less than 10,000 trajectories.
Therefore, we enable aggregration of the energy profiles using a functional boxplot visualization~\cite{sun2011functional}.
In this way, a representative (median) profile can be extracted as well as an overview of the whole profiles ensemble can conveyed by depicting particular data percentiles (see Figure~\ref{fig:fbplot}).
Moreover, possible outlying profiles can be easily included into the visualization while keeping it clear.

\begin{figure}[htb]
\centering
\includegraphics[height=6.5em]{figures/energy-all}
\hspace{1em}
\includegraphics[height=6.5em]{figures/energy-fbplot}
\caption{TODO caption, font size, outliers boundary, quality}
\label{fig:fbplot}
\end{figure}

While the aggregated visualization of the energy profiles helps to learn the overall trend from the data, it does not allow to learn about important differencies among trajectory groups, e.g., clusters.
In order to address this issue we provide users with scatter plot visualization of the following trajectory properties: i) minimum distance to ligand binding site, and ii) area under energy curve, i.e., the total binding energy of a trajectory.
Using the scatterplot visualization of the properties, the user can define one or more selections of trajectories.
Furthermore, we enable to compare the user-defined selections by superimposing their boxplot representations within a common chart (see Figure~\ref{fig:superfbplot}).

\begin{figure}[htb]
\centering
\includegraphics[height=9em]{figures/energy-fbplot}
\caption{TODO caption}
\label{fig:superfbplot}
\end{figure}

\section{Future Work}

We plan to extend our computation and visual analysis framework such that we will be able to cover whole MD simulation data (>50,000 snapshots).

\section*{Acknowledgments}
This work was supported by the project 17-07690S of the Czech Science Foundation.

%-------------------------------------------------------------------------
\bibliographystyle{eg-alpha}
\bibliography{poster}

\end{document}
