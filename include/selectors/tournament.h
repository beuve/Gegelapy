#include "selector/tournamentSelector.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyTournament : public Selector::TournamentSelector {
public:
  PyTournament(bool areElitesReproductible, double ratioSavedRoots, uint64_t sizeTournament)
      : TournamentSelector(){
        params->tournament.areElitesReproductible = areElitesReproductible;
        params->tournament.ratioSavedRoots = ratioSavedRoots;
        params->tournament.sizeTournament = sizeTournament;
      }
};
