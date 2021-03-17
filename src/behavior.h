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

      const Double3 dendrite_axis = dendrite->GetSpringAxis();

      if ( dendrite->IsTerminal() )
      {
        const double growth_weight = 0.50;
        const Double3 growth_direction = dendrite_axis * growth_weight;

        const double randomness_weight = 0.35;
        const double old_direction_weight = 1.50;

        auto random_axis = random->template UniformArray<3>(-1, 1);

        const Double3 random_direction = random_axis * randomness_weight;

        const Double3 old_direction = dendrite_axis * old_direction_weight;

        Double3 new_step_direction =
            old_direction + random_direction + growth_direction;
        // elongate the dendrite toward new_step_direction
        dendrite->ElongateTerminalEnd(50, new_step_direction);
        // shrink diameter
        dendrite->SetDiameter(dendrite->GetDiameter() * 0.999);
      }

      if ( dendrite->IsTerminal() &&
           dendrite->GetDiameter() > 0.6 &&
           random->Uniform() < dendrite->GetDiameter() * 0.01 )
      {
        auto rand_noise = random->template UniformArray<3>(-2, 2);
        //
        const Double3 branch_direction =
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
