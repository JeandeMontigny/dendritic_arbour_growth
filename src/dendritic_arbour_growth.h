// -----------------------------------------------------------------------------
//
// Copyright (C) 2021 CERN & Newcastle University for the benefit of the
// BioDynaMo collaboration. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------
#ifndef DENDRITIC_ARBOUR_GROWTH_H_
#define DENDRITIC_ARBOUR_GROWTH_H_

#include "biodynamo.h"
#include "behavior.h"
#include "neuroscience/neuroscience.h"

namespace bdm {

inline int Simulate(int argc, const char** argv) {
  neuroscience::InitModule();
  Simulation simulation(argc, argv);
  auto* rm = simulation.GetResourceManager();
  auto* rand = simulation.GetRandom();

  for (int N=0; N<20; N++)
  {
    Double3 xyz = {rand->Uniform(0,100), rand->Uniform(0,100), 0.0};
    //
    auto* neuron = new neuroscience::NeuronSoma(xyz);
    neuron->SetDiameter(10);
    rm->AddAgent(neuron);
    {
      auto* neurite = neuron->ExtendNewNeurite({0.,0.,+1.});
      neurite->SetDiameter(1);
      neurite->AddBehavior(new DendriteGrowth());
    }
    {
      auto* neurite = neuron->ExtendNewNeurite({0.,0.,-1.});
      neurite->SetDiameter(1);
      neurite->AddBehavior(new DendriteGrowth());
    }
  }

  std::cout << "Simulating... " << std::endl;
  simulation.GetScheduler()->Simulate(250);

  return 0;
}

}  // namespace bdm

#endif  // DENDRITIC_ARBOUR_GROWTH_H_
