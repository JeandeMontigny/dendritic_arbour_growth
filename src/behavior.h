#ifndef BIOLOGY_MODULES_H_
#define BIOLOGY_MODULES_H_

#include "core/behavior/behavior.h"
#include "core/resource_manager.h"
#include "neuroscience/neuroscience.h"

namespace bdm {

struct DendriteGrowth : public Behavior {
  BDM_BEHAVIOR_HEADER(DendriteGrowth, Behavior, 1);

  DendriteGrowth() { AlwaysCopyToNew(); }
  virtual ~DendriteGrowth() {}

  void Run(Agent* agent) override {
    auto* sim = Simulation::GetActive();
    auto* random = sim->GetRandom();

    auto* dendrite = bdm_static_cast<neuroscience::NeuriteElement*>(agent);

    if (dendrite->GetDiameter() > 0.5) {

      Double3 growth_direction = {0, 0, 1};

      double randomness_weight = 1;
      double old_direction_weight = 2;

      Double3 random_axis = {random->Uniform(-1, 1), random->Uniform(-1, 1),
                             random->Uniform(-1, 1)};
      Double3 random_direction = random_axis * randomness_weight;
      Double3 old_direction = dendrite->GetSpringAxis() * old_direction_weight;

      Double3 new_step_direction =
          old_direction + random_direction + growth_direction;

      // elongate the dendrite toward new_step_direction
      dendrite->ElongateTerminalEnd(10, new_step_direction);
      // shrink diameter
      dendrite->SetDiameter(dendrite->GetDiameter() - 0.001);

      if (dendrite->IsTerminal() && dendrite->GetDiameter() > 0.6
          && random->Uniform(0, 1) < dendrite->GetDiameter() * 0.01) {
        auto rand_noise = random->template UniformArray<3>(-0.1, 0.1);
        Double3 branch_direction =
            Math::Perp3(dendrite->GetUnitaryAxisDirectionVector() + rand_noise,
                        random->Uniform(0, 1)) + dendrite->GetSpringAxis();
        auto* dendrite_2 = bdm_static_cast<neuroscience::NeuriteElement*>(
            dendrite->Branch(branch_direction));
        dendrite_2->SetDiameter(dendrite->GetDiameter());
      } // end if branching

    }  // end if diameter > x
  }    // end run

};  // end DendriteGrowth

}  // namespace bdm

#endif  // BIOLOGY_MODULES_H_
