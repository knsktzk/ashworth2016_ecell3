# ashworth2016_ecell3

Lipid metabolic zonation model on E-Cell3 converted from Ashworth model(Ashworth et al., 2016)

About [E-Cell3](https://www.e-cell.org/projects/ecell3.html)

## Reference Publication
- William B. Ashworth, Nathan A. Davies, I. David L. Bogle. A Computational Model of Hepatic Energy Metabolism: Understanding Zonated Damage and Steatosis in NAFLD. PLoS Comput Biol. 2016 Sep 15;12(9):e1005105. doi: 10.1371/journal.pcbi.1005105. eCollection 2016 Sep. PubMed PMID: 27632189

  - Supporting Information: [S1 Code: XPPAUT Model File](https://doi.org/10.1371/journal.pcbi.1005105.s004)

## Model Files
- `BASE_MODEL.em`
  - A model using custom `Process`. Just execute `$ make` to build cpp files.
- `run.py`
  - Run the simulation with the following command
  - `$ ecell3-session run.py`
  - Simulation results are in `result.csv`
